#pragma once

#include <hwlib.hpp>
#include <display_cursor.hpp>
namespace r2d2::display {
    /*
    * Class display_c is the base class for all displays in R2D2. It inherits
    * from hwlib::window.
    * 
    * @tparam Cursor_Count is the amount of cursors to store. Most of the time 
    * this is equal to the number claimed cursors in the display_cursor enum.
    * 
    * @tparam Display_Size_Width is an uint8_t representing the width of the display
    * 
    * @tparam Display_Size_Height is an uint8_t representing the height of the display
    * 
    */
    template<std::size_t Cursor_Count, uint8_t Display_Size_Width, uint8_t Display_Size_Height>
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

        // Keeps track of cursors
        r2d2::display::display_cursor cursors[Cursor_Count];
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
                                uint16_t height, const uint16_t *data) {
            // set the data to the correct pixel
            for (size_t t_y = 0; t_y < width; t_y++) {
                for (size_t t_x = 0; t_x < width; t_x++) {
                    // set the pixel with data at location of t_x + t_y * w
                    set_pixel(x + t_x, y + t_y, data[t_x + (t_y * width)]);
                }
            }
        }

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
                                uint16_t height, const uint16_t data) {
            // set all the pixels to data
            for (size_t t_y = 0; t_y < width; t_y++) {
                for (size_t t_x = 0; t_x < width; t_x++) {
                    // set the pixel with datas
                    set_pixel(x + t_x, y + t_y, data);
                }
            }
        }


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
                                uint16_t pixel_color) {
            // Loops through all rows of the character 
            const hwlib::image& character_image = display_font[character];                               
            for (int image_y = 0; image_y < 8; image_y++) {
                for (int image_x = 0; image_x < 8; image_x++) {
                    // Check if the pixel needs to be drawn. 
                    // If the pixel color is anything other than white, draw it
                    hwlib::color character_pixel_color = character_image[hwlib::xy(image_x, image_y)];
                    if (character_pixel_color != hwlib::white) {
                        set_pixel(x + image_x, y + image_y, pixel_color);
                    }
                }
            }
        }

        /**
         * @brief Draws given characters to the target cursor. For every 
         * character drawn this way, the cursor will move 8 pixels.
         * 
         * @param cursor_target This targets the cursor with which to draw
         * 
         * @param characters Array of characters to draw
         * 
         * @param character_amount number of characters to draw
         */
        virtual void set_character(uint8_t cursor_target, const char *characters,
                                uint8_t character_amount){
            // Checks if the given cursor is not out of bounds
            if (cursor_target >= Cursor_Count) {
                return;
            }
            display_cursor &cursor = cursors[cursor_target];
            for (uint8_t char_i = 0; char_i < character_amount; char_i++) {
                set_character(cursor.cursor_x, cursor.cursor_y, characters[char_i], 
                    color_to_pixel(cursor.cursor_color));
                cursor.cursor_x += 8;
            }
        }

        /**
         * @brief Sets the targeted cursor to the given position
         * 
         * @param cursor_target This targets which cursor to move
         * 
         * @param x new X position of the cursor
         * 
         * @param y new Y position of the cursor
         */
        virtual void set_cursor_positon(uint8_t cursor_target, uint8_t x, 
                                uint8_t y){
            // Checks if the given cursor is not out of bounds
            if (cursor_target >= Cursor_Count) {
                return;
            }
            cursors[cursor_target].cursor_x = x;
            cursors[cursor_target].cursor_y = y;
        }

        /**
         * @brief Sets the targeted cursor to the given color
         * 
         * @param cursor_target This targets which cursor to change colors
         * 
         * @param color The new color
         */
        virtual void set_cursor_color(uint8_t cursor_target, hwlib::color color) {
            // Checks if the given cursor is not out of bounds
            if (cursor_target >= Cursor_Count) {
                return;
            }
            cursors[cursor_target].cursor_color = color;
        };

        /**
         * @brief Override for hwlib::window the class doesn't need to implement
         * a flush if not needed
         *
         */
        virtual void flush() override {}
    };
} // namespace r2d2::display