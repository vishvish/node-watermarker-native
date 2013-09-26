{
  "targets": [
  {
    "target_name": "watermarker",
      "include_dirs": [
        "<!(brew --config | grep PREFIX | sed -n -e 's/^.*: //p')/include/GraphicsMagick"
        ],

      "sources": [ "src/watermarker.cc" ],
      'cxxflags!': [ '-fno-exceptions' ],
      'cxxflags_cc!': [ '-fno-exceptions' ],

      "conditions": [

        ['OS=="mac"', {
          'cflags': [
            '-v'
            ],
          'ldflags': [
            '-v'
            ],
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'OTHER_CFLAGS': [
              '<!@(GraphicsMagick++-config --cxxflags)'
              ]
          },
          "libraries": [
            '<!@(GraphicsMagick++-config --ldflags --libs)',
          ],
          'cxxflags': [
            '<!@(GraphicsMagick++-config --cxxflags --cppflags)',
          '-v'
            ],
        }],

      ['OS=="linux"', {
        "libraries": [
          '<!@(GraphicsMagick++-config --ldflags --libs)',
        ],
        'cxxflags': [
          '<!@(GraphicsMagick++-config --cxxflags --cppflags)'
          ],
      }]
    ]
  }]
}
