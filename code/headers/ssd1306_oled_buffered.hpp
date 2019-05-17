#pragma once

#include <hwlib.hpp>

#include <i2c_bus.hpp>
#include <ssd1306.hpp>

namespace r2d2::display {
    /**
     * SSD1306 buffered interface for an oled
     * Implements hwlib::window to easily use text and drawing functions that
     * are already implemented. Extends from r2d2::display::ssd1306_i2c_c
     *
     * The template parameters are used for the parent class.
     */
    template <std::size_t CursorCount, uint8_t DisplaySizeWidth,
              uint8_t DisplaySizeHeight>
    class ssd1306_oled_buffered_c
        : public ssd1306_i2c_c<CursorCount, DisplaySizeWidth,
                               DisplaySizeHeight> {
    private:
        /**
         * The buffer with the pixel data
         * The first byte is used for the data-prefix that the display driver
         * requires to be sent when sending pixel data.
         */
        uint8_t buffer[(DisplaySizeWidth * DisplaySizeHeight / 8) + 1] = {};

    public:
        /**
         * Construct the display driver by providing the communication bus and
         * the address of the display.
         */
        ssd1306_oled_buffered_c(r2d2::i2c::i2c_bus_c &bus,
                                const uint8_t &address)
            : ssd1306_i2c_c<CursorCount, DisplaySizeWidth, DisplaySizeHeight>(
                  bus, address) {
            // set the command for writing to the screen
            buffer[0] = this->ssd1306_data_prefix;

            // write the initalisation sequence to the screen
            bus.write(address, this->ssd1306_initialization,
                      sizeof(this->ssd1306_initialization) / sizeof(uint8_t));
        }

        /**
         * @brief Write a pixel to the screen
         *
         * @param x
         * @param y
         * @param data Colour of the pixel to write. data > 0 will set the pixel
         * Data <= 0 will clear the pixel
         */
        void set_pixel(uint16_t x, uint16_t y, const uint16_t data) override {
            // calculate the index of the pixel
            uint16_t t_index = (x + (y / 8) * this->size.x) + 1;

            // set or clear the pixel
            if (data) {
                buffer[t_index] |= (0x01 << (y % 8));
            } else {
                buffer[t_index] &= ~(0x01 << (y % 8));
            }
        }
        /**
         * This clears the display this overrides the default clear of hwlib
         * becouse it is realy inefficient for this screen.
         */
        void clear(hwlib::color col) override {
            // get a the data for the screen
            const uint8_t d = (col == hwlib::white) ? 0xFF : 0x00;

            // set all values to the color of the data
            for (uint_fast16_t x = 1; x < sizeof(buffer); ++x) {
                buffer[x] = d;
            }
        }

        /**
         * This clears the display this overrides the default clear of hwlib
         * becouse it is realy inefficient for this screen.
         */
        void clear() override {
            // clear the screen with the background
            clear(this->background);
        };

        /**
         * Flushes the data to the display.
         * All the data is kept in a buffer on the arduino until this function
         * is called, it will then push the entirety of the buffer to the
         * display at once.
         */
        void flush() {
            // update cursor of the display
            ssd1306_oled_buffered_c::command(
                ssd1306_oled_buffered_c::ssd1306_command::column_addr, 0, 127);
            ssd1306_oled_buffered_c::command(
                ssd1306_oled_buffered_c::ssd1306_command::page_addr, 0, 7);
            // write data to the screen
            this->bus.write(this->address, this->buffer, sizeof(this->buffer));
        }
    };

} // namespace r2d2::display