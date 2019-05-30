#pragma once

#include <hwlib.hpp>
#include <st7735.hpp>
#include <type_traits>

namespace r2d2::display {
    /**
     * Class st7735_unbuffered is an interface for the st7735 chip
     *
     * Implements hwlib::window to easily use text and drawing functions that
     * are already implemented. Extends from r2d2::display::st7735_c
     *
     * The template paramters are required for the parent class
     */
    template <std::size_t CursorCount, class DisplayScreen>
    class st7735_unbuffered_c : public st7735_c<CursorCount, DisplayScreen> {
    public:
        /**
         * @brief Construct a new st7735_unbuffered_c object
         *
         * @param bus
         * @param cs
         * @param dc
         * @param reset
         */
        st7735_unbuffered_c(hwlib::spi_bus &bus, hwlib::pin_out &cs,
                            hwlib::pin_out &dc, hwlib::pin_out &reset)
            : st7735_c<CursorCount, DisplayScreen>(bus, cs, dc, reset) {
        }

        /**
         * @brief Directly write a pixel to the screen
         *
         * @param x
         * @param y
         * @param data
         */
        void set_pixel(uint16_t x, uint16_t y, const uint16_t data) override {
            // set cursor on correct position
            st7735_unbuffered_c::set_cursor(x, y, x, y);

            // write to ram
            st7735_unbuffered_c::write_command(st7735_unbuffered_c::RAMWR);

            // make a copy and reverse byte order
            const uint16_t inverted_data = __REV16(data); 

            // write pixel data to the screeen
            st7735_unbuffered_c::write_data((uint8_t *)&inverted_data, 2);
        }

        /**
         * @brief Directly write multiple pixels to the screen
         *
         * @param x
         * @param y
         * @param width
         * @param height
         * @param data
         */
        void set_pixels(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                        const uint16_t *data) override {
            // set the window to the size we want to write to
            st7735_unbuffered_c::set_cursor(x, y, x + width, y + height);

            // write to ram
            st7735_unbuffered_c::write_command(st7735_unbuffered_c::RAMWR);

            // unfortunaly the arduino due is little endian otherwise we could
            // put all the data directly to the write_data function
            for (std::size_t i = 0; i < width * height; i++) {
                const uint16_t inverted_data = __REV16(data[i]);

                st7735_unbuffered_c::write_data((uint8_t *)&inverted_data, 2);
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
         * @param data
         */
        void set_pixels(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                        const uint16_t data) override {
            st7735_unbuffered_c::set_cursor(x, y, x + width, y + height);

            // write to ram
            st7735_unbuffered_c::write_command(st7735_unbuffered_c::RAMWR);
            // make a copy and reverse byte order
            const uint16_t inverted_data = __REV16(data); 

            for (std::size_t i = 0; i < std::size_t((width + 1) * (height + 1));
                 i++) {
                st7735_unbuffered_c::write_data((uint8_t *)&inverted_data, 2);
            }
        }
    };

} // namespace r2d2::display
