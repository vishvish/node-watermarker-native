var ben       = require('ben')
,   watermarker        = require('watermarker')
,   im_native = require('..')
,   async     = require('async')
,   assert    = require('assert')
,   debug     = 0
;

var file  = "./test/photo1.jpg";

var body  = require('fs').readFileSync( file );

function watermark (callback) {
    watermarker.run({
      imageData: require('fs').readFileSync( "./test/photo2.jpg" ),
      watermarkData: require('fs').readFileSync( "./test/watermark.png" )
    }, function (err,stdout, stderr) {
        assert( stdout.length > 0 );
        // console.log( "im length: "+stdout.length );
        require('fs').writeFileSync( "./test/out.fork.jpg", stdout, 'binary' );
        callback();
    });
}

async.waterfall([
    function (callback) {
        console.log( "before resize: ", process.memoryUsage() );
        callback();
    },
    function (callback) {
        ben.async( resize, function (ms) {
            console.log( "resize: " + ms + "ms per iteration" );
            callback();
        });
    },
    function (callback) {
        console.log( "after resize: ", process.memoryUsage() );
        callback();
    },
    function (callback) {
        resize_native( callback );
    },
    function (callback) {
        console.log( "after resize_native 1st: ", process.memoryUsage() );
        callback();
    },
    function (callback) {
        ben.async( resize_native, function (ms) {
            console.log( "resize_native: " + ms + "ms per iteration" );
            callback();
        });
    },
    function (callback) {
        console.log( "after resize_native: ", process.memoryUsage() );
        callback();
    },
], function(err) {
    if ( err ) {
        console.log("err: ", err);
    }
});

/*
resize:       16.09ms per iteration
resize_native: 0.89ms per iteration
*/
