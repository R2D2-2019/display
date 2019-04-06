#include <hwlib.hpp>
#include <i2c_bus.hpp>
#include <ssd1306.hpp>

namespace r2d2::display {
    class ssd1306_oled_buffered : public r2d2::display::ssd1306_i2c_c,
                                    public hwlib::window {
    private:
        static auto constexpr wsize = hwlib::xy(128, 64);

        uint8_t buffer[wsize.x * wsize.y / 8] = {0};

        void write_implementation(hwlib::xy pos, hwlib::color col) override;

    public:
        /// construct by providing the i2c channel
        ssd1306_oled_buffered(r2d2::i2c::i2c_bus_c &bus,
                                const uint8_t &address);

        void clear() override;

        void flush() override;
    };

} // namespace r2d2::display