#include <hwlib.hpp>
#include <i2c_bus.hpp>
#include <ssd1306_oled_unbuffered.hpp>
int main(void) {
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    // this starts the sysclock
    hwlib::wait_ms(20);

    auto bus = r2d2::i2c::i2c_bus_c(
        r2d2::i2c::i2c_bus_c::interface::interface_0, 50000);
    auto display = r2d2::display::ssd1306_oled_unbuffered(bus, 0x3c);
    display.clear();
    for (uint_fast16_t y = 0; y < 64; y++) {
        for (uint_fast16_t x = 0; x < 128; x++) {
            display.write(hwlib::xy(x, y), hwlib::white);
            hwlib::wait_ms_busy(50);
        }
    }
}