#include <comm.hpp>
#include <hwlib.hpp>

#include <display_module.hpp>
#include <display_screen.hpp>
#include <st7735_buffered.hpp>
#include <st7735_inverted_color_buffered.hpp>
#include <st7735_inverted_color_unbuffered.hpp>
#include <st7735_unbuffered.hpp>
//#include <ssd1306_oled_buffered.hpp>
//#include <ssd1306_oled_unbuffered.hpp>

int main() {
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;

    hwlib::wait_ms(10);

    // hardware spi
    auto spi_bus = due::hwspi(0, due::hwspi::SPI_MODE::SPI_MODE0, 3);
    auto pin_dummy = hwlib::pin_out_dummy;
    auto dc = hwlib::target::pin_out(hwlib::target::pins::d9);
    auto rst = hwlib::target::pin_out(hwlib::target::pins::d8);

    // use pin_dummy because chip select(cs) is controlled by the hardware spi
    r2d2::display::st7735_buffered_c<r2d2::display::st7735_128x160_s>
        color_display(spi_bus, pin_dummy, dc, rst);

    r2d2::comm_c comm;

    r2d2::display::module_c<r2d2::display::st7735_128x160_s> module(
        comm, color_display);

    // Loop to print the ascii characters starting with: !(33) ending with: ~(126). 
    for (char c=33; c <=126; c++){
        color_display.clear(); 
        for (unsigned int row=0; row <= 128; row+=10){
            for(unsigned int colum=0; colum <=160; colum+=10){
                color_display.set_character(row,colum,c,color_display.color_to_pixel(hwlib::white)); 
            }
        }
        color_display.flush(); 
        hwlib::wait_ms(2000); 
    }

    //A white sqaure will be drawed on the screen.
    // Starting small in the upper left corner of the screen and it will expand till the screen is full. 
    for (unsigned int i=0; i<=160; i++){
        color_display.clear(); 
        color_display.set_pixels(0,0,128,i,color_display.color_to_pixel(hwlib::white));
        color_display.flush();
        hwlib::wait_ms(100); 
    }

    // A loop to make the screen red, green, blue and white with a puase of 500 ms. 
    color_display.clear();
    color_display.set_pixels(0,0,128,160,color_display.color_to_pixel(hwlib::red)); 
    color_display.flush(); 
    hwlib::wait_ms(2000); 
    color_display.clear();
    color_display.set_pixels(0,0,128,160,color_display.color_to_pixel(hwlib::green)); 
    color_display.flush(); 
    hwlib::wait_ms(2000);
    color_display.clear();
    color_display.set_pixels(0,0,128,160,color_display.color_to_pixel(hwlib::blue)); 
    color_display.flush(); 
    hwlib::wait_ms(2000);
    color_display.clear();
    color_display.set_pixels(0,0,128,160,color_display.color_to_pixel(hwlib::white)); 
    color_display.flush(); 

    //End of test program.
}