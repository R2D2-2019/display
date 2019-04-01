#include <hwlib.hpp>
#include "st7735_unbuffered.hpp"

int main(void) {
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    // this starts the sysclock
    hwlib::wait_ms(0);

    auto spi_bus = due::hwspi(0, due::hwspi::SPI_MODE::SPI_MODE0);

    auto pin_dummy = hwlib::pin_out_dummy;
    auto dc = hwlib::target::pin_out(hwlib::target::pins::d9);
    auto rst = hwlib::target::pin_out(hwlib::target::pins::d8);

    r2d2::display::st7735_unbuffered_c display(spi_bus, pin_dummy, dc, rst);

    display.set_pixels(0, 0, 127, 160, uint16_t(0x0000));
    display.set_pixels(0, 0, 127, 160, uint16_t(0xFFFF));

    uint8_t start_x = 26;
    uint8_t start_y = 1;

    for(;;){
        for(uint8_t y = start_y; y < start_y + 160; y++){            
            display.set_pixel(start_x, y, 0xFFE0);
            display.set_pixel(start_x + 79, y, 0xFFE0);
        }
        for(uint8_t x = start_x; x < start_x + 80; x++){
            display.set_pixel(x, start_y, 0x00F8);
            display.set_pixel(x, start_y + 159, 0x00F8);            
        }

        display.set_pixel(start_x + 10, start_y + 10, 0x0000);
    }
}