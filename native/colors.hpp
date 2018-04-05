#ifndef COLORS_HPP
#define COLORS_HPP

#include <stdint.h>

/**
 * Represents a color in RGB colorspace with alpha channel.
 */
struct rgba {
    float r;
    float g;
    float b;
    float a;

    rgba(const uint8_t *data);
};

/**
 * Represents a color in RGB colorspace.
 */
struct rgb {
    float r;
    float g;
    float b;

    rgb(const uint8_t *data);

    /**
     * Convert a color in RGB colorspace with alpha channel to a color in RGB colorspace without.
     * The alpha channel is not dropped, instead it is blended to white.
     */
    rgb(const rgba &input);
};

float rgbToY(const rgb &input);

float rgbToI(const rgb &input);

float rgbToQ(const rgb &input);

float deltaY(const float &firstY, const float &secondY);

float deltaI(const float &firstI, const float &secondI);

float deltaQ(const float &firstQ, const float &secondQ);

/**
 * Represents a color in YIQ colorspace.
 */
struct yiq {
    float y;
    float i;
    float q;

    /**
     * Convert a color in RGB colorspace to YIQ colorspace.
     */
    yiq(const rgb &input);
};

#endif
