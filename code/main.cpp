#include <comm.hpp>
#include <hwlib.hpp>

#include <display_module.hpp>
#include <display_screen.hpp>
#include <ssd1306_oled_buffered.hpp>

int main() {
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;

    hwlib::wait_ms(10);

    // Initialise pins
    auto scl = hwlib::target::pin_oc(hwlib::target::pins::scl);
    auto sda = hwlib::target::pin_oc(hwlib::target::pins::sda);
    
    // Initialise i2c_bus
    using i2c_bus = r2d2::i2c::i2c_bus_c;
    i2c_bus bus(i2c_bus::interface::interface_1, 100'000);

    // Initialise display
    r2d2::display::ssd1306_oled_buffered_c<
            static_cast<std::size_t>(r2d2::claimed_display_cursor::CURSORS_COUNT),
            r2d2::display::ssd1306_128x64_s> 
        display(bus, 0x3C);

    r2d2::comm_c comm;

    r2d2::display::module_c<
            static_cast<std::size_t>(r2d2::claimed_display_cursor::CURSORS_COUNT),
            r2d2::display::ssd1306_128x64_s> 
        module(comm, display);
    
    for (;;) {
        module.process();
    }
}