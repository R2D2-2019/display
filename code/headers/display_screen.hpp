#pragma once

#include <cstdint>

namespace r2d2::display {
    struct st7735_128x160_s {
        constexpr static uint8_t width = 128;
        constexpr static uint8_t height = 160;
        constexpr static uint8_t x_offset = 0;
        constexpr static uint8_t y_offset = 0;
    };

    struct st7735_80x160_s {
        constexpr static uint8_t width = 80;
        constexpr static uint8_t height = 160;
        constexpr static uint8_t x_offset = 26;
        constexpr static uint8_t y_offset = 1;
    };

    struct ssd1306_128x64_s {
        constexpr static uint8_t width = 128;
        constexpr static uint8_t height = 64;
        constexpr static uint8_t x_offset = 0;
        constexpr static uint8_t y_offset = 0;
    };
} // namespace r2d2::display
