#include "ssd1306_oled_unbuffered.hpp"

namespace r2d2::display {
    ssd1306_oled_unbuffered_c::ssd1306_oled_unbuffered_c(
        r2d2::i2c::i2c_bus_c &bus, const uint8_t &address)
        : display(wsize), ssd1306_i2c_c(bus, address) {

        // write the initalisation sequence to the screen
        bus.write(address, ssd1306_initialization,
                  sizeof(ssd1306_initialization) / sizeof(uint8_t));
    }

    void ssd1306_oled_unbuffered_c::set_pixel(uint16_t x, uint16_t y,
                                              const uint16_t data) {

        // calculate the index of the pixel
        uint16_t t_index = x + (y / 8) * size.x;

        // set or clear the pixel
        if (data) {
            buffer[t_index] |= (0x01 << (y % 8));
        } else {
            buffer[t_index] &= ~(0x01 << (y % 8));
        }

        // write pixel byte to screen
        pixels_byte_write(hwlib::xy(x, y / 8), buffer[t_index]);
    }

    void ssd1306_oled_unbuffered_c::clear() {
        // clear the display
        clear(background);
    }

    void ssd1306_oled_unbuffered_c::clear(hwlib::color col) {
        // get a the data for the screen
        const uint8_t d = (col == hwlib::white) ? 0xFF : 0x00;

        // update cursor of the display
        command(ssd1306_command::column_addr, 0, 127);
        command(ssd1306_command::page_addr, 0, 7);

        // create buffer for the display
        uint8_t data[sizeof(buffer) + 1] = {0};

        // set the command for writing to the screen
        data[0] = ssd1306_data_prefix;

        // set all the data
        for (uint_fast16_t x = 1; x < sizeof(data); ++x) {
            data[x] = d;
        }

        // write all the data to the bus
        bus.write(address, data, sizeof(data));

        // update the cursor
        cursor = hwlib::xy(0, 0);
    }

    uint16_t ssd1306_oled_unbuffered_c::color_to_pixel(const hwlib::color &c) {
        // return as bool becouse we only need bools for the display
        return (c == hwlib::white);
    }
} // namespace r2d2::display