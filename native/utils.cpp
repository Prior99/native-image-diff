#include "utils.hpp"

ColorRGB rgbAt(const bool hasAlpha, const uint8_t *data, const uint32_t index) {
    return hasAlpha ? ColorRGB(ColorRGBA(data + index)) : ColorRGB(data + index);
}


