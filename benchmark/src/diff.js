const Benchmark = require("benchmark");
const NodeLibpng = require("node-libpng");
const NativeImageDiff = require("native-image-diff");
const Pixelmatch = require("pixelmatch");
const drawChart = require("./chart");

module.exports = () => new Promise(resolve => {
    console.log("Benchmarking diff")
    const image1 = NodeLibpng.readPngFileSync(`${__dirname}/../../images/rainbow-1.png`);
    const image2 = NodeLibpng.readPngFileSync(`${__dirname}/../../images/rainbow-2.png`);
    const suite = new Benchmark.Suite();
    console.log(image1, image2)
            NativeImageDiff.diffImages(image1, image2);
    suite
        .add("native-image-diff", () => {
            NativeImageDiff.diffImages(image1, image2);
        })
        .add("pixelmatch", () => {
            const output = Buffer.alloc(512 * 512 * 4);
            Pixelmatch(image1.data, image2.data, output, 512, 512, { threshold: 0.1, includeAA: false });
        })
        .on("cycle", event => console.log(String(event.target)))
        .on("complete", () => drawChart(suite, `${__dirname}/../benchmark-diff.png`, resolve))
        .run();
});
