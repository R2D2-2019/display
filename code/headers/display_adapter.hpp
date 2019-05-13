#pragma once

#include <hwlib.hpp>
#include <display_cursor.hpp>
namespace r2d2::display {
    class display_c : public hwlib::window {
    protected:
        /**
         * @brief Write implementation for hwlib
         *
         * @param pos
         * @param col
         */
        void write_implementation(hwlib::xy pos, hwlib::color col) {
            set_pixel(pos.x, pos.y, color_to_pixel(col));
        }
        
        // The font used in the display is 8*8 
        hwlib::font_default_8x8 display_font = hwlib::font_default_8x8();

        // Keeps track of cursors (for now hardcoded as 5 cursors)
        uint8_t cursor_size = 5;
        r2d2::display::display_cursor cursors[5];
    public:
        display_c(hwlib::xy size, hwlib::color foreground = hwlib::white,
                hwlib::color background = hwlib::black)
            : hwlib::window(size, foreground, background) {
        }

        /**
         * @brief Converts a hwlib::color to the pixel data for the screen with
         * a maximum of two bytes for every pixel
         *
         * @param c
         */
        virtual uint16_t color_to_pixel(const hwlib::color &c) = 0;

        /**
         * @brief Write a pixel to the screen
         *
         * @param x
         * @param y
         * @param data
         */
        virtual void set_pixel(uint16_t x, uint16_t y, const uint16_t data) = 0;

        /**
         * @brief Write multiple pixels to the screen
         *
         * @param x
         * @param y
         * @param width
         * @param height
         * @param data
         */
        virtual void set_pixels(uint16_t x, uint16_t y, uint16_t width,
                                uint16_t height, const uint16_t *data);

        /**
         * @brief Fill multiple pixels with the same color to the
         * screen
         *
         * @param x
         * @param y
         * @param width
         * @param height
         * @param data
         */
        virtual void set_pixels(uint16_t x, uint16_t y, uint16_t width,
                                uint16_t height, const uint16_t data);


        /**
         * @brief Sets character in a single color
         * 
         * @param x x-coordinate of the character (x=0 is the leftmost collumn)
         * 
         * @param y y-coordinate of the character (y=0 is the highest row)
         * 
         * @param character The un-extended (0-127) ascii value of the character
         *  
         * 
         * @param pixel_color The color of the pixel
         * 
         */
        virtual void set_character(uint16_t x, uint16_t y, char character, 
                                uint16_t pixel_color);


        /**
         * @brief Draws given characters to the target cursor
         * 
         * @param cursor_target This targets the cursor with which to draw
         * 
         * @param characters Array of characters to draw
         * 
         * @param character_amount number of characters to draw
         */
        virtual void set_character(uint8_t cursor_target, char *characters,
                                uint8_t character_amount);

        /**
         * @brief Override for hwlib::window the class doesn't need to implement
         * a flush if not needed
         *
         */
        virtual void flush() override;
    };
} // namespace r2d2::display