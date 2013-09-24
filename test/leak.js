var watermarker = require('..');
var debug       = 1;
var memwatch    = require('memwatch');

memwatch.on( 'leak', function( info ) {
  console.log( "leak: ", info );
});

memwatch.on('stats', function(stats) {
  console.log( "stats: ", stats );
});

var hd = new memwatch.HeapDiff();

var ret = watermarker.run({
  imageData: require('fs').readFileSync( "./test/photo1.jpg" ),
  watermarkData: require('fs').readFileSync( "./test/watermark.png" ),
  debug: debug
});

require('assert').ok( ret );

var diff = hd.end();
console.log("diff: ", diff );
