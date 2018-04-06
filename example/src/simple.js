const NodeLibpng = require("node-libpng");
const NativeImageDiff = require("native-image-diff");

// Read the images from PNG images on the disk.
const imageA = NodeLibpng.readPngFileSync(`${__dirname}/../../images/pixels-example-ya.png`);
const imageB = NodeLibpng.readPngFileSync(`${__dirname}/../../images/pixels-example-yay.png`);
// Compare both images.
const result = NativeImageDiff.diffImages(imageA, imageB);
// Log the result.
console.log(`${result.pixels} pixels were different.`);
// Write the diff image to the disk.
NodeLibpng.writePngFileSync(`${__dirname}/../../images/pixels-example-diff.png`, result.image.data, result.image);
