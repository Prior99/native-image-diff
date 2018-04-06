# native-image-diff

[![npm](https://img.shields.io/npm/v/native-image-diff.svg)](https://www.npmjs.com/package/native-image-diff)
[![Build Status](https://travis-ci.org/Prior99/native-image-diff.svg?branch=master)](https://travis-ci.org/Prior99/native-image-diff)
[![Build status](https://ci.appveyor.com/api/projects/status/go1birwl4uvqhj49/branch/master?svg=true)](https://ci.appveyor.com/project/Prior99/native-image-diff/branch/master)
[![Coverage Status](https://coveralls.io/repos/github/Prior99/native-image-diff/badge.svg?branch=master)](https://coveralls.io/github/Prior99/native-image-diff?branch=master)

A simple library for comparing two images using a native C++ binding.

Please also refer to the **[Documentation](https://prior99.github.io/native-image-diff/docs/index.html)**.

## Table of contents

 * [native-image-diff](#native-image-diff)
    * [Table of contents](#table-of-contents)
    * [Supported environments](#supported-environments)
    * [Usage](#usage)
    * [Contributing](#contributing)
    * [Contributors](#contributors)

## Usage

```typescript
import { diffImages } from "native-image-diff";

// Somehow get buffers to an RGB or RGBA image.
const imageBuffer1 = Buffer.alloc(300);
const imageBuffer2 = Buffer.alloc(300);

const image1 = {
    data: imageBuffer1,
    width: 20,
    height: 5
};

const image2 = {
    data: imageBuffer2,
    width: 20,
    height: 5
};

const { image, pixels, totalDelta } = diffImages(image1, image2);
// `image` now containes the width and the height of an RGBA image
// visualizing the difference between `image1` and `image2`.
// `pixels` is the amount of pixels which didn't match.
// `totalDelta` is a relatively scaled number with higher values being
// the most different and `0` being total equal.
```

## Supported environments

This is a native Addon to NodeJS which delivers prebuilt binaries. Only some environments are supported:

| Node Version      | Windows 64-Bit     | Windows 32-Bit     | Linux 64-Bit       | Linux 32-Bit | OSX                |
|-------------------|--------------------|--------------------|--------------------|--------------|--------------------|
| Earlier           | -                  | -                  | -                  | -            | -                  |
| Node 6 *(Abi 48)* | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | -            | :heavy_check_mark: |
| Node 7 *(Abi 51)* | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | -            | :heavy_check_mark: |
| Node 8 *(Abi 57)* | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | -            | :heavy_check_mark: |
| Node 9 *(Abi 59)* | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | -            | :heavy_check_mark: |

## Contributing

Yarn is used instead of npm, so make sure it is installed, probably: `npm install -g yarn`.

Generally, it should be enough to just run:

```
make
```

which will install all node dependencies and compile the C++ code,
compile typescript, execute all test, lint the sources and build the docs.

## Contributors

 - Alexander Dmitriev
 - Frederick Gnodtke
