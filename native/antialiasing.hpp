#ifndef ANTIALIASING_HPP
#define ANTIALIASING_HPP

#include <stdint.h>

/**
 * Check if a pixel is likely part of antialiasing.
 */
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
);

#endif
