#include <algorithm>
#include "antialiasing.hpp"
#include "utils.hpp"
#include <limits>

using namespace std;

Rect getNeighbourBoundaries(const XY &center, const ImageMeta &image) {
    return {
        { max(center.x - 1, 0u), max(center.y - 1, 0u) },
        { min(center.x + 1, image.width - 1), min(center.y + 1, image.height - 1) }
    };
}


bool compareAntialiasing(const ImageMeta &first, const ImageMeta &second, const uint32_t x, const uint32_t y) {
    const auto diffAntialiasingUnidirectional = [&x, &y](const ImageMeta &from, const ImageMeta &to) {
        AntialiasingInfo info;
        getAntialiasingInfo(from, { x, y }, info);
        // If the initial interation already detected the pixel not to be antialiasing, exit early and return `false`.
        if (info.notAntialiasing) {
            return false;
        }
        // Check the brightest and darkest pixels in the source and compared image.
        return (!moreThanTwoNeighboursEqual(from, info.darkest) && !moreThanTwoNeighboursEqual(to, info.darkest)) ||
            (!moreThanTwoNeighboursEqual(from, info.brightest) && !moreThanTwoNeighboursEqual(to, info.brightest));
    };
    return diffAntialiasingUnidirectional(first, second) || diffAntialiasingUnidirectional(second, first);
}

void getAntialiasingInfo(const ImageMeta &image, const XY center, AntialiasingInfo &result) {
    // The boundaries of all 8 adjacent pixels.
    const auto bounds = getNeighbourBoundaries(center, image);
    // The amount of pixels with equal brightness.
    uint32_t equalPixels = 0;
    // The amount of pixels with a positive delta (The neighbouring pixel was brighter).
    uint32_t brighterPixels = 0;
    // The amount of pixels with a negative delta (The neighbouring pixel was darker).
    uint32_t darkerPixels = 0;
    // The value and coordinates of the brightest pixel.
    float brightest = 0;
    XY brightestCoords;
    // The value and coordinates of the darkest pixel.
    float darkest = 0;
    XY darkestCoords;

    const auto inputIndex = (center.y * image.width + center.x) * image.bytesPerPixel;
    const auto inputBrightness = rgbToY(rgbAt(image.hasAlpha, image.data, inputIndex));

    // Iterate over all 8 adjacent pixels.
    for (uint32_t y = bounds.start.y; y <= bounds.end.y; ++y) {
        for (uint32_t x = bounds.start.x; x <= bounds.end.x; ++x) {
            // Only take the surrounding pixels into account, not the pixel we are testing.
            if (y == center.y && x == center.x) { continue; }
            const auto currentIndex = (y * image.width + x) * image.bytesPerPixel;
            const auto currentBrightness = rgbToY(rgbAt(image.hasAlpha, image.data, currentIndex));
            const auto brightnessDelta = inputBrightness - currentBrightness;
            // Adjust the counters for brighter, darker and equal pixels.
            if (brightnessDelta == 0) { equalPixels++; }
            else if (brightnessDelta < 0) { darkerPixels++; }
            else { brighterPixels++; }
            // If more than two adjacent pixels were equal, it wasn't antialiasing.
            if (equalPixels > 2) {
                result.notAntialiasing = true;
                return;
            }
            // Remember the darkest pixel.
            if (brightnessDelta < darkest) {
                darkest = brightnessDelta;
                darkestCoords = { x, y };
            }
            // Remember the brightest pixel.
            if (brightnessDelta > brightest) {
                brightest = brightnessDelta;
                brightestCoords = { x, y };
            }
        }
    }
    // If there are no darker or brighter adjacent pixels, it's not antialiasing.
    if (brighterPixels == 0 || darkerPixels == 0) {
        result.notAntialiasing = true;
        return;
    }
    result.notAntialiasing = false;
    result.darkest = darkestCoords;
    result.brightest = brightestCoords;
    return;
}

bool moreThanTwoNeighboursEqual(const ImageMeta &image, const XY center) {
    // The boundaries of all 8 adjacent pixels.
    const auto bounds = getNeighbourBoundaries(center, image);
    // The amount of pixels with equal brightness.
    uint32_t equalPixels = 0;

    const auto inputIndex = (center.y * image.width + center.x) * image.bytesPerPixel;
    const auto inputBrightness = rgbToY(rgbAt(image.hasAlpha, image.data, inputIndex));

    // Iterate over all 8 adjacent pixels.
    for (uint32_t y = bounds.start.y; y <= bounds.end.y; ++y) {
        for (uint32_t x = bounds.start.x; x <= bounds.end.x; ++x) {
            // Only take the surrounding pixels into account, not the pixel we are testing.
            if (y == center.y && x == center.x) { continue; }
            const auto currentIndex = (y * image.width + x) * image.bytesPerPixel;
            const auto currentBrightness = rgbToY(rgbAt(image.hasAlpha, image.data, currentIndex));
            // Adjust the counters for brighter, darker and equal pixels.
            if (inputBrightness == currentBrightness) { equalPixels++; }
            if (equalPixels > 2) { return false; }
        }
    }
    return true;
}
