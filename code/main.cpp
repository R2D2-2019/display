#include <hwlib.hpp>
#include <comm.hpp>

#include <i2c_bus.hpp>
#include <ssd1306_oled_buffered.hpp>

#include "display_module.hpp"

int main() {
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;

    hwlib::wait_ms(10);

    // Initialise a I2C bus at full speed (400Kbit/s)
    auto bus = r2d2::i2c::i2c_bus_c(
        r2d2::i2c::i2c_bus_c::interface::interface_0, 400000);

    // Initialise the display using address 0x3C and the bus
    auto display = r2d2::display::ssd1306_oled_buffered_c(bus, 0x3c);

    r2d2::comm_c comm;

    r2d2::display::module_c module(comm, display);

    for (;;) {
        module.process();
    }
}