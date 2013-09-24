var test = require('tap').test;
var watermarker = require('..');
var debug = 1;

function saveToFileIfDebug (buffer, file) {
  if (debug) {
    require('fs').writeFileSync( file, buffer, 'binary' );
    console.log( "wrote file: "+file );
  }
}

test( 'watermark an image in a tiled manner', function(t) {
  var buffer;
  try {
    buffer = watermarker.run({
      imageData: require('fs').readFileSync( "./test/photo.jpg" ),
      watermarkData: require('fs').readFileSync( "./test/watermark.png" )
    });
  } catch (e) {
    t.equal( e.message, 'watermark aborted', 'err message' );
  }
  t.equal( Buffer.isBuffer(buffer), true, 'buffer is Buffer' );
  // t.equal( buffer.length, 2295, 'converted buffer size ok' );
  saveToFileIfDebug( buffer, "./test/photo-watermarked.jpg" );
  t.end();
})

