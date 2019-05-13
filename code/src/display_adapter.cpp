#include <display_adapter.hpp>


namespace r2d2::display {
    void display_c::set_pixels(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                             const uint16_t *data) {
        // set the data to the correct pixel
        for (size_t t_y = 0; t_y < w; t_y++) {
            for (size_t t_x = 0; t_x < w; t_x++) {
                // set the pixel with data at location of t_x + t_y * w
                set_pixel(x + t_x, y + t_y, data[t_x + (t_y * w)]);
            }
        }
    }

    void display_c::set_pixels(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                             const uint16_t data) {
        // set all the pixels to data
        for (size_t t_y = 0; t_y < w; t_y++) {
            for (size_t t_x = 0; t_x < w; t_x++) {
                // set the pixel with datas
                set_pixel(x + t_x, y + t_y, data);
            }
        }
    }

    
    void display_c::set_character(uint16_t x, uint16_t y, 
                                    char character, uint16_t pixel_color) {
        // Loops through all rows of the character 
        const hwlib::image& character_image = display_font[character];                               
        for(int image_y = 0; image_y < 8; image_y++) {
            for(int image_x = 0; image_x < 8; image_x++) {
                hwlib::color character_pixel_color = character_image[hwlib::xy(image_x, image_y)];
                if(character_pixel_color != hwlib::white) {
                    set_pixel(x + image_x, y + image_y, color_to_pixel(pixel_color));

                }
            }
        }
    }

    void display_c::flush() {
    }
} // namespace r2d2::display