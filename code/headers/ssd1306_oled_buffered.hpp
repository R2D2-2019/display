#pragma once

#include <hwlib.hpp>

#include <display_adapter.hpp>
#include <i2c_bus.hpp>
#include <ssd1306.hpp>

namespace r2d2::display {
    /**
     * SSD1306 buffered interface for an oled
     * Implements hwlib::window to easily use text and drawing functions that
     * are already implemented. Extends from r2d2::display::ssd1306_i2c_c
     */
    class ssd1306_oled_buffered_c : public display_c, protected ssd1306_i2c_c {
    private:
        /**
         * The size of the display
         * This is needed even though hwlib::window keeps it's own size to
         * create the buffer at compile time.
         */
        static auto constexpr wsize = hwlib::xy(128, 64);

        /**
         * The buffer with the pixel data
         * The first byte is used for the data-prefix that the display driver
         * requires to be sent when sending pixel data.
         */
        uint8_t buffer[(wsize.x * wsize.y / 8) + 1] = {};

        /**
         * @brief converts a hwlib::color to the pixel data for the screen with
         * a maximum of two bytes for every pixel
         *
         * @param c
         */
        uint16_t color_to_pixel(const hwlib::color &c) override;

    public:
        /**
         * Construct the display driver by providing the communication bus and
         * the address of the display.
         */
        ssd1306_oled_buffered_c(r2d2::i2c::i2c_bus_c &bus,
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

        /**
         * Flushes the data to the display.
         * All the data is kept in a buffer on the arduino until this function
         * is called, it will then push the entirety of the buffer to the
         * display at once.
         */
        void flush();
    };

} // namespace r2d2::display