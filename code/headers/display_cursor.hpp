#pragma once

#include <hwlib.hpp>
/**
 * Display_cursor keeps track of side information, like position and color.
 */
namespace r2d2::display {
    struct display_cursor{
        uint8_t cursor_x = 0;
        uint8_t cursor_y = 0;
        hwlib::color cursor_color = hwlib::color(0, 0, 0);
        
    };
} // namespace r2d2::display