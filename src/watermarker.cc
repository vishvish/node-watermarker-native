#include "watermarker.h"
#include <iostream>

namespace demo {
  using v8::Exception;
  using v8::FunctionCallbackInfo;
  using v8::Isolate;
  using v8::Local;
  using v8::Number;
  using v8::Object;
  using v8::String;
  using v8::Value;

  void Method(const FunctionCallbackInfo<Value>& args) {
    Magick::InitializeMagick(NULL);

    Isolate* isolate = args.GetIsolate();

    Local<Object> obj = Local<Object>::Cast( args[ 0 ] );

     //Wot a palaver to get a string - this can't be right :)
    v8::Local<v8::String> imd =
      v8::String::NewFromUtf8(isolate, "imageData", v8::String::kInternalizedString);

    v8::Local<v8::String> wmd =
      v8::String::NewFromUtf8(isolate, "watermarkData", v8::String::kInternalizedString);

    Local<Object> imageData     = Local<Object>::Cast( obj->Get( imd ) );
    Local<Object> watermarkData = Local<Object>::Cast( obj->Get( wmd ) );

    Magick::Blob imageBlob( Buffer::Data(imageData), Buffer::Length(imageData) );
    Magick::Blob watermarkBlob( node::Buffer::Data(watermarkData), node::Buffer::Length(watermarkData) );

    Magick::Image image;

    try {
      image.read(imageBlob);
    }
    catch (std::exception& err) {
      isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "Image read failed with error.")));
      return;
    }
    catch (...) {
      isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "Unhandled exception.")));
      return;
    }

    Magick::Image watermark;

    try {
      watermark.read( watermarkBlob );
    }
    catch (std::exception& err) {
      isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "Watermark read failed with error.")));
      return;
    }
    catch (...) {
      isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "Unhandled exception.")));
      return;
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
    char *bChar = (char *)outputBlob.data();
    MaybeLocal<Object> retBuffer = Nan::NewBuffer(bChar, outputBlob.length());
    //memcpy( node::Buffer::Data( retBuffer->handle_ ), outputBlob.data(), outputBlob.length() );


    //args.GetReturnValue().Set(outputBlob);

    args.GetReturnValue().Set(retBuffer.ToLocalChecked());
    //args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
  }

  void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "hello", Method);
  }

  NODE_MODULE(addon, init)
}

