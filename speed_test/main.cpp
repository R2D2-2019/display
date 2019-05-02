#include <hwlib.hpp>
#include <i2c_bus.hpp>

// Include the display required for testing
#include <st7735_buffered.hpp>
// defining your display type
typedef r2d2::display::st7735_buffered_c display_type;



void display_rectangles_test(display_type&);
void display_circles_test(display_type&);
void fill_screen_test(display_type&);


int main() {
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);
    // Initialising the display here



    // Passing the display into the functions

    // display_rectangles_test(display);
    // display_circles_test(display);
    // fill_screen_test(display);
    return 0;
}


// Draws 4 white rectangles as follows:
// 40x12 starting at 1,1
// 3x3 starting at 45, 1
// 20x30 starting at 20, 30
// 50x2 starting at 1, 30
void display_rectangles_test(display_type& test_display){
    uint64_t start_time = hwlib::now_us();
    // Rectangle 1
    test_display.set_pixels(1, 1, 40, 1, uint16_t(0xFFFF)); //upper line
    test_display.set_pixels(41, 1, 1, 12, uint16_t(0xFFFF)); //right line
    test_display.set_pixels(1, 1, 1, 12, uint16_t(0xFFFF)); //left line
    test_display.set_pixels(1, 13, 40, 1, uint16_t(0xFFFF)); //bottom line
    test_display.flush();

    // Rectangle 2
    test_display.set_pixels(45, 1, 3, 1, uint16_t(0xFFFF)); //upper line
    test_display.set_pixels(48, 1, 1, 3, uint16_t(0xFFFF)); //left line
    test_display.set_pixels(45, 1, 1, 3, uint16_t(0xFFFF)); //right line
    test_display.set_pixels(45, 4, 3, 1, uint16_t(0xFFFF)); //bottom line
    test_display.flush();
    
    // Rectangle 3
    test_display.set_pixels(20, 30, 20, 1, uint16_t(0xFFFF)); //upper line
    test_display.set_pixels(40, 30, 1, 30, uint16_t(0xFFFF)); //right line
    test_display.set_pixels(20, 30, 1, 30, uint16_t(0xFFFF)); //left line
    test_display.set_pixels(20, 60, 20, 1, uint16_t(0xFFFF)); //bottom line
    test_display.flush();

    // Rectangle 4
    test_display.set_pixels(1, 30, 50, 1, uint16_t(0xFFFF)); //upper line
    test_display.set_pixels(51, 30, 1, 2, uint16_t(0xFFFF)); //right line
    test_display.set_pixels(1, 30, 1, 2, uint16_t(0xFFFF)); //left line
    test_display.set_pixels(1, 32, 50, 1, uint16_t(0xFFFF)); //bottom line
    test_display.flush();
    uint64_t stop_time = hwlib::now_us();
    hwlib::cout << "Display has drawn all rectangles in "  << (stop_time - start_time) << " us.\n";
}


void display_circles_test(display_type& test_display){
    //Todo
}

void fill_screen_test(display_type& test_display){
    uint64_t start_time_fill = hwlib::now_us();
    for(int_fast16_t y = 0; y < test_display.size.y; y++){
        for(int_fast16_t x = 0; x < test_display.size.x; x++){
            test_display.set_pixel(x, y, uint16_t(0xFFFF));
        }
    }
    test_display.flush();
    uint64_t stop_time_fill = hwlib::now_us();
    hwlib::cout << "Filling the screen white took " << (stop_time_fill - start_time_fill) << " us.\n";
    
    uint64_t start_time_clear = hwlib::now_us();
    for(int_fast16_t y = 0; y < test_display.size.y; y++){
        for(int_fast16_t x = 0; x < test_display.size.x; x++){
            test_display.set_pixel(x, y, uint16_t(0x0000));
        }
    }
    test_display.flush();
    uint64_t stop_time_clear = hwlib::now_us();
    hwlib::cout << "Clearing the screen took " << (stop_time_clear - start_time_clear) << " us.\n";


}