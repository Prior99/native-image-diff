import { diffImages } from "../diff-images";
import { readPngFileSync, writePngFileSync } from "node-libpng";

describe("diffImages", () => {
    describe("red and blue rectangles without alpha channel", () => {
        const image1 = readPngFileSync(`${__dirname}/fixtures/blue-red-32x16-1.png`);
        const image2 = readPngFileSync(`${__dirname}/fixtures/blue-red-32x16-2.png`);

        it("diffs the two images as expected", () => {
            const { image, pixels, totalDelta } = diffImages({
                image1,
                image2,
            });
            expect(image.data).toEqual(readPngFileSync(`${__dirname}/diffs/red-and-blue.png`).data);
            expect(pixels).toBe(86);
            expect(totalDelta).toBe(1291946.25);
        });

        it("doesn't generate an output image if not desired by the user", () => {
            const { image, pixels, totalDelta } = diffImages({
                image1,
                image2,
                generateDiffImage: false,
            });
            expect(image).toBeUndefined();
            expect(pixels).toBe(86);
            expect(totalDelta).toBe(1291946.25);
        });
    });

    describe("red and blue rectangles with alpha channel", () => {
        const image1 = readPngFileSync(`${__dirname}/fixtures/blue-red-green-alpha-32x16-1.png`);
        const image2 = readPngFileSync(`${__dirname}/fixtures/blue-red-green-alpha-32x16-2.png`);

        it("diffs the two images as expected", () => {
            const { image, pixels, totalDelta } = diffImages({
                image1,
                image2,
            });
            expect(image.data).toEqual(readPngFileSync(`${__dirname}/diffs/red-and-blue-with-alpha.png`).data);
            expect(pixels).toBe(81);
            expect(totalDelta).toBe(956607.1875);
        });
    });

    describe("with an antialiased image", () => {
        const image1 = readPngFileSync(`${__dirname}/fixtures/antialiasing-75x75-1.png`);
        const image2 = readPngFileSync(`${__dirname}/fixtures/antialiasing-75x75-2.png`);

        it("with antialiasing detection enabled", () => {
            const { image, pixels, totalDelta } = diffImages({
                image1,
                image2,
                checkForAntialiasing: true,
            });
            expect(image.data).toEqual(readPngFileSync(`${__dirname}/diffs/antialiasing-enabled.png`).data);
            expect(totalDelta).toBe(0);
            expect(pixels).toBe(0);
        });

        it("with antialiasing detection disabled", () => {
            const { image, pixels, totalDelta } = diffImages({
                image1,
                image2,
                checkForAntialiasing: false,
            });
            expect(image.data).toEqual(readPngFileSync(`${__dirname}/diffs/antialiasing-disabled.png`).data);
            expect(totalDelta).toBe(788076.6875);
            expect(pixels).toBe(265);
        });
    });

    describe("with different sizes", () => {
        const image1 = readPngFileSync(`${__dirname}/fixtures/different-sizes-20x20-1.png`);
        const image2 = readPngFileSync(`${__dirname}/fixtures/different-sizes-30x25-2.png`);

        it("detects the difference", () => {
            const { image, pixels, totalDelta } = diffImages({ image1, image2 });
            expect(image.data).toEqual(readPngFileSync(`${__dirname}/diffs/different-sizes.png`).data);
            expect(pixels).toBe(350);
            expect(totalDelta).toBe(12325250);
        });
    });

    describe("with different thresholds", () => {
        const image1 = readPngFileSync(`${__dirname}/fixtures/threshold-red-100-4x4-1.png`);
        const image2 = readPngFileSync(`${__dirname}/fixtures/threshold-red-110-4x4-2.png`);

        it("detects no differing pixels with a threshold of 0.05", () => {
            const { image, pixels, totalDelta } = diffImages({
                image1,
                image2,
                colorThreshold: 0.05,
            });
            expect(totalDelta).toBe(0);
            expect(pixels).toBe(0);
            for (let i = 0; i < image.data.length; ++i) { expect(image.data[i]).toBe(0); }
        });

        it("detects a difference with a threshold of 0.01", () => {
            const { image, pixels, totalDelta } = diffImages({
                image1,
                image2,
                colorThreshold: 0.01,
            });
            expect(image.data).toEqual(readPngFileSync(`${__dirname}/diffs/threshold-0.01.png`).data);
            expect(totalDelta).toBe(64.0384521484375);
            expect(pixels).toBe(4);
        });
    });

    describe("with invalid inputs", () => {
        it("throws an error if one of the inputs didn't have a buffer as data", () => {
            expect(() => {
                diffImages({
                    image1: {
                        data: "a string",
                        width: 10,
                        height: 10,
                    },
                    image2: {
                        data: Buffer.alloc(300),
                        width: 10,
                        height: 10,
                    },
                } as any);
            }).toThrowErrorMatchingSnapshot();
        });

        it("throws an error if one of the inputs didn't have a valid width", () => {
            expect(() => {
                diffImages({
                    image1: {
                        data: Buffer.alloc(300),
                        width: "ten",
                        height: 10,
                    },
                    image2: {
                        data: Buffer.alloc(300),
                        width: 10,
                        height: 10,
                    },
                } as any);
            }).toThrowErrorMatchingSnapshot();
        });

        it("throws an error if one of the inputs didn't have a valid height", () => {
            expect(() => {
                diffImages({
                    image1: {
                        data: Buffer.alloc(300),
                        width: 10,
                        height: "ten",
                    },
                    image2: {
                        data: Buffer.alloc(300),
                        width: 10,
                        height: 10,
                    },
                } as any);
            }).toThrowErrorMatchingSnapshot();
        });

        it("throws an error if one of the inputs wasn't an object", () => {
            expect(() => {
                diffImages({
                    image1: "some image I found somewhere",
                    image2: {
                        data: Buffer.alloc(300),
                        width: 10,
                        height: 10,
                    },
                } as any);
            }).toThrowErrorMatchingSnapshot();
        });

        it("throws an error if one of the input buffers had an unexpected amount of bytes per pixel", () => {
            expect(() => {
                diffImages({
                    image1: {
                        data: Buffer.alloc(500),
                        width: 10,
                        height: 10,
                    },
                    image2: {
                        data: Buffer.alloc(300),
                        width: 10,
                        height: 10,
                    },
                } as any);
            }).toThrowErrorMatchingSnapshot();
        });
    });
});
