#pragma once

#include <hwlib.hpp>
#include <display_adapter.hpp>
#include <st7735.hpp>

namespace r2d2::display {

    template <std::size_t CursorCount, class DisplayScreen>
    class st7735_inverted_color_c
        : public st7735_c<CursorCount, DisplayScreen> {
    protected:
        st7735_inverted_color_c(hwlib::spi_bus &bus, hwlib::pin_out &cs,
                                hwlib::pin_out &dc, hwlib::pin_out &reset)
            : st7735_c<CursorCount, DisplayScreen>(
                  bus, cs, dc, reset) {
            this->init();
            // display inversion on, memory direction control
            this->write_command(this->_INVON, this->_MADCTL);
            this->write_data(0xC8);
            hwlib::wait_ms(20);
        }
    };
} // namespace r2d2::display
