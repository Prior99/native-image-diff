const NodeLibpng = require("node-libpng");
const NativeImageDiff = require("native-image-diff");

// Read the images from PNG images on the disk.
const imageNoAntialiasing =
    NodeLibpng.readPngFileSync(`${__dirname}/../../images/antialiasing-example-no-antialiasing.png`);
const imageAntialiasing =
    NodeLibpng.readPngFileSync(`${__dirname}/../../images/antialiasing-example-with-antialiasing.png`);

// Compare both images with antialiasing detection enabled.
const resultDetectionEnabled = NativeImageDiff.diffImages({
    image1: imageNoAntialiasing,
    image2: imageAntialiasing,
    // This can be omitted; It's `true` by default.
    detectAntialiasing: true,
});
console.log(`Antialiasing detection enabled: ${resultDetectionEnabled.pixels} pixels were different.`);
// Write the diff image to the disk.
NodeLibpng.writePngFileSync(
    `${__dirname}/../../images/antialiasing-example-diff-detection-enabled.png`,
    resultDetectionEnabled.image.data,
    resultDetectionEnabled.image,
);

// Compare both images with antialiasing detection disabled.
const resultDetectionDisabled = NativeImageDiff.diffImages({
    image1: imageNoAntialiasing,
    image2: imageAntialiasing,
    detectAntialiasing: false,
});
console.log(`Antialiasing detection disabled: ${resultDetectionDisabled.pixels} pixels were different.`);
// Write the diff image to the disk.
NodeLibpng.writePngFileSync(
    `${__dirname}/../../images/antialiasing-example-diff-detection-disabled.png`,
    resultDetectionDisabled.image.data,
    resultDetectionDisabled.image,
);
