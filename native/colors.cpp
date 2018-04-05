#include <cmath>
#include "colors.hpp"

rgba::rgba(const uint8_t *data) :
    r(static_cast<float>(data[0])),
    g(static_cast<float>(data[1])),
    b(static_cast<float>(data[2])),
    a(static_cast<float>(data[3])) {}

rgb::rgb(const uint8_t *data) :
    r(static_cast<float>(data[0])),
    g(static_cast<float>(data[1])),
    b(static_cast<float>(data[2])) {}


rgb::rgb(const rgba &input) :
    r(255.0f + (input.r - 255.0f) * input.a / 255.0f),
    g(255.0f + (input.g - 255.0f) * input.a / 255.0f),
    b(255.0f + (input.b - 255.0f) * input.a / 255.0f) {}

float rgbToY(const rgb &input) {
    return input.r * 0.29889531 + input.g * 0.58662247 + input.b * 0.11448223;
}

float rgbToI(const rgb &input) {
    return input.r * 0.59597799 - input.g * 0.27417610 - input.b * 0.32180189;
}

float rgbToQ(const rgb &input) {
    return input.r * 0.21147017 - input.g * 0.52261711 + input.b * 0.31114694;
}

float deltaY(const float &firstY, const float &secondY) {
    return 0.5053 * pow(firstY - secondY, 2);
}

float deltaI(const float &firstI, const float &secondI) {
    return 0.299 * pow(firstI - secondI, 2);
}

float deltaQ(const float &firstQ, const float &secondQ) {
    return 0.1957 * pow(firstQ - secondQ, 2);
}

yiq::yiq(const rgb &input) :
    y(rgbToY(input)),
    i(rgbToI(input)),
    q(rgbToQ(input)) {}
