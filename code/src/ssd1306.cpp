#include "ssd1306.hpp"

// ssd1306_i2c_c::
namespace r2d2::display {
    ssd1306_i2c_c::ssd1306_i2c_c(r2d2::i2c::i2c_bus_c &bus,
                                 const uint8_t &address)
        : bus(bus), address(address), cursor(255, 255) {
        // wait for the controller to be ready for the initialization
        hwlib::wait_ms(20);
    }

    void ssd1306_i2c_c::command(ssd1306_commands c) {
        uint8_t data[] = {ssd1306_cmd_prefix, (uint8_t)c};
        bus.write(address, data, sizeof(data) / sizeof(uint8_t));
    }

    void ssd1306_i2c_c::command(ssd1306_commands c, uint8_t d0) {
        uint8_t data[] = {ssd1306_cmd_prefix, (uint8_t)c, ssd1306_cmd_prefix,
                          d0};
        bus.write(address, data, sizeof(data) / sizeof(uint8_t));
    }

    void ssd1306_i2c_c::command(ssd1306_commands c, uint8_t d0, uint8_t d1) {
        uint8_t data[] = {ssd1306_cmd_prefix, (uint8_t)c,
                          ssd1306_cmd_prefix, d0,
                          ssd1306_cmd_prefix, d1};
        bus.write(address, data, sizeof(data) / sizeof(uint8_t));
    }

    void ssd1306_i2c_c::data(uint8_t d) {
        uint8_t data[] = {ssd1306_data_prefix, d};
        bus.write(address, data, sizeof(data) / sizeof(uint8_t));
    }
    void ssd1306_i2c_c::pixels_byte_write(hwlib::xy location, uint8_t d) {

        if (location != cursor) {
            command(ssd1306_commands::column_addr, location.x, 127);
            command(ssd1306_commands::page_addr, location.y, 7);
            cursor = location;
        }

        uint8_t data[] = {ssd1306_data_prefix, d};
        bus.write(address, data, sizeof(data) / sizeof(uint8_t));
        cursor.x++;
    }
} // namespace r2d2::display