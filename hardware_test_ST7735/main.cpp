#include <comm.hpp>
#include <hwlib.hpp>

#include <display_module.hpp>
#include <display_screen.hpp>
#include <st7735_buffered.hpp>
#include <st7735_unbuffered.hpp>

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
        //128 is used because the screen has a resolution of 160 x 128
        for (unsigned int row=0; row <= 128; row+=10){
            //160 is used because the screen has a resolution of 160 x 128
            for(unsigned int colum=0; colum <=160; colum+=10){
                color_display.set_character(row,colum,c,color_display.color_to_pixel(hwlib::white)); 
            }
        }
        color_display.flush(); 
        hwlib::wait_ms(500); 
    }

    //A white horzontal line will be drawed on the screen and will grow to a screen filling square.
    //160 is used because the screen has a resolution of 160 x 128
    for (unsigned int i=0; i<=160; i++){
        color_display.clear(); 
        //128 is used because the screen has a resolution of 160 x 128
        color_display.set_pixels(0,0,128,i,color_display.color_to_pixel(hwlib::white));
        color_display.flush();
    }

    //A white vertical line will be drawed on the screen and will grow to a screen filling square.
    //128 is used because the screen has a resolution of 160 x 128
    for (unsigned int j=0; j <=128; j++){
        color_display.clear(); 
        //160 is used because the screen has a resolution of 160 x 128
        color_display.set_pixels(0,0,j,160,color_display.color_to_pixel(hwlib::white)); 
        color_display.flush(); 
    }

    // A loop to make the screen red, green, blue and white with a puase of 1000 ms. 
    // The screen has a resolution of 160 by 128 pixels this is why the values 128 and 160 are used. 
    color_display.clear();
    color_display.set_pixels(0,0,128,160,color_display.color_to_pixel(hwlib::red)); 
    color_display.flush(); 
    hwlib::wait_ms(1000); 
    color_display.clear();
    color_display.set_pixels(0,0,128,160,color_display.color_to_pixel(hwlib::green)); 
    color_display.flush(); 
    hwlib::wait_ms(1000);
    color_display.clear();
    color_display.set_pixels(0,0,128,160,color_display.color_to_pixel(hwlib::blue)); 
    color_display.flush(); 
    hwlib::wait_ms(1000);
    color_display.clear();
    color_display.set_pixels(0,0,128,160,color_display.color_to_pixel(hwlib::white)); 
    color_display.flush(); 
    hwlib::wait_ms(1000); 

    //Write 'end' on the screen so user know test is completed. 
    color_display.clear();
    color_display.set_character(50,80,'E',color_display.color_to_pixel(hwlib::white)); 
    color_display.set_character(60,80,'N',color_display.color_to_pixel(hwlib::white));
    color_display.set_character(70,80,'D',color_display.color_to_pixel(hwlib::white));
    color_display.flush();
    //End of test program.
}