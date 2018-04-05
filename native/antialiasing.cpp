#include <algorithm>
#include "antialiasing.hpp"
#include "utils.hpp"
#include <limits>

using namespace std;

bool isAntialiasing(const ImageMeta *first, const ImageMeta *second, const uint32_t &xInput, const uint32_t &yInput) {
    // The boundaries of all 8 adjacent pixels.
    const auto xStart = max(xInput - 1, static_cast<uint32_t>(0));
    const auto yStart = max(yInput - 1, static_cast<uint32_t>(0));
    const auto xEnd = min(xInput + 1, first->width - 1);
    const auto yEnd = min(yInput + 1, first->height - 1);
    // The amount of equal pixels.
    uint32_t equalPixels = 0;
    // The amount of pixels with a positive delta (The other pixel was brighter).
    uint32_t brighterPixels = 0;
    // The amount of pixels with a negative delta (The other pixel was darker).
    uint32_t darkerPixels = 0;
    // The value and coordinates of the brightest pixel.
    float brightest = 0;
    uint32_t brightestX = 0;
    uint32_t brightestY = 0;
    // The value and coordinates of the darkest pixel.
    float darkest = 0;
    uint32_t darkestX = 0;
    uint32_t darkestY = 0;

    const auto inputIndex = (yInput * first->width + xInput) * first->bytesPerPixel;
    const auto inputBrightness = rgbToY(rgbAt(first->hasAlpha, first->data, inputIndex));

    // Iterate over all 8 adjacent pixels.
    for (uint32_t y = yStart; y <= yEnd; ++y) {
        for (uint32_t x = xStart; x <= xEnd; ++x) {
            const auto currentIndex = (y * first->width + x) * first->bytesPerPixel;
            // Only take the surrounding pixels into account, not the pixel we are testing.
            if (y == yInput && x == xInput) { continue; }
            const auto currentBrightness = rgbToY(rgbAt(first->hasAlpha, first->data, currentIndex));
            const auto brightnessDelta = inputBrightness - currentBrightness;
            // Adjust the counters for brighter, darker and equal pixels.
            if (brightnessDelta == 0) { equalPixels++; }
            else if (brightnessDelta < 0) { darkerPixels++; }
            else { brighterPixels++; }
            // If more than two adjacent pixels were equal, it wasn't antialiasing.
            if (equalPixels > 2) { return false; }
            if (second == nullptr) { continue; }
            // Remember the darkest pixel.
            if (brightnessDelta < darkest) {
                darkest = brightnessDelta;
                darkestX = x;
                darkestY = y;
            }
            // Remember the brightest pixel.
            if (brightnessDelta > brightest) {
                brightest = brightnessDelta;
                brightestX = x;
                brightestY = y;
            }
        }
    }
    if (second == nullptr) { return true; }
    // If there are no darker or brighter adjacent pixels, it's not antialiasing.
    if (brighterPixels == 0 || darkerPixels == 0) { return false; }
    // If either the darkest or the brightest pixel has more than 2 equal adjacent pixels in both images this pixel is antialiasing.
    return (
        !isAntialiasing(first, nullptr, darkestX, darkestY) &&
        !isAntialiasing(second, nullptr, darkestX, darkestY)
    ) || (
        !isAntialiasing(first, nullptr, brightestX, brightestY) &&
        !isAntialiasing(second, nullptr, brightestX, brightestY)
    );
}
