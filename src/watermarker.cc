#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif  // BUILDING_NODE_EXTENSION

#include "watermarker.h"
#include <list>
#include <string.h>
#include <exception>
#include <sstream>
using namespace std; 
using namespace Magick;

#define THROW_ERROR_EXCEPTION(x) ThrowException(v8::Exception::Error(String::New(x))); \
  scope.Close(Undefined())

// input
//   args[ 0 ]: options. required, object with following key,values
//              {
//                  imageData:        required. Buffer with binary image data
//                  debug:          optional. 1 or 0
//              }
Handle<Value> Watermark(const Arguments& args) {
  HandleScope scope;

  Local<Object> obj = Local<Object>::Cast( args[ 0 ] );
  Local<Object> imageData     = Local<Object>::Cast( obj->Get( String::NewSymbol("imageData") ) );
  Local<Object> watermarkData = Local<Object>::Cast( obj->Get( String::NewSymbol("watermarkData") ) );

  if ( imageData->IsUndefined() || ! node::Buffer::HasInstance(imageData) ) {
    return THROW_ERROR_EXCEPTION("composite()'s 1st argument should have \"imageData\" key with a Buffer instance");
  }

  int debug = obj->Get( String::NewSymbol("debug") )->Uint32Value();
  if (debug) printf( "debug: on\n" );

  Magick::Blob imageBlob( node::Buffer::Data(imageData), node::Buffer::Length(imageData) );
  Magick::Blob watermarkBlob( node::Buffer::Data(watermarkData), node::Buffer::Length(watermarkData) );

  Magick::Image image;
  try {
    image.read( imageBlob );
  }
  catch (std::exception& err) {
    std::string message = "image.read failed with error: ";
    message += err.what();
    return THROW_ERROR_EXCEPTION(message.c_str());
  }
  catch (...) {
    return THROW_ERROR_EXCEPTION("unhandled error");
  }

  Magick::Image watermark;
  try {
    watermark.read( watermarkBlob );
  }
  catch (std::exception& err) {
    std::string message = "watermark.read failed with error: ";
    message += err.what();
    return THROW_ERROR_EXCEPTION(message.c_str());
  }
  catch (...) {
    return THROW_ERROR_EXCEPTION("unhandled error");
  }

  int hTiles = (image.rows() + watermark.rows() - 1) / watermark.rows();
  int vTiles = (image.columns() + watermark.columns() - 1) / watermark.columns();
  int ww = watermark.rows() + 50;
  int wh = watermark.columns() + 50;

  for(int i = 0; i < hTiles; i++) {
    for(int j = 0; j < vTiles; j++) {
      image.composite(watermark, (ww * i), (wh * j), Magick::OverCompositeOp);
    }
  }

  Magick::Blob outputBlob;
  image.write( &outputBlob );

  node::Buffer* retBuffer = node::Buffer::New( outputBlob.length() );
  memcpy( node::Buffer::Data( retBuffer->handle_ ), outputBlob.data(), outputBlob.length() );
  return scope.Close( retBuffer->handle_ );
}

Handle<Value> Resize(const Arguments& args) {
  HandleScope scope;

  Local<Object> obj       = Local<Object>::Cast( args[ 0 ] );
  Local<Object> imageData = Local<Object>::Cast( obj->Get( String::NewSymbol( "imageData" ) ) );
  unsigned int width      = obj->Get( String::New( "width" ) )->Uint32Value();
  unsigned int height     = obj->Get( String::New( "height" ) )->Uint32Value();
  unsigned int quality    = obj->Get( String::New( "quality" ) )->IntegerValue();

  if ( imageData->IsUndefined() || ! node::Buffer::HasInstance( imageData ) ) {
    return THROW_ERROR_EXCEPTION("composite()'s 1st argument should have \"imageData\" key with a Buffer instance");
  }

  Magick::Blob imageBlob( node::Buffer::Data( imageData ), node::Buffer::Length( imageData ) );
  Magick::Image image;

  try {
    image.read( imageBlob );
  }
  catch ( std::exception& err ) {
    std::string message = "image.read failed: ";
    message += err.what();
    return THROW_ERROR_EXCEPTION( message.c_str() );
  }
  catch( ... ) {
    return THROW_ERROR_EXCEPTION( "error" ); 
  }

  std::ostringstream os;
  os << width << "x" << height << ">";
  std::string s = os.str();

  Magick::Geometry resizeTo( s );
  Magick::Blob outputBlob;
  image.quality( quality );
  image.scale( resizeTo );
  image.write( &outputBlob );

  node::Buffer* retBuffer = node::Buffer::New( outputBlob.length() );
  memcpy( node::Buffer::Data( retBuffer->handle_ ), outputBlob.data(), outputBlob.length() );
  return scope.Close( retBuffer->handle_ );
}

void init(Handle<Object> target, char **argv) {
  InitializeMagick(*argv);
  target->Set(String::NewSymbol("watermark"), FunctionTemplate::New(Watermark)->GetFunction());
  target->Set(String::NewSymbol("resize"), FunctionTemplate::New(Resize)->GetFunction());
}

NODE_MODULE(watermarker, init)
