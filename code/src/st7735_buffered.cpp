#include "st7735_buffered.hpp"

namespace r2d2::display {

    st7735_buffered_c::st7735_buffered_c(hwlib::spi_bus &bus,
                                             hwlib::pin_out &cs,
                                             hwlib::pin_out &dc,
                                             hwlib::pin_out &reset)
        : st7735_c(bus, cs, dc, reset) 
    {}

    void st7735_buffered_c::set_pixel(uint16_t x, uint16_t y,
                                        const uint16_t data) {

        const uint16_t t = __REV16(data); // make a copy and reverse byte order

        // write pixel data to the buffer
        buffer[x + (y * width)] = t;
    }

    void st7735_buffered_c::set_pixels(uint16_t x, uint16_t y, uint16_t w,
                                         uint16_t h, const uint16_t *data) {

        // unfortunaly the arduino due is little endian otherwise we could put
        // all the data directly to the buffer
        for (size_t ly = 0; ly < h; ly++) {
            for (size_t lx = 0; lx < w; lx++) {
                const uint16_t t = __REV(data[ly * lx]);

                ((uint16_t *)buffer)[(x + lx) + ((y + ly) * width)] = t;
            }
        }
    }

    void st7735_buffered_c::set_pixels(uint16_t x, uint16_t y, uint16_t w,
                                         uint16_t h, const uint16_t data) {
        
        uint16_t t = __REV16(data); // make a copy and reverse byte order

        for(uint16_t i = 0; i < (width * height); i++){
            ((uint16_t *)buffer)[i] = t;
        }
    }

    void st7735_buffered_c::flush(){
        set_cursor(0, 0, width - 1, height - 1);

        // write to ram
        write_command(_RAMWR);

        write_data(buffer, sizeof(buffer));
    }
} // namespace r2d2::display