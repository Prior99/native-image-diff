#include <nan.h>

#include "image-diff.hpp"

NAN_MODULE_INIT(InitNativeImageDiff) {
    InitImageDiff(target);
}

NODE_MODULE(native_image_diff, InitNativeImageDiff)
