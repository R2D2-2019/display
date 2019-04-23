#include "ssd1306.hpp"

// ssd1306_i2c_c::
namespace r2d2::display {
    ssd1306_i2c_c::ssd1306_i2c_c(r2d2::i2c::i2c_bus_c &bus,
                                 const uint8_t &address)
        : display_c(wsize, hwlib::white, hwlib::black),
          bus(bus),
          address(address),
          cursor(255, 255) {
    }

    void ssd1306_i2c_c::command(ssd1306_command c) {
        // create command packet
        uint8_t data[] = {ssd1306_cmd_prefix, (uint8_t)c};

        // write command to the bus
        bus.write(address, data, sizeof(data) / sizeof(uint8_t));
    }

    void ssd1306_i2c_c::command(ssd1306_command c, uint8_t d0) {
        // create command packet
        uint8_t data[] = {ssd1306_cmd_prefix, (uint8_t)c, ssd1306_cmd_prefix,
                          d0};

        // write command to the bus
        bus.write(address, data, sizeof(data) / sizeof(uint8_t));
    }

    void ssd1306_i2c_c::command(ssd1306_command c, uint8_t d0, uint8_t d1) {
        // create command packet
        uint8_t data[] = {ssd1306_cmd_prefix, (uint8_t)c,
                          ssd1306_cmd_prefix, d0,
                          ssd1306_cmd_prefix, d1};

        // write command to the bus
        bus.write(address, data, sizeof(data) / sizeof(uint8_t));
    }

    void ssd1306_i2c_c::data(uint8_t d) {
        // create data packet
        uint8_t data[] = {ssd1306_data_prefix, d};

        // write data to the bus
        bus.write(address, data, sizeof(data) / sizeof(uint8_t));
    }
    void ssd1306_i2c_c::pixels_byte_write(hwlib::xy location, uint8_t d) {

        // check if we need to update the current cursor of the screen
        if (location != cursor) {
            command(ssd1306_command::column_addr, location.x, 127);
            command(ssd1306_command::page_addr, location.y, 7);
            cursor = location;
        }

        // write data to the screen
        data(d);

        // update the local cursor
        cursor.x++; // TODO: cursor fallthrough
    }


    uint16_t ssd1306_i2c_c::color_to_pixel(const hwlib::color &c) {
        // return as bool becouse we only need bools for the display
        return (c == hwlib::white);
    }
} // namespace r2d2::display