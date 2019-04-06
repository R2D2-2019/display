#pragma once

#include <base_module.hpp>
#include <hwlib.hpp>

namespace r2d2::display {
    class module_c : public base_module_c {
    protected:
        hwlib::window &display;

    public:
        /**
         * @param comm
         * @param led
         */
        module_c(base_comm_c &comm, hwlib::window &display)
            : base_module_c(comm), display(display) {

            // Set up listeners
            comm.listen_for_frames({
                r2d2::frame_type::DISPLAY_FILLED_RECTANGLE
            });
        }

        /**
         * Let the module process data.
         */
        void process() override {
            while (comm.has_data()) {
                auto frame = comm.get_data();

                // Don't handle requests
                if (frame.request) {
                    continue;
                }

                // Get the data from the frame
                const auto data =
                    frame.as_frame_type<frame_type::DISPLAY_FILLED_RECTANGLE>();

                for (uint8_t y = 0; y < data.height; y++) {
                    for (uint8_t x = 0; x < data.width; x++) {
                        display.write(
                            hwlib::xy(data.x + x, data.y + y),
                            hwlib::color(data.red, data.green, data.blue));
                    }
                }
            }
        }
    };
} // namespace r2d2::display