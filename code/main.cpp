#include <hwlib.hpp>
#include <comm.hpp>

#include "display_module.hpp"

int main() {
    using namespace r2d2;

    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;

    hwlib::wait_ms(10);

    r2d2::comm_c comm;

    // display::module_c module(comm);

    for (;;) {
        // module.process();
    }
}