#include <st7735_buffered.hpp>
#include <hwlib.hpp>

int main(void) {
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    // this starts the sysclock
    hwlib::wait_ms(0);

    // hardware spi
    auto spi_bus = due::hwspi(0, due::hwspi::SPI_MODE::SPI_MODE0, 3);
    auto pin_dummy = hwlib::pin_out_dummy;

    // software spi
    // auto miso = hwlib::pin_in_dummy; // dummy pin becouse we dont need input
    // auto mosi = hwlib::target::pin_out(hwlib::target::pins::d12);
    // auto clk = hwlib::target::pin_out(hwlib::target::pins::d11);
    // auto spi_bus = hwlib::spi_bus_bit_banged_sclk_mosi_miso(clk, mosi, miso);
    // auto pin_dummy = hwlib::target::pin_out(hwlib::target::pins::d10);

    auto dc = hwlib::target::pin_out(hwlib::target::pins::d9);
    auto rst = hwlib::target::pin_out(hwlib::target::pins::d8);

    // use pin_dummy becouse chip select(cs) is controlled by the hardware spi
    r2d2::display::st7735_buffered_c display(spi_bus, pin_dummy, dc, rst);

    display.set_pixels(0, 0, 80, 160, uint16_t(0xFFFF)); // white
    display.flush();
    display.set_pixels(0, 0, 80, 160, uint16_t(0x0000)); // black
    display.flush();

    // draw boarders on the edges of the screen
    for (uint8_t y = 0; y < 160; y++) {
        display.set_pixel(0, y,
                          display.color_to_pixel(hwlib::color(0, 0xFF, 0)));
        display.set_pixel(0 + 79, y,
                          display.color_to_pixel(hwlib::color(0, 0xFF, 0)));
    }
    for (uint8_t x = 0; x < 80; x++) {
        display.set_pixel(x, 0,
                          display.color_to_pixel(hwlib::color(0xFF, 0xFF, 0)));
        display.set_pixel(x, 159,
                          display.color_to_pixel(hwlib::color(0xFF, 0xFF, 0)));
    }

    display.flush();

    // endless loop
    for (;;) {
        display.set_pixel(10, 10,
                          display.color_to_pixel(hwlib::color(255, 255, 255)));
        display.flush();
    }
}