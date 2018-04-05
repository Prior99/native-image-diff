import { __native_imageDiff } from "./native";

/**
 * Represents the dimensions of an image (width and height) in pixels.
 */
export interface Dimensions {
    /**
     * The width of this image in pixels.
     */
    width: number;
    /**
     * The height of this image in pixels.
     */
    height: number;
}
/**
 * A diffable image with a width and a height as well as a buffer containing the data.
 * The data must be in RGB or RGBA format.
 */
export interface DiffImage extends Dimensions {
    /**
     * A node buffer containing the image information in RGB or RGBA format.
     */
    data: Buffer;
}

/**
 * The result of diffing two images.
 */
export interface DiffResult {
    /**
     * The amount of pixels that didn't match.
     */
    pixels: number;
    /**
     * The output image visually representing the difference between two images.
     */
    image?: DiffImage;
    /**
     * A relative number which grows from `0` the higher the total difference between both images was.
     * This also takes the color differences between individual pixels into account.
     */
    totalDelta: number;
}

/**
 * Returns the amount of bytes per pixels for the given image.
 *
 * @param image The image of which the bytes per pixels should be calculated.
 *
 * @return The amount of bytes per pixels for the provided image.
 */
function getBytesPerPixel(image: DiffImage): number {
    const { data, width, height } = image;
    return data.length / (width * height);
}

/**
 * Checks that the given object is a valid `DiffImage`.
 *
 * @see DiffImage
 *
 * @return Whether the given image is of type `DiffImage`.
 */
function isDiffImage(image: DiffImage): image is DiffImage {
    if (typeof image !== "object") { return false; }
    const { data, width, height } = image;
    if (!Buffer.isBuffer(data)) { return false; }
    if (!Number.isInteger(width)) { return false; }
    if (!Number.isInteger(height)) { return false; }
    const bytesPerPixel = getBytesPerPixel(image);
    if (bytesPerPixel !== 3 && bytesPerPixel !== 4) { return false; }
    return true;
}

/**
 * Calculate the common dimensions of two images. This will return the maximum width and height of both provided images.
 *
 * @param image1 The first image of which to calculate the wdth and height.
 * @param image2 The second image of which to calculate the wdth and height.
 *
 * @return The common maximum dimensions of both images.
 */
function getCommonDimensions(image1: DiffImage, image2: DiffImage): Dimensions {
    return {
        width: Math.max(image1.width, image2.width),
        height: Math.max(image1.height, image2.height),
    };
}

export interface DiffImagesArguments {
    /**
     * The first image to compare.
     */
    image1: DiffImage;
    /**
     * The second image to compare.
     */
    image2: DiffImage;
    /**
     * A threshold of perceived color difference between two pixels. Ranging from `0` to `1`
     * with `0` being the most sensitive and `1` ignoring basically any difference between two pixels.
     * This threshold is applied to each pixel. Defaults to `0.1`.
     */
    colorThreshold?: number;
    /**
     * Whether to attempt to detect antialiasing when comparing the images. Defaults to `true`.
     */
    checkForAntialiasing?: boolean;
    /**
     * If set to `false` no image visualizing the difference is generated.
     */
    generateDiffImage?: boolean;
}

/**
 * Compare two images and return the result including meta information and a diff image as a buffer.
 *
 * @return The information about the difference between the two images.
 */
export function diffImages({
    image1,
    image2,
    colorThreshold,
    checkForAntialiasing,
    generateDiffImage,
}: DiffImagesArguments): DiffResult {
    // Make sure the input is valid.
    if (!isDiffImage(image1) || !isDiffImage(image2)) {
        throw new Error("Provided image wasn't a diffable image.");
    }
    const safeColorThreshold = typeof colorThreshold === "number" ? colorThreshold : 0.1;
    const safeCheckForAntialiasing = typeof checkForAntialiasing === "boolean" ? checkForAntialiasing : true;
    const safeGenerateDiffImage = typeof generateDiffImage === "boolean" ? generateDiffImage : true;
    // Call the native module.
    const { pixels, totalDelta, imageData: data } = __native_imageDiff(
        image1.data,
        image1.width,
        image1.height,
        image2.data,
        image2.width,
        image2.height,
        safeColorThreshold,
        safeCheckForAntialiasing,
        safeGenerateDiffImage,
    );
    if (safeGenerateDiffImage) {
        const image = {
            ...getCommonDimensions(image1, image2),
            data,
        };
        return { pixels, totalDelta, image };
    }
    return { pixels, totalDelta };
}
