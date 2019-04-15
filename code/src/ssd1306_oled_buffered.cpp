#include "ssd1306_oled_buffered.hpp"

namespace r2d2::display {
    ssd1306_oled_buffered_c::ssd1306_oled_buffered_c(r2d2::i2c::i2c_bus_c &bus,
                                                     const uint8_t &address)
        : display_c(wsize, hwlib::white, hwlib::black),
          ssd1306_i2c_c(bus, address) {

        // set the command for writing to the screen
        buffer[0] = ssd1306_data_prefix;

        // write the initalisation sequence to the screen
        bus.write(address, ssd1306_initialization,
                  sizeof(ssd1306_initialization) / sizeof(uint8_t));
    }

    void ssd1306_oled_buffered_c::set_pixel(uint16_t x, uint16_t y,
                                            const uint16_t data) {
        // calculate the index of the pixel
        uint16_t t_index = (x + (y / 8) * size.x) + 1;

        // set or clear the pixel
        if (data) {
            buffer[t_index] |= (0x01 << (y % 8));
        } else {
            buffer[t_index] &= ~(0x01 << (y % 8));
        }
    }    

    void ssd1306_oled_buffered_c::clear() {
        // clear the screen with the background
        clear(background);
    }

    void ssd1306_oled_buffered_c::clear(hwlib::color col) {
        // get a the data for the screen
        const uint8_t d = (col == hwlib::white) ? 0xFF : 0x00;

        // set all values to the color of the data
        for (uint_fast16_t x = 1; x < sizeof(buffer); ++x) {
            buffer[x] = d;
        }
    }

    void ssd1306_oled_buffered_c::flush() {
        // update cursor of the display
        command(ssd1306_command::column_addr, 0, 127);
        command(ssd1306_command::page_addr, 0, 7);

        // write data to the screen
        bus.write(address, buffer, sizeof(buffer));
    }

    uint16_t ssd1306_oled_buffered_c::color_to_pixel(const hwlib::color &c) {
        // return as bool becouse we only need bools for the display
        return (c == hwlib::white);
    }
} // namespace r2d2::display