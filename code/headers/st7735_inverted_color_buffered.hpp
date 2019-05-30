#pragma once

#include <hwlib.hpp>
#include <st7735_buffered.hpp>
#include <type_traits>

namespace r2d2::display {

    /**
     * Class st7735_inverted_color_buffered is an interface for the st7735 chip
     * where the display has its colors inverted.
     *
     * Implements hwlib::window to easily use text and drawing functions that
     * are already implemented. Extends from
     * r2d2::display::st7735_inverted_color_c
     *
     * The template paramters are required for the parent class
     */
    template <std::size_t CursorCount, class DisplayScreen>
    class st7735_inverted_color_buffered_c
        : public st7735_buffered_c<CursorCount, DisplayScreen> {
    public:
        /**
         * @brief Construct a new st7735_unbuffered_c object
         *
         * @param bus
         * @param cs
         * @param dc
         * @param reset
         */
        st7735_inverted_color_buffered_c(hwlib::spi_bus &bus,
                                         hwlib::pin_out &cs, hwlib::pin_out &dc,
                                         hwlib::pin_out &reset)
            : st7735_buffered_c<CursorCount, DisplayScreen>(bus, cs, dc, reset) {
            // display inversion on, memory direction control
            this->init();
            this->write_command(st7735_c<CursorCount,DisplayScreen>::INVON, st7735_c<CursorCount,DisplayScreen>::MADCTL);
            this->write_data(0xC8);
            hwlib::wait_ms(20);
        }
    };

} // namespace r2d2::display
