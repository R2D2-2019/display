#include "st7735_unbuffered.hpp"

namespace r2d2::display {

    st7735_unbuffered_c::st7735_unbuffered_c(hwlib::spi_bus &bus,
                                             hwlib::pin_out &cs,
                                             hwlib::pin_out &dc,
                                             hwlib::pin_out &reset)
        : st7735_c(bus, cs, dc, reset) 
    {}

    void st7735_unbuffered_c::set_pixel(uint16_t x, uint16_t y,
                                        const uint16_t data) {
        // set cursor on correct position
        set_cursor(x, y, x, y);

        // write to ram
        write_command(_RAMWR);

        const uint16_t t = __REV16(data); // make a copy and reverse byte order

        // write pixel data to the screeen
        write_data((uint8_t *)&t, 2);
    }

    void st7735_unbuffered_c::set_pixels(uint16_t x, uint16_t y, uint16_t w,
                                         uint16_t h, const uint16_t *data) {
        // set the window to the size we want to write to
        set_cursor(x, y, x + w, y + h);

        // write to ram
        write_command(_RAMWR);

        // unfortunaly the arduino due is little endian otherwise we could put
        // all the data directly to the write_data function
        for (size_t i = 0; i < w * h; i++) {
            const uint16_t t = __REV16(data[i]);

            write_data((uint8_t *)&t, 2);
        }
    }

    void st7735_unbuffered_c::set_pixels(uint16_t x, uint16_t y, uint16_t w,
                                         uint16_t h, const uint16_t data) {
        set_cursor(x, y, x + w, y + h);

        // write to ram
        write_command(_RAMWR);

        const uint16_t t = __REV16(data); // make a copy and reverse byte order

        for (size_t i = 0; i < size_t((w + 1) * (h + 1)); i++) {
            write_data((uint8_t *)&t, 2);
        }
    }
} // namespace r2d2::display