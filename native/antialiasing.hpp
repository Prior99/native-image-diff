#ifndef ANTIALIASING_HPP
#define ANTIALIASING_HPP

#include <stdint.h>
#include "utils.hpp"

/**
 * Check if a pixel is likely part of antialiasing.
 */
bool isAntialiasing(const ImageMeta *first, const ImageMeta *second, const uint32_t &inputX, const uint32_t &inputY);

#endif
