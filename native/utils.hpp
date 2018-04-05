#ifndef UTILS_HPP
#define UTILS_HPP

#include <stdint.h>
#include "colors.hpp"

rgb rgbAt(const bool &hasAlpha, const uint8_t *data, const uint32_t &index);

#endif
