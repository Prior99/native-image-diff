#include <algorithm>
#include "antialiasing.hpp"
#include "utils.hpp"
#include <iostream>
#include <limits>

using namespace std;

bool isAntialiasing(
    uint8_t *firstData,
    const uint32_t &firstWidth,
    const uint32_t &firstHeight,
    const bool &firstHasAlpha,
    const bool &firstBytesPerPixel,
    uint8_t *secondData,
    const uint32_t &secondWidth,
    const uint32_t &secondHeight,
    const bool &secondHasAlpha,
    const bool &secondBytesPerPixel,
    const uint32_t &inputX,
    const uint32_t &inputY
) {
    // The boundaries of all 8 adjacent pixels.
    const auto xStart = max(inputX - 1, static_cast<uint32_t>(0));
    const auto xEnd = min(inputX + 1, min(firstWidth, secondWidth) - 1);
    const auto yStart = max(inputY + 1, static_cast<uint32_t>(0));
    const auto yEnd = min(inputY + 1, min(firstHeight, secondHeight) - 1);
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
    float darkest = numeric_limits<float>::max();
    uint32_t darkestX = 0;
    uint32_t darkestY = 0;

    const auto inputIndex = (inputY * firstWidth + inputX) * firstBytesPerPixel;
    const auto inputBrightness = rgbToY(rgbAt(firstHasAlpha, firstData, inputIndex));

    // Iterate over all 8 adjacent pixels.
    for (uint32_t y = yStart; y <= yEnd; ++y) {
        for (uint32_t x = xStart; x <= xEnd; ++x) {
            const auto currentIndex = (y * firstWidth + x) * firstBytesPerPixel;
            // Only take the surrounding pixels into account, not the pixel we are testing.
            if (y == inputY && x == inputX) { continue; }
            const auto currentBrightness = rgbToY(rgbAt(firstHasAlpha, firstData, currentIndex));
            const auto brightnessDelta = deltaY(inputBrightness, currentBrightness);
            // Adjust the counters for brighter, darker and equal pixels.
            if (brightnessDelta == 0) { equalPixels++; }
            else if (brightnessDelta > 0) { brighterPixels++; }
            else { darkerPixels++; }
            // If more than two adjacent pixels were equal, it wasn't antialiasing.
            if (equalPixels > 2) { return false; }
            if (secondData == nullptr) { continue; }
            // Remember the darkest pixel.
            if (brightnessDelta < darkest) {
                std::cout << "New darkest pixel: " << brightnessDelta << " at " << x << "," << y << std::endl;
                darkest = brightnessDelta;
                darkestX = x;
                darkestY = y;
            }
            // Remember the brightest pixel.
            if (brightnessDelta > brightest) {
                std::cout << "New brightest pixel: " << brightnessDelta << " at " << x << "," << y << std::endl;
                brightest = brightnessDelta;
                brightestX = x;
                brightestY = y;
            }
        }
    }
    if (secondData == nullptr) { return true; }
    // If there are no darker or brighter adjacent pixels, it's not antialiasing.
    if (brighterPixels == 0 || darkerPixels == 0) { return false; }
    // If either the darkest or the brightest pixel has more than 2 equal adjacent pixels in both images this pixel is antialiasing.
    return (
        !isAntialiasing(firstData, firstWidth, firstHeight, firstHasAlpha, firstBytesPerPixel, nullptr, secondWidth, secondHeight, secondHasAlpha, secondBytesPerPixel, darkestX, darkestY) &&
        !isAntialiasing(secondData, firstWidth, firstHeight, firstHasAlpha, firstBytesPerPixel, nullptr, secondWidth, secondHeight, secondHasAlpha, secondBytesPerPixel, darkestX, darkestY)
    ) || (
        !isAntialiasing(firstData, firstWidth, firstHeight, firstHasAlpha, firstBytesPerPixel, nullptr, secondWidth, secondHeight, secondHasAlpha, secondBytesPerPixel, brightestX, brightestY) &&
        !isAntialiasing(secondData, firstWidth, firstHeight, firstHasAlpha, firstBytesPerPixel, nullptr, secondWidth, secondHeight, secondHasAlpha, secondBytesPerPixel, brightestX, brightestY)
    );
}
