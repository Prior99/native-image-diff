#include "utils.hpp"

rgb rgbAt(const bool &hasAlpha, const uint8_t *data, const uint32_t &index) {
    return hasAlpha ? rgb(rgba(data + index)) : rgb(data + index);
}
