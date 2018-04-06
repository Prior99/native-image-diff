const NodeLibpng = require("node-libpng");
const NativeImageDiff = require("native-image-diff");

// Read the images from PNG images on the disk.
const imageA = NodeLibpng.readPngFileSync(`${__dirname}/../../images/pixels-example-ya.png`);
const imageB = NodeLibpng.readPngFileSync(`${__dirname}/../../images/pixels-example-yay.png`);
// Compare both images. Don't generate a diff image.
const result = NativeImageDiff.diffImages({
    image1: imageA,
    image2: imageB,
    generateDiffImage: false,
});
// Log the result.
console.log(`${result.pixels} pixels were different.`);
