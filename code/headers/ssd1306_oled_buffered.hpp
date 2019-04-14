#pragma once

#include <hwlib.hpp>
#include <i2c_bus.hpp>
#include <ssd1306.hpp>

namespace r2d2::display {
    /**
     * SSD1306 buffered interface for an oled
     * Implements hwlib::window to easily use text and drawing functions that
     * are already implemented. Extends from r2d2::display::ssd1306_i2c_c
     */
    class ssd1306_oled_buffered_c : public r2d2::display::ssd1306_i2c_c,
                                    public hwlib::window {
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
         * The write implementation of hwlib::window
         * This is what allows us to re-use the existing code.
         * It sets a pixel at the given coordinate to the given color. In the
         * case of this specific display the color is either white or black.
         */
        void write_implementation(hwlib::xy pos, hwlib::color col) override;

    public:
        /**
         * Construct the display driver by providing the communication bus and
         * the address of the display.
         */
        ssd1306_oled_buffered_c(r2d2::i2c::i2c_bus_c &bus,
                                const uint8_t &address);

        /**
         * Clears the display.
         * Sets all pixels to "off"
         */
        void clear() override;

        /**
         * Flushes the data to the display.
         * All the data is kept in a buffer on the arduino until this function
         * is called, it will then push the entirety of the buffer to the
         * display at once.
         */
        void flush() override;
    };

} // namespace r2d2::display