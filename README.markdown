# Node-Watermarker-Native

Subtle, full-cover, high-performance watermarking for Node.js, courtesy of [GraphicsMagick](http://www.graphicsmagick.org/)'s Magick++ C++ API binding.

With assistance from Masakazu Ohtsuka's [node-imagemagick-native](https://github.com/mash/node-imagemagick-native).

This is a high-performance version of the watermarker used on my photos  at [http://www.vishvish.com/](http://www.vishvish.com/). I required all-over, subtle, watermarking to reduce copyright theft by companies illegally selling prints and products using my images. Watermarking is a controversial topic amongst some photographers, but I felt that it was necessary, and a subtle covering pattern looks much better than a bold, ugly copyright notice in one corner, which can easily be cropped out.

The watermarker also provides resizing, bound to GraphicsMagick, as a separate operation.

## Examples

### Watermarking

    var watermarker = require('watermarker');

    var imageData = require('fs').readFileSync('./test/photo.jpg');
    var watermarkData = require('fs').readfilesync('./test/watermark.png');

    var watermarked = watermarker.watermark({
      imageData: imageData,
      watermarkData: watermarkData
    });

### Resizing

The resizer will maintain aspect ratio of the image, and the width/height values denotes the size of the box that the final image should fit into. The quality property is not optional, and should be above 80 for high quality images.

    var resized = watermarker.resize({
      imageData: imageData,
      width: 300,
      height: 300,
      quality: 90
    });

## Build

Instructions for OS X:

Via Homebrew:

    brew install graphicsmagick --build-from-source
    npm install
    grunt build
    npm test

The `binding.gyp` run automatically finds the correct include folder with Homebrew, which is useful when you have installed Homebrew outside of its default `/usr/local` which I feel is a terrible place to install it.

Check the `test` folder for processed images.

## Watermark Image

This should be a transparent, subtle image, saved as a PNG. Open the `watermark.png` in the test folder using Photoshop, create a background layer filled with black and see how gentle the watermark is. It works very well in compositing.

### Changelog

1.0.0

Basic tiling of watermark image across photograph.

1.0.1

Adding auto-discovery of correct Homebrew include directory.

---

## Photograph Copyright

Photos supplied for test purposes are Copyright (c) Vish Vishvanath and may only be used for testing this software and its derivatives, and may not be displayed, published, sold or otherwise reused without permission.

## Software License (MIT)

Copyright (c) Vish Vishvanath <http://www.vishvish.com/>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
