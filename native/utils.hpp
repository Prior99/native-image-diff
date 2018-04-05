#ifndef UTILS_HPP
#define UTILS_HPP

#include <stdint.h>
#include "colors.hpp"

ColorRGB rgbAt(const bool hasAlpha, const uint8_t *data, const uint32_t index);

struct ImageMeta {
    const uint8_t *const data;
    const uint32_t width;
    const uint32_t height;
    const bool hasAlpha;
    const uint32_t bytesPerPixel;
};

#endif
