#include "st7735.hpp"

namespace r2d2::display {
    void st7735_c::write_data(const uint8_t *data, size_t size) {
        // set display in data mode
        dc.write(true);

        auto transaction = bus.transaction(cs);
        transaction.write(size, data);
    }

    void st7735_c::set_cursor(uint16_t x_min, uint16_t y_min, uint16_t x_max,
                              uint16_t y_max) {
        x_min += x_offset; x_max += x_offset;
        y_min += y_offset; y_max += y_offset;

        // set the min and max row
        write_command(_CASET);
        write_data(uint8_t(x_min >> 8), uint8_t(x_min & 0xFF),
                   uint8_t(x_max >> 8), uint8_t(x_max & 0xFF));

        // set the min and max column
        write_command(_RASET);
        write_data(uint8_t(y_min >> 8), uint8_t(y_min & 0xFF),
                   uint8_t(y_max >> 8), uint8_t(y_max & 0xFF));
    }

    void st7735_c::init() {
        // reset the display
        reset.write(true);
        hwlib::wait_ms(5);
        reset.write(false);
        hwlib::wait_ms(5);
        reset.write(true);
        hwlib::wait_ms(5);

        // do a software reset and stop sleep mode, voltage booster on
        write_command(_SWRESET);
        hwlib::wait_ms(150);

        write_command(_SLPOUT);
        hwlib::wait_ms(500);

        // frame rate control normal mode
        write_command(_FRMCTR1);
        write_data(0x01, 0x2C, 0x2D);

        // frame rate control idle mode
        write_command(_FRMCTR2);
        write_data(0x01, 0x2C, 0x2D);

        // frame rate control partial mode
        write_command(_FRMCTR3);
        write_data(0x01, 0x2C, 0x2D, 
                   0x01, 0x2C, 0x2D);

        hwlib::wait_ms(20);

        // display invertion
        write_command(_INVCTR);
        write_data(0x07);

        // power control settings
        write_command(_PWCTR1);
        write_data(0xA2, 0x02, 0x84);
        write_command(_PWCTR2);
        write_data(0xC5);
        write_command(_PWCTR3);
        write_data(0xA0, 0x00);
        write_command(_PWCTR4);
        write_data(0x8A, 0x2A);
        write_command(_PWCTR5);
        write_data(0x8A, 0xEE);

        // vcomh voltage control
        write_command(_VMCTR1);
        write_data(0x0E);

        // display inversion off, memory direction control
        write_command(_INVOFF, _MADCTL);
        write_data(0xC0);
        hwlib::wait_ms(20);

        // set screen in 8 bit bus mode with 16 bit color
        write_command(_COLMOD);
        write_data(0x05);
        hwlib::wait_ms(10);

        // set the cursor to the maximum of the screen
        set_cursor(0x00, 0x00, width - 1, height - 1);

        // set gamma adjustment + polarity
        write_command(_GMCTRP1);
        write_data(0x02, 0x1C, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2D, 0x29, 0x25,
                   0x2B, 0x39, 0x00, 0x01, 0x03, 0x10);

        // set gamma adjustment - polarity
        write_command(_GMCTRN1);
        write_data(0x03, 0x1D, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, 0x2E, 0x2E,
                   0x37, 0x3F, 0x00, 0x00, 0x02, 0x10);

        // normal display on
        write_command(_NORON);
        hwlib::wait_ms(10);

        // screen on
        write_command(_DISPON);
        hwlib::wait_ms(100);
    }
} // namespace r2d2::display