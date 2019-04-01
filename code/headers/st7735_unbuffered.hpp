#ifndef R2D2_ST7735_UNBUFFERED_HPP
#define R2D2_ST7735_UNBUFFERED_HPP

#pragma once

#include <hwlib.hpp>
#include <type_traits>

#include "st7735.hpp"

namespace r2d2::display {

    class st7735_unbuffered_c : public st7735_c {
    protected:
        // bus supported for the
        hwlib::spi_bus &bus;

        // pins for the display
        hwlib::pin_out &cs;
        hwlib::pin_out &dc;
        hwlib::pin_out &reset;

        template <typename... Args>
        void write_command(Args &&... args) {
            // set display in command mode
            dc.write(false);

            // convert all commands to a array
            const uint8_t commands[] = {static_cast<uint8_t>(args)...};

            // write all commands on the bus
            auto transaction = bus.transaction(cs);
            transaction.write(sizeof(commands), commands);
        }

        template <typename... Args>
        void write_data(uint8_t data, Args &&... args) {
            // convert all data to a array
            const uint8_t commands[] = {data, static_cast<uint8_t>(args)...};

            // write all data on the bus
            write_data(commands, sizeof(commands));
        }

        void write_data(const uint8_t *data, size_t size);

        void init();

        void set_cursor(uint16_t x_min, uint16_t y_min, uint16_t x_max,
                        uint16_t y_max);

    public:
        /**
         * @brief Construct a new st7735 unbuffered c object
         *
         * @param bus
         * @param cs
         * @param dc
         * @param reset
         */
        st7735_unbuffered_c(hwlib::spi_bus &bus, hwlib::pin_out &cs,
                            hwlib::pin_out &dc, hwlib::pin_out &reset);

        /**
         * @brief Directly write a pixel to the screen
         *
         * @param x
         * @param y
         * @param data
         */
        void set_pixel(uint16_t x, uint16_t y, const uint16_t data) override;

        /**
         * @brief Directly write multiple pixels to the screen
         *
         * @param x
         * @param y
         * @param width
         * @param height
         * @param data
         */
        void set_pixels(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                        const uint16_t *data) override;

        /**
         * @brief Directly fill multiple pixels with the same color to the
         * screen
         *
         * @param x
         * @param y
         * @param width
         * @param height
         * @param data
         */
        void set_pixels(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                        const uint16_t data) override;
    };

} // namespace r2d2::display

#endif