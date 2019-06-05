#pragma once

#include <hwlib.hpp>
#include <st7735_unbuffered.hpp>
#include <type_traits>

namespace r2d2::display {

    /**
     * Class st7735_inverted_color_unbuffered is an interface for the st7735
     * chip where the display has its colors inverted.
     *
     * Implements hwlib::window to easily use text and drawing functions that
     * are already implemented. Extends from
     * r2d2::display::st7735_inverted_color_c
     *
     * The template paramters are required for the parent class
     */
    template <class DisplayScreen>
    class st7735_inverted_color_unbuffered_c
        : public st7735_unbuffered_c<DisplayScreen> {
    public:
        /**
         * @brief Construct a new st7735_unbuffered_c object
         *
         * @param bus
         * @param cs
         * @param dc
         * @param reset
         */
        st7735_inverted_color_unbuffered_c(hwlib::spi_bus &bus,
                                           hwlib::pin_out &cs,
                                           hwlib::pin_out &dc,
                                           hwlib::pin_out &reset)
            : st7735_unbuffered_c<DisplayScreen>(bus, cs, dc, reset) {
            // display inversion on, memory direction control
            this->init();
            this->write_command(this->INVON, this->MADCTL);
            this->write_data(0xC8);
            hwlib::wait_ms(20);
        }
    };

} // namespace r2d2::display
