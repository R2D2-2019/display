#ifndef R2D2_ST7735_HPP
#define R2D2_ST7735_HPP

#pragma once

#include <cstdint>

namespace r2d2::display {

    class st7735_c {
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

        void write_data(const uint8_t *data, size_t size) {
            // set display in data mode
            dc.write(true);

            auto transaction = bus.transaction(cs);
            transaction.write(size, data);
        }      

        template <typename... Args>
        void write_data(uint8_t data, Args &&... args) {
            // convert all data to a array
            const uint8_t commands[] = {data, static_cast<uint8_t>(args)...};

            // write all data on the bus
            write_data(commands, sizeof(commands));
        } 

        st7735_c(hwlib::spi_bus &bus, hwlib::pin_out &cs,
                 hwlib::pin_out &dc, hwlib::pin_out &reset)
            :bus(bus), cs(cs), dc(dc), reset(reset)
        {}

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
         * @brief Directly write a pixel to the screen
         *
         * @param x
         * @param y
         * @param data
         */
        virtual void set_pixel(uint16_t x, uint16_t y,
                               const uint16_t data) = 0;

        /**
         * @brief Directly write multiple pixels to the screen
         *
         * @param x
         * @param y
         * @param width
         * @param height
         * @param data
         */
        virtual void set_pixels(uint16_t x, uint16_t y, uint16_t width,
                                uint16_t height, const uint16_t *data) = 0;

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
        virtual void set_pixels(uint16_t x, uint16_t y, uint16_t width,
                                uint16_t height, const uint16_t data) = 0;
    };

} // namespace r2d2::display

#endif