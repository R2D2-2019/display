#include "ssd1306_oled_unbuffered.hpp"

namespace r2d2::display {
    void ssd1306_oled_unbuffered::write_implementation(hwlib::xy pos,
                                                       hwlib::color col) {

        int a = pos.x + (pos.y / 8) * size.x;
        hwlib::cout << pos.y << "\n";
        if (col == hwlib::white) {
            buffer[a] |= (0x01 << (pos.y % 8));
        } else {
            buffer[a] &= ~(0x01 << (pos.y % 8));
        }

        pixels_byte_write(hwlib::xy(pos.x, pos.y / 8), buffer[a]);
    }

    ssd1306_oled_unbuffered::ssd1306_oled_unbuffered(r2d2::i2c::i2c_bus_c &bus,
                                                     const uint8_t &address)
        : ssd1306_i2c_c(bus, address),
          window(wsize, hwlib::white, hwlib::black) {
        hwlib::cout << "test" << hwlib::endl;
        bus.write(address, ssd1306_initialization,
                  sizeof(ssd1306_initialization) / sizeof(uint8_t));
    }
    void ssd1306_oled_unbuffered::clear() {
        const uint8_t d = (background == hwlib::white) ? 0xFF : 0x00;
        command(ssd1306_commands::column_addr, 0, 127);
        command(ssd1306_commands::page_addr, 0, 7);
        uint8_t data[sizeof(buffer) + 1] = {0};
        data[0] = ssd1306_data_prefix;
        for (uint_fast16_t x = 1; x < sizeof(data); ++x) {
            data[x] = d;
        }
        bus.write(address, data, sizeof(data));
        cursor = wsize;
    }

    void ssd1306_oled_unbuffered::flush() {
    }
} // namespace r2d2::display