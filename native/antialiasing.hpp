#ifndef ANTIALIASING_HPP
#define ANTIALIASING_HPP

#include <stdint.h>
#include "utils.hpp"

struct XY {
    uint32_t x;
    uint32_t y;
};

struct Rect {
    XY start;
    XY end;
};

struct AntialiasingInfo {
    XY darkest;
    XY brightest;
    bool notAntialiasing;
};

/**
 * Returns a rect with all eight neighbours of the specified center included.
 */
Rect getNeighbourBoundaries(const XY &center, const ImageMeta &image);

/**
 * Retrieve the darkest and brightest pixel as well as information about whether it is already clear
 * that the specified pixel isn't part of antialiasing.
 */
void getAntialiasingInfo(const ImageMeta &image, const XY inputCoords, AntialiasingInfo &info);

/**
 * Check whether more than two of the eight neighbours of the specified center are equal.
 */
bool moreThanTwoNeighboursEqual(const ImageMeta &image, const XY center);

/**
 * Check if a pixel is likely part of antialiasing but wasn't in the other image and the
 * other way around.
 */
bool compareAntialiasing(const ImageMeta &first, const ImageMeta &second, const uint32_t x, const uint32_t y);

#endif
