#pragma once

#include <hwlib.hpp>
#include <type_traits>

#include "st7735.hpp"

namespace r2d2::display {

    /**
     * Class st7735_buffered is an interface for the st7735 chip
     * 
     * Implements hwlib::window to easily use text and drawing functions that
     * are already implemented. Extends from r2d2::display::ssd1306_i2c_c
     * 
     * The template paramters are required for the parent class
     */
    template<std::size_t Cursor_Count, uint8_t Display_Size_Width, uint8_t Display_Size_Height>
    class st7735_buffered_c : public st7735_c<Cursor_Count, Display_Size_Width, Display_Size_Height> {
    protected:
        uint16_t buffer[Display_Size_Width * Display_Size_Height] = {};

    public:
        /**
         * @brief Construct a new st7735 unbuffered c object
         *
         * @param bus
         * @param cs
         * @param dc
         * @param reset
         */
        st7735_buffered_c(hwlib::spi_bus &bus, hwlib::pin_out &cs,
                          hwlib::pin_out &dc, hwlib::pin_out &reset)
            : st7735_c<Cursor_Count, Display_Size_Width, Display_Size_Height>(bus, cs, dc, reset) 
        {}

        /**
         * @brief Directly write a pixel to the screen
         *
         * @param x
         * @param y
         * @param data Data is the color of the pixel
         */
        void set_pixel(uint16_t x, uint16_t y, const uint16_t data) override  {

            const uint16_t t = __REV16(data); // make a copy and reverse byte order

            // write pixel data to the buffer
            this->buffer[x + (y * this->width)] = t;
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
                        const uint16_t *data) override  {
            // unfortunaly the arduino due is little endian otherwise we could put
            // all the data directly to the buffer
            for (size_t v = 0; v < height; v++) {
                for (size_t t = 0; t < width; t++) {
                    const uint16_t g = __REV16(data[(v * width) + t]);

                    buffer[(x + t) + ((y + v) * this->width)] = g;
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
            
            uint16_t g = __REV16(data); // make a copy and reverse byte order

            for (size_t v = 0; v < height; v++) {
                for (size_t t = 0; t < width; t++) {
                    buffer[(x + t) + ((y + v) * this->width)] = g;
                }            
            }
        }

        /**
         * @brief Flushes the display
         * 
         */
        void flush() override {
            st7735_buffered_c::set_cursor(0, 0, this->width - 1, this->height - 1);

            // write to ram
            st7735_buffered_c::write_command(st7735_buffered_c::_RAMWR);

            // write data to display
            st7735_buffered_c::write_data((uint8_t *)buffer, this->width * this->height * 2);
        }
    };

} // namespace r2d2::display
