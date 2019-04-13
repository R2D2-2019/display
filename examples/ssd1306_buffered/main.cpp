#include <hwlib.hpp>
#include <i2c_bus.hpp>
#include <ssd1306_oled_buffered.hpp>

int main(void) {
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    // this starts the sysclock
    hwlib::wait_ms(20);

    // Initialise a I2C bus at full speed (400Kbit/s)
    auto bus = r2d2::i2c::i2c_bus_c(
        r2d2::i2c::i2c_bus_c::interface::interface_0, 400000);
    // Initialise the display using address 0x3C and the bus
    auto display = r2d2::display::ssd1306_oled_buffered_c(bus, 0x3c);
    // Clear the noise of the display
    display.clear();
    // Flush the changes to the buffer
    display.flush();
    // Pixel check: turn each pixel on
    for (uint_fast16_t y = 0; y < 64; y++) {
        for (uint_fast16_t x = 0; x < 128; x++) {
            display.write(hwlib::xy(x, y), hwlib::white);
        }
    }
    // Flush the new buffer to the display at once
    display.flush();
    // Clear the display
    display.clear();
    // Play wouter's random lines demo
    hwlib::graphics_random_lines(display);
}