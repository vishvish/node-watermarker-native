{
  "targets": [
  {
    "target_name": "addon",
      "sources": [ "src/watermarker.cc" ],
      "conditions": [
        ['OS=="mac"', {
          "include_dirs": [
            "<!(node -e \"require('nan')\")",
            "<!(brew --config | grep PREFIX | sed -n -e 's/^.*: //p')/include/GraphicsMagick"
          ],
          'cflags': [
            '-v'
          ],
          'ldflags': [
            '-v'
          ],
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'OTHER_CFLAGS': [
              '-mmacosx-version-min=10.7',
            '-std=c++11',
            '-stdlib=libc++',
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
  }
  ]
}

