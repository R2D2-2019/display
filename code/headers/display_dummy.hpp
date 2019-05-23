#pragma once

#include <hwlib.hpp>
#include <display_cursor.hpp>
#include <display_adapter.hpp>

namespace r2d2::display{

    template <std::size_t CursorCount, uint8_t DisplaySizeWidth, uint8_t DisplaySizeHeight>
    class display_dummy_c : public display_c<CursorCount, DisplaySizeWidth, DisplaySizeHeight>{
        public:
            display_dummy_c():
                display_c<CursorCount, DisplaySizeWidth, DisplaySizeHeight>(hwlib::xy(DisplaySizeWidth, DisplaySizeHeight)){

            }
            /**
             * @brief Converts a hwlib::color to the pixel data for the screen with
             * a maximum of two bytes for every pixel
             *
             * @param c
             */
            virtual uint16_t color_to_pixel(const hwlib::color &c) override {
                return 0;
            };

            /**
             * @brief Write a pixel to the screen
             *
             * @param x
             * @param y
             * @param data
             */
            virtual void set_pixel(uint16_t x, uint16_t y, const uint16_t data) override {};

            r2d2::display::display_cursor_s get_cursor(uint8_t cursor_id){
                return this->cursors[cursor_id];
            };
    };
}