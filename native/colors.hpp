#ifndef COLORS_HPP
#define COLORS_HPP

#include <stdint.h>

/**
 * Represents a color in RGB colorspace with alpha channel.
 */
struct ColorRGBA {
    float r;
    float g;
    float b;
    float a;

    ColorRGBA(const uint8_t *data);
};

/**
 * Represents a color in RGB colorspace.
 */
struct ColorRGB {
    float r;
    float g;
    float b;

    ColorRGB(const uint8_t *data);

    /**
     * Convert a color in RGB colorspace with alpha channel to a color in RGB colorspace without.
     * The alpha channel is not dropped, instead it is blended to white.
     */
    ColorRGB(const ColorRGBA &input);
};

float rgbToY(const ColorRGB &input);

float rgbToI(const ColorRGB &input);

float rgbToQ(const ColorRGB &input);

float deltaY(const float &firstY, const float &secondY);

float deltaI(const float &firstI, const float &secondI);

float deltaQ(const float &firstQ, const float &secondQ);

/**
 * Represents a color in YIQ colorspace.
 */
struct ColorYIQ {
    float y;
    float i;
    float q;

    /**
     * Convert a color in RGB colorspace to YIQ colorspace.
     */
    ColorYIQ(const ColorRGB &input);
};

#endif
