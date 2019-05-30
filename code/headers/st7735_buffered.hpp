#pragma once

#include <hwlib.hpp>
#include <st7735.hpp>
#include <type_traits>

namespace r2d2::display {

    /**
     * Class st7735_buffered is an interface for the st7735 chip
     *
     * Implements hwlib::window to easily use text and drawing functions that
     * are already implemented. Extends from r2d2::display::st7735_c
     *
     * The template paramters are required for the parent class
     */
    template <std::size_t CursorCount, class DisplayScreen>
    class st7735_buffered_c : public st7735_c<CursorCount, DisplayScreen> {
    protected:
        uint16_t buffer[DisplayScreen::width * DisplayScreen::height] = {};

    public:
        /**
         * @brief Construct a new st7735_unbuffered_c object
         *
         * @param bus
         * @param cs
         * @param dc
         * @param reset
         */
        st7735_buffered_c(hwlib::spi_bus &bus, hwlib::pin_out &cs,
                          hwlib::pin_out &dc, hwlib::pin_out &reset)
            : st7735_c<CursorCount, DisplayScreen>(
                  bus, cs, dc, reset) {
        }

        /**
         * @brief Directly write a pixel to the screen
         *
         * @param x
         * @param y
         * @param data
         */
        void set_pixel(uint16_t x, uint16_t y, const uint16_t data) override {

            const uint16_t current_width =
                __REV16(data); // make a copy and reverse byte order

            // write pixel data to the buffer
            this->buffer[x + (y * this->width)] = current_width;
        }

        /**
         * @brief Directly write multiple pixels to the screen. This effectively
         * draws a rectangle based on given location and size.
         *
         *
         * @param x
         * @param y
         * @param width
         * @param height
         * @param data Data is a pointer to the color of the pixel
         */
        void set_pixels(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                        const uint16_t *data) override {
            // unfortunaly the arduino due is little endian otherwise we could
            // put all the data directly to the buffer
            for (std::size_t current_height = 0; current_height < height; current_height++) {
                for (std::size_t current_width = 0; current_width < width; current_width++) {
                    const uint16_t inveted_data = __REV16(data[(current_height * width) + current_width]);

                    buffer[(x + current_width) + ((y + current_height) * this->width)] = inverted_data;
                }
            }
        }

        /**
         * @brief Directly fill multiple pixels with the same color to the
         * screen
         *
         * @param x
         * @param y
         * @param width
         * @param height
         * @param data Data is the color of all pixels
         */
        void set_pixels(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                        const uint16_t data) override {

            uint16_t inverted_data = __REV16(data); // make a copy and reverse byte order

            for (std::size_t current_height = 0; current_height < height; current_height++) {
                for (std::size_t current_width = 0; current_width < width; current_width++) {
                    buffer[(x + current_width) + ((y + current_height) * this->width)] = inverted_data;
                }
            }
        }

        /**
         * @brief Flushes the display
         *
         */
        void flush() override {
            st7735_buffered_c::set_cursor(0, 0, this->width - 1,
                                          this->height - 1);

            // write to ram
            st7735_buffered_c::write_command(st7735_buffered_c::_RAMWR);

            // write data to display
            st7735_buffered_c::write_data((uint8_t *)buffer,
                                          this->width * this->height * 2);
        }
    };

} // namespace r2d2::display
