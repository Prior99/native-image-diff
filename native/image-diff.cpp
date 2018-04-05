#include <node_buffer.h>
#include <cmath>
#include <algorithm>
#include <stdint.h>
#include "image-diff.hpp"
#include "colors.hpp"
#include "utils.hpp"
#include "antialiasing.hpp"

using namespace node;
using namespace v8;
using namespace std;

float colorDelta(const rgb &firstColor, const rgb &secondColor) {
    const yiq firstYiq(firstColor);
    const yiq secondYiq(secondColor);
    return deltaY(firstYiq.y, secondYiq.y) + deltaI(firstYiq.i, secondYiq.i) + deltaQ(firstYiq.q, secondYiq.q);
}


void drawDiffPixel(
    uint8_t *diffImage,
    const uint32_t commonWidth,
    const uint32_t x,
    const uint32_t y,
    const uint8_t r,
    const uint8_t g,
    const uint8_t b
) {
    if (diffImage == nullptr) { return; }
    const auto index = (y * commonWidth + x) * 4;
    diffImage[index + 0] = r;
    diffImage[index + 1] = g;
    diffImage[index + 2] = b;
    diffImage[index + 3] = 255;
}

NAN_METHOD(imageDiff) {
    // 1st Parameter: The first image's buffer.
    Local<Object> firstBuffer = Local<Object>::Cast(info[0]);
    auto firstLength = Buffer::Length(firstBuffer);
    auto *firstData = reinterpret_cast<uint8_t*>(Buffer::Data(firstBuffer));
    // 2nd Parameter: The first image's width.
    const auto firstWidth = static_cast<uint32_t>(info[1]->NumberValue());
    // 3rd Parameter: The first image's height.
    const auto firstHeight = static_cast<uint32_t>(info[2]->NumberValue());
    // 4th Parameter: The second image's buffer.
    Local<Object> secondBuffer = Local<Object>::Cast(info[3]);
    auto secondLength = Buffer::Length(secondBuffer);
    auto *secondData = reinterpret_cast<uint8_t*>(Buffer::Data(secondBuffer));
    // 5th Parameter: The second image's width.
    const auto secondWidth = static_cast<uint32_t>(info[4]->NumberValue());
    // 6th Parameter: The second image's height.
    const auto secondHeight = static_cast<uint32_t>(info[5]->NumberValue());
    // 7th Parameter: The color threshold per pixel.
    const auto colorThreshold = static_cast<float>(info[6]->NumberValue());
    // 8th Parameter: Whether to check for antialiasing.
    const auto checkAntialiasing = static_cast<bool>(info[7]->BooleanValue());
    // 9th Parameter: Whether to generate an output image.
    const auto generateDiffImage = static_cast<bool>(info[8]->BooleanValue());
    // Computed values.
    const uint32_t firstBytesPerPixel = firstLength / (firstWidth * firstHeight);
    const uint32_t secondBytesPerPixel = secondLength / (secondWidth * secondHeight);
    const auto firstHasAlpha = firstBytesPerPixel == 4;
    const auto secondHasAlpha = secondBytesPerPixel == 4;
    const auto commonWidth = max(firstWidth, secondWidth);
    const auto commonHeight = max(firstHeight, secondHeight);
    // Initialize metadata structs for both images.
    const ImageMeta firstMeta = { firstData, firstWidth, firstHeight, firstHasAlpha, firstBytesPerPixel };
    const ImageMeta secondMeta = { secondData, secondWidth, secondHeight, secondHasAlpha, secondBytesPerPixel };
    // `35215` is the maximum difference betweend two colors in the YIQ colorspace.
    const float maxPossibleDelta = 35215.0f;
    // The max acceptable delta in YIQ color space. 
    const float maxAcceptedDelta = maxPossibleDelta * pow(colorThreshold, 2.0f);
    // The amount of pixels which didn't match.
    uint32_t pixels = 0;
    // The total combined delta between all pixels.
    float totalDelta = 0.0f;
    // The buffer for the diff image. Initialized to `nullptr` if no such image should be generated.
    uint8_t *diffImage = nullptr;
    const auto diffImageLength = 4 * commonWidth * commonHeight;
    if (generateDiffImage) {
        diffImage = new uint8_t[diffImageLength];
        for (uint32_t index = 0; index < diffImageLength; ++index) { diffImage[index] = 0; }
    }
    // Iterate over every pixel and compare both images.
    for (uint32_t y = 0; y < commonHeight; ++y) {
        for (uint32_t x = 0; x < commonWidth; ++x) {
            // If the current coordinates exceed the first or second image's dimensions, set the delta to absolute maximum.
            if (x >= firstWidth || x >= secondWidth || y >= firstHeight || y >= secondHeight) {
                totalDelta += maxPossibleDelta;
                pixels++;
                drawDiffPixel(diffImage, commonWidth, x, y, 255, 0, 0);
                continue;
            }

            const auto firstIndex = (y * firstWidth + x) * firstBytesPerPixel;
            const auto secondIndex = (y * secondWidth + x) * secondBytesPerPixel;
            const auto firstRgb = rgbAt(firstHasAlpha, firstData, firstIndex);
            const auto secondRgb = rgbAt(secondHasAlpha, secondData, secondIndex);
            const auto delta = colorDelta(firstRgb, secondRgb);
            // If the delta exceeded the accepted maximum difference for a pixel, check for antialiasing.
            if (delta > maxAcceptedDelta) {
                // If the user decided to take antialiasing into account, check if the pixel was part of antialiasing.
                if (checkAntialiasing && (isAntialiasing(&firstMeta, &secondMeta, x, y) || isAntialiasing(&secondMeta, &firstMeta, x, y))) {
                    // The pixel was part of antialiasing. Write a yellow pixel and continue with the loop.
                    drawDiffPixel(diffImage, commonWidth, x, y, 255, 255, 0);
                    continue;
                }
                // The pixel was not part of antialiasing or the user disabled the check. Draw a red pixel.
                drawDiffPixel(diffImage, commonWidth, x, y, 255, 0, 0);
                totalDelta += delta;
                pixels++;
            }
        }
    }
    // Return the calculated result.
    Local<Object> returnValue = Nan::New<Object>();
    returnValue->Set(Nan::New("pixels").ToLocalChecked(), Nan::New(static_cast<double>(pixels)));
    returnValue->Set(Nan::New("totalDelta").ToLocalChecked(), Nan::New(static_cast<double>(totalDelta)));
    if (generateDiffImage) {
        returnValue->Set(Nan::New("imageData").ToLocalChecked(), Nan::NewBuffer(reinterpret_cast<char*>(diffImage), diffImageLength).ToLocalChecked());
    }
    info.GetReturnValue().Set(returnValue);
}

NAN_MODULE_INIT(InitImageDiff) {
    target->Set(Nan::New("__native_imageDiff").ToLocalChecked(), Nan::New<FunctionTemplate>(imageDiff)->GetFunction());
}
