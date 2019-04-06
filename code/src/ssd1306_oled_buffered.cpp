#include "ssd1306_oled_buffered.hpp"

namespace r2d2::display {
    void ssd1306_oled_buffered::write_implementation(hwlib::xy pos,
                                                     hwlib::color col) {
        int a = pos.x + (pos.y / 8) * size.x;
        if (col == hwlib::white) {
            buffer[a + 1] |= (0x01 << (pos.y % 8));
        } else {
            buffer[a + 1] &= ~(0x01 << (pos.y % 8));
        }
    }

    ssd1306_oled_buffered::ssd1306_oled_buffered(r2d2::i2c::i2c_bus_c &bus,
                                                 const uint8_t &address)
        : ssd1306_i2c_c(bus, address),
          window(wsize, hwlib::white, hwlib::black) {
        bus.write(address, ssd1306_initialization,
                  sizeof(ssd1306_initialization) / sizeof(uint8_t));
    }
    void ssd1306_oled_buffered::clear() {
        const uint8_t d = (background == hwlib::white) ? 0xFF : 0x00;
        for (uint_fast16_t x = 1; x < sizeof(buffer); ++x) {
            buffer[x] = d;
        }
        cursor = size;
    }

    void ssd1306_oled_buffered::flush() {
        command(ssd1306_commands::column_addr, 0, 127);
        command(ssd1306_commands::page_addr, 0, 7);
        buffer[0] = ssd1306_data_prefix;
        bus.write(address, buffer, sizeof(buffer));
    }
} // namespace r2d2::display