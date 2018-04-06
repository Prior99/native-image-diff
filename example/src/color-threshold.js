const NodeLibpng = require("node-libpng");
const NativeImageDiff = require("native-image-diff");

// Read the images from PNG images on the disk.
const imageRed = NodeLibpng.readPngFileSync(`${__dirname}/../../images/red-rectangle-example-red.png`);
const imageGradient = NodeLibpng.readPngFileSync(`${__dirname}/../../images/red-rectangle-example-gradient.png`);

// Compare the images with different color thresholds.
for (let colorThreshold = 0; colorThreshold <= 0.7; colorThreshold += 0.1) {
    const result = NativeImageDiff.diffImages({
        image1: imageRed,
        image2: imageGradient,
        colorThreshold,
        detectAntialiasing: false,
    });
    const percent = Math.floor(colorThreshold * 100);
    // Log the result for this threshold.
    console.log(`Color threshold ${percent}%: ${result.pixels} pixels were different.`);
    // Write the diff image.
    NodeLibpng.writePngFileSync(
        `${__dirname}/../../images/red-rectangle-example-${percent}-diff.png`,
        result.image.data,
        result.image,
    );
}
