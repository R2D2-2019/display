#pragma once
#include <cstdint>

namespace r2d2::display {
    struct display_screen_s {
        uint8_t screen_height;
        uint8_t screen_width;
        uint8_t screen_x_offset;
        uint8_t screen_y_offset;
    };
} // namespace r2d2::display