#ifndef R2D2_ST7735_HPP
#define R2D2_ST7735_HPP

#pragma once

#include <hwlib.hpp>
#include <display_adapter.hpp>

namespace r2d2::display {

    class st7735_c : public display_c {
    protected:
        // all the commands for the display
        constexpr static uint8_t _NOP = 0x00;
        constexpr static uint8_t _SWRESET = 0x01;
        constexpr static uint8_t _RDDID = 0x04;
        constexpr static uint8_t _RDDST = 0x09;
        constexpr static uint8_t _SLPIN = 0x10;
        constexpr static uint8_t _SLPOUT = 0x11;
        constexpr static uint8_t _PTLON = 0x12;
        constexpr static uint8_t _NORON = 0x13;
        constexpr static uint8_t _INVOFF = 0x20;
        constexpr static uint8_t _INVON = 0x21;
        constexpr static uint8_t _DISPOFF = 0x28;
        constexpr static uint8_t _DISPON = 0x29;
        constexpr static uint8_t _CASET = 0x2A;
        constexpr static uint8_t _RASET = 0x2B;
        constexpr static uint8_t _RAMWR = 0x2C;
        constexpr static uint8_t _RAMRD = 0x2E;
        constexpr static uint8_t _PTLAR = 0x30;
        constexpr static uint8_t _COLMOD = 0x3A;
        constexpr static uint8_t _MADCTL = 0x36;
        constexpr static uint8_t _FRMCTR1 = 0xB1;
        constexpr static uint8_t _FRMCTR2 = 0xB2;
        constexpr static uint8_t _FRMCTR3 = 0xB3;
        constexpr static uint8_t _INVCTR = 0xB4;
        constexpr static uint8_t _DISSET5 = 0xB6;
        constexpr static uint8_t _PWCTR1 = 0xC0;
        constexpr static uint8_t _PWCTR2 = 0xC1;
        constexpr static uint8_t _PWCTR3 = 0xC2;
        constexpr static uint8_t _PWCTR4 = 0xC3;
        constexpr static uint8_t _PWCTR5 = 0xC4;
        constexpr static uint8_t _VMCTR1 = 0xC5;
        constexpr static uint8_t _RDID1 = 0xDA;
        constexpr static uint8_t _RDID2 = 0xDB;
        constexpr static uint8_t _RDID3 = 0xDC;
        constexpr static uint8_t _RDID4 = 0xDD;
        constexpr static uint8_t _PWCTR6 = 0xFC;
        constexpr static uint8_t _GMCTRP1 = 0xE0;
        constexpr static uint8_t _GMCTRN1 = 0xE1;

        // since it uses a generic driver the display has a offset
        constexpr static uint8_t x_offset = 26;
        constexpr static uint8_t y_offset = 1;

        // display bus
        hwlib::spi_bus &bus;

        // pins for the display
        hwlib::pin_out &cs;
        hwlib::pin_out &dc;
        hwlib::pin_out &reset;

        /**
         * @brief Write a command to the screen
         *
         * @tparam Args
         * @param args
         */
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

        /**
         * @brief Write display data/command data to the screen
         *
         * @param data
         * @param size
         */
        void write_data(const uint8_t *data, size_t size);

        /**
         * @brief Write display data/command data to the screen
         *
         * @tparam Args
         * @param data
         * @param args
         */
        template <typename... Args>
        void write_data(uint8_t data, Args &&... args) {
            // convert all data to a array
            const uint8_t commands[] = {data, static_cast<uint8_t>(args)...};

            // write all data on the bus
            write_data(commands, sizeof(commands));
        }

        /**
         * @brief inits the display
         *
         */
        void init();

        /**
         * @brief Set the cursor object
         *
         * @param x_min
         * @param y_min
         * @param x_max
         * @param y_max
         */
        void set_cursor(uint16_t x_min, uint16_t y_min, uint16_t x_max,
                        uint16_t y_max);

        /**
         * @brief Construct a new st7735_c object
         *
         * @param bus
         * @param cs
         * @param dc
         * @param reset
         */
        st7735_c(hwlib::spi_bus &bus, hwlib::pin_out &cs, hwlib::pin_out &dc,
                 hwlib::pin_out &reset)
            :display_c(hwlib::xy(width, height)) , bus(bus), cs(cs), dc(dc), reset(reset) {
            init();
        }

    public:
        /**
         * @brief width of display
         *
         */
        constexpr static uint8_t width = 80;

        /**
         * @brief height of display
         *
         */
        constexpr static uint8_t height = 160;

        /**
         * @brief Converst a hwlib::color to a uint16_t in the format the screen
         * wants
         *
         * @param c
         * @return constexpr uint16_t
         */
        uint16_t color_to_pixel(const hwlib::color &c) override {
            return (uint16_t(c.red) * 0x1F / 0xFF) << 11 |
                   (uint16_t(c.green) * 0x3F / 0xFF) << 5 |
                   (uint16_t(c.blue) * 0x1F / 0xFF);
        }
    };
} // namespace r2d2::display

#endif