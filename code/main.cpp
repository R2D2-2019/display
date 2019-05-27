#include <comm.hpp>
#include <hwlib.hpp>

#include "display_module.hpp"
#include <display_screen.hpp>
#include <i2c_bus.hpp>
#include <ssd1306_oled_unbuffered.hpp>
#include <ssd1306_oled_buffered.hpp>
#include <st7735_inverted_color_unbuffered.hpp>
#include <st7735_unbuffered.hpp>

int main() {
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;

    hwlib::wait_ms(10);
    /*=====================================================================
    // hardware spi
    auto spi_bus = due::hwspi(0, due::hwspi::SPI_MODE::SPI_MODE0, 3);
    auto pin_dummy = hwlib::pin_out_dummy;

    auto dc = hwlib::target::pin_out(hwlib::target::pins::d9);
    auto rst = hwlib::target::pin_out(hwlib::target::pins::d8);

    r2d2::display::st7735_inverted_color_unbuffered_c<
        static_cast<std::size_t>(r2d2::claimed_display_cursor::CURSORS_COUNT),
        r2d2::display::st7735_128x160_s
        > color_display(spi_bus, pin_dummy, dc, rst);

    r2d2::comm_c comm;

    r2d2::display::module_c<
            static_cast<std::size_t>(r2d2::claimed_display_cursor::CURSORS_COUNT),
            r2d2::display::st7735_128x160_s
        > module(comm, color_display);
    ==========================================================================*/

    auto scl = hwlib::target::pin_oc(hwlib::target::pins::scl1);
    auto sda = hwlib::target::pin_oc(hwlib::target::pins::sda1);
    using i2c_bus = r2d2::i2c::i2c_bus_c;

    // create the bus we want to use
    i2c_bus bus(i2c_bus::interface::interface_1, 100'000);

    // use pin_dummy becouse chip select(cs) is controlled by the hardware spi
    r2d2::display::ssd1306_oled_buffered_c<
            static_cast<std::size_t>(r2d2::claimed_display_cursor::CURSORS_COUNT),
            r2d2::display::ssd1306_128x64_s> 
        oled_display(i2c_bus, uint8_t addr = 0x3C);

    r2d2::comm_c comm;

    r2d2::display::module_c<
            static_cast<std::size_t>(r2d2::claimed_display_cursor::CURSORS_COUNT),
            r2d2::display::ssd1306_128x64_s> 
        module(comm, oled_display);
    
    
    oled_display.clear();

    for (;;) {
        module.process();
    }
}