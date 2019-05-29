#pragma once

#include <display_cursor.hpp>
#include <display_screen.hpp>
#include <hwlib.hpp>

namespace r2d2::display {
    /**
     * Class display_c is the base class for all displays in R2D2. It inherits
     * from hwlib::window.
     *
     * @tparam CursorCount is the amount of cursors to store. Most of the time
     * this is equal to the number claimed cursors in the display_cursor enum.
     *
     * @tparam DisplayScreen is a struct which has contains information of the
     * screen which will be used.
     *
     */
    template <std::size_t CursorCount, class DisplayScreen>
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
        r2d2::display::display_cursor_s cursors[CursorCount];

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
            for (std::size_t t_y = 0; t_y < width; t_y++) {
                for (std::size_t t_x = 0; t_x < width; t_x++) {
                    // set the pixel with data at location of t_x + t_y * width
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
            for (std::size_t t_y = 0; t_y < width; t_y++) {
                for (std::size_t t_x = 0; t_x < width; t_x++) {
                    // set the pixel with datas
                    set_pixel(x + t_x, y + t_y, data);
                }
            }
        }

        /**
         * @brief Sets character in a single color
         *
         * @param x x-coordinate of the character (x=0 is the leftmost collumn)
         * @param y y-coordinate of the character (y=0 is the highest row)
         * @param character The un-extended (0-127) ascii value of the character
         * @param pixel_color The color of the character
         */
        virtual void set_character(uint16_t x, uint16_t y, char character,
                                   uint16_t pixel_color) {
            // Loops through all rows of the character
            const hwlib::image &character_image = display_font[character];
            for (uint16_t image_y = 0; image_y < 8; image_y++) {
                for (uint16_t image_x = 0; image_x < 8; image_x++) {
                    // Check if the pixel needs to be drawn.
                    // If the pixel color is anything other than white, draw it
                    hwlib::color character_pixel_color =
                        character_image[hwlib::xy(image_x, image_y)];
                    if (character_pixel_color != hwlib::white) {
                        set_pixel(x + image_x, y + image_y, pixel_color);
                    }
                }
            }
        }

        /**
         * @brief Sets characters in a single color
         *
         * @param x x-coordinate of the first character
         * @param y y-coordinate of the first character
         * @param characters Array of characters to draw
         * @param pixel_color The color of all characters
         */
        virtual void set_character(uint16_t x, uint16_t y,
                                   const char *character,
                                   uint16_t pixel_color) {
            std::size_t index = 0;
            while (character[index] != '\0') {
                set_character(x, y, character[index], pixel_color);

                // If the cursor is about to go out of bounds, return.
                if (x + 8 < DisplayScreen::width) {
                    x += 8;
                } else {
                    return;
                }
                index++;
            }
        }

        /**
         * @brief Draws given characters to the target cursor. For every
         * character drawn this way, the cursor will move 8 pixels.
         *
         * @param cursor_target This targets the cursor with which to draw
         * @param characters Array of characters to draw
         */
        virtual void set_character(uint8_t cursor_target,
                                   const char *characters) {
            // Checks if the given cursor is not out of bounds
            if (cursor_target >= CursorCount) {
                return;
            }
            display_cursor_s &cursor = cursors[cursor_target];
            std::size_t index = 0;
            while (characters[index] != '\0') {
                set_character(cursor.cursor_x, cursor.cursor_y,
                              characters[index],
                              color_to_pixel(cursor.cursor_color));

                // If the cursor is about to go out of bounds, return.
                if (cursor.cursor_x + 8 < DisplayScreen::width) {
                    set_cursor_positon(cursor_target, cursor.cursor_x + 8,
                                       cursor.cursor_y);
                } else {
                    return;
                }
                index++;
            }
        }

        /**
         * @brief Fill multiple pixels in a circle shape with the same color to
         * the screen
         *
         * @param x x-coordinate of the midpoint of the circle
         * @param y y-coordinate of the midpoint of the circle
         * @param radius the radius of the circle in pixels
         * @param filled a boolean which if true will create a filled circle and
         * if false it will create a hollow circle
         * @param data
         */
        virtual void set_pixels_circle(uint16_t x, uint16_t y, uint16_t radius,
                                       bool filled, const uint16_t data) {
            int t_x = radius;
            int t_y = 0;
            int err = 0;

            if (filled == true) {
                int xChange = 1 - (radius << 1);
                int yChange = 0;

                while (t_x >= t_y) {
                    for (int i = x - t_x; i <= x + t_x; i++) {
                        set_pixel(i, y + t_y, data);
                        set_pixel(i, y - t_y, data);
                    }
                    for (int i = x - t_y; i <= x + t_y; i++) {
                        set_pixel(i, y + t_x, data);
                        set_pixel(i, y - t_x, data);
                    }

                    t_y++;
                    err += yChange;
                    yChange += 2;
                    if (((err << 1) + xChange) > 0) {
                        t_x--;
                        err += xChange;
                        xChange += 2;
                    }
                }
            } else {
                while (t_x >= t_y) {
                    set_pixel(x + t_x, y + t_y, data);
                    set_pixel(x + t_y, y + t_x, data);
                    set_pixel(x - t_y, y + t_x, data);
                    set_pixel(x - t_x, y + t_y, data);
                    set_pixel(x - t_x, y - t_y, data);
                    set_pixel(x - t_y, y - t_x, data);
                    set_pixel(x + t_y, y - t_x, data);
                    set_pixel(x + t_x, y - t_y, data);

                    if (err <= 0) {
                        t_y += 1;
                        err += 2 * t_y + 1;
                    }

                    if (err > 0) {
                        t_x -= 1;
                        err -= 2 * t_x + 1;
                    }
                }
            }
        }

        /**
         * @brief Fill multiple pixels in a circle shape with the same color to
         * the screen using the cursor
         *
         * @param x x-coordinate of the midpoint of the circle
         * @param y y-coordinate of the midpoint of the circle
         * @param radius the radius of the circle in pixels
         * @param filled a boolean which if true will create a filled circle and
         * if false it will create a hollow circle
         */
        virtual void set_pixels_circle(uint8_t cursor_target, uint16_t radius,
                                       bool filled) {
            // Checks if the given cursor is not out of bounds
            if (cursor_target >= CursorCount) {
                return;
            }
            display_cursor_s cursor = cursors[cursor_target];

            int x = cursor.cursor_x;
            int y = cursor.cursor_y;
            int color = color_to_pixel(cursor.cursor_color);

            int t_x = radius;
            int t_y = 0;
            int err = 0;

            if (filled == true) {
                int xChange = 1 - (radius << 1);
                int yChange = 0;

                while (t_x >= t_y) {
                    for (int i = x - t_x; i <= x + t_x; i++) {
                        set_pixel(i, y + t_y, color);
                        set_pixel(i, y - t_y, color);
                    }
                    for (int i = x - t_y; i <= x + t_y; i++) {
                        set_pixel(i, y + t_x, color);
                        set_pixel(i, y - t_x, color);
                    }

                    t_y++;
                    err += yChange;
                    yChange += 2;
                    if (((err << 1) + xChange) > 0) {
                        t_x--;
                        err += xChange;
                        xChange += 2;
                    }
                }
            } else {

                while (t_x >= t_y) {
                    set_pixel(x + t_x, y + t_y, color);
                    set_pixel(x + t_y, y + t_x, color);
                    set_pixel(x - t_y, y + t_x, color);
                    set_pixel(x - t_x, y + t_y, color);
                    set_pixel(x - t_x, y - t_y, color);
                    set_pixel(x - t_y, y - t_x, color);
                    set_pixel(x + t_y, y - t_x, color);
                    set_pixel(x + t_x, y - t_y, color);

                    if (err <= 0) {
                        t_y += 1;
                        err += 2 * t_y + 1;
                    }

                    if (err > 0) {
                        t_x -= 1;
                        err -= 2 * t_x + 1;
                    }
                }
            }
        }

        /**
         * @brief Sets the targeted cursor to the given position. Can't go
         * out of the size of the window. It will stop before that. Any
         * characters written to be out of bounds will be drawn over each
         * other.
         *
         * @param cursor_target This targets which cursor to move
         * @param x new X position of the cursor
         * @param y new Y position of the cursor
         */
        virtual void set_cursor_positon(uint8_t cursor_target, uint8_t x,
                                        uint8_t y) {
            // Checks if the given cursor is not out of bounds
            if (cursor_target >= CursorCount) {
                return;
            }

            // prevents out of bounds width
            if (x < DisplayScreen::width) {
                cursors[cursor_target].cursor_x = x;
            }
            // prevents out of bounds height
            if (y < DisplayScreen::height) {
                cursors[cursor_target].cursor_y = y;
            }
        }

        /**
         * @brief Sets the targeted cursor to the given color
         *
         * @param cursor_target This targets which cursor to change colors
         * @param color The new color
         */
        virtual void set_cursor_color(uint8_t cursor_target,
                                      hwlib::color color) {
            // Checks if the given cursor is not out of bounds
            if (cursor_target >= CursorCount) {
                return;
            }
            cursors[cursor_target].cursor_color = color;
        };

        /**
         * @brief Override for hwlib::window the class doesn't need to
         * implement a flush if not needed
         */
        virtual void flush() override {
        }
    };
} // namespace r2d2::display