#include "ssd1306_oled_buffered.hpp"

namespace r2d2::display {
    ssd1306_oled_buffered_c::ssd1306_oled_buffered_c(r2d2::i2c::i2c_bus_c &bus,
                                                 const uint8_t &address)
        : ssd1306_i2c_c(bus, address) {
        display(wsize, hwlib::white, hwlib::black) {
        bus.write(address, ssd1306_initialization,
                  sizeof(ssd1306_initialization) / sizeof(uint8_t));
    }

    // void ssd1306_oled_buffered_c::write_implementation(hwlib::xy pos,
    //                                                  hwlib::color col) {
    //     int a = pos.x + (pos.y / 8) * size.x;
    //     if (col == hwlib::white) {
    //         buffer[a + 1] |= (0x01 << (pos.y % 8));
    //     } else {
    //         buffer[a + 1] &= ~(0x01 << (pos.y % 8));
    //     }
    // }

    void ssd1306_oled_buffered_c::clear(hwlib::color col) {
        const uint8_t d = (col == hwlib::white) ? 0xFF : 0x00;
        for (uint_fast16_t x = 1; x < sizeof(buffer); ++x) {
            buffer[x] = d;
        }
    }

    void ssd1306_oled_buffered_c::flush() {
        command(ssd1306_command::column_addr, 0, 127);
        command(ssd1306_command::page_addr, 0, 7);
        buffer[0] = ssd1306_data_prefix;
        bus.write(address, buffer, sizeof(buffer));
    }
} // namespace r2d2::display