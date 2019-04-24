#pragma once

#include <hwlib.hpp>

#include <i2c_bus.hpp>
#include <ssd1306.hpp>

namespace r2d2::display {
    /**
     * SSD1306 unbuffered interface for an oled
     * Implements hwlib::window to easily use text and drawing functions that
     * are already implemented. Extends from r2d2::display::ssd1306_i2c_c
     */
    class ssd1306_oled_unbuffered_c : public ssd1306_i2c_c {
    private:
        /**
         * The buffer with the pixel data
         * The first byte is used for the data-prefix that the display driver
         * requires to be sent when sending pixel data.
         */
        uint8_t buffer[width * height / 8 + 1] = {};

    public:
        /**
         * Construct the display driver by providing the communication bus and
         * the address of the display.
         */
        ssd1306_oled_unbuffered_c(r2d2::i2c::i2c_bus_c &bus,
                                  const uint8_t &address);

        /**
         * @brief Write a pixel to the screen
         *
         * @param x
         * @param y
         * @param data
         */
        void set_pixel(uint16_t x, uint16_t y, const uint16_t data) override;

        /**
         * This clears the display this overrides the default clear of hwlib
         * becouse it is realy inefficient for this screen.
         */
        void clear(hwlib::color col) override;

        /**
         * This clears the display this overrides the default clear of hwlib
         * becouse it is realy inefficient for this screen.
         */
        void clear() override;
    };

} // namespace r2d2::display