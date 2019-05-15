#pragma once

#include <base_module.hpp>
#include <display_adapter.hpp>
#include <hwlib.hpp>

namespace r2d2::display {
    class module_c : public base_module_c {
    protected:
        display_c &display;

    public:
        /**
         * @param comm
         * @param led
         */
        module_c(base_comm_c &comm, display_c &display)
            : base_module_c(comm), display(display) {

            // Set up listeners
            comm.listen_for_frames(
                {r2d2::frame_type::DISPLAY_FILLED_RECTANGLE});
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

                if (frame.type == r2d2::frame_type::DISPLAY_FILLED_RECTANGLE) {
                    // Get the data from the frame
                    const auto data =
                        frame.as_frame_type<frame_type::DISPLAY_FILLED_RECTANGLE>();

                    display.set_pixels(data.x, data.y, data.width, data.height,
                                        display.color_to_pixel(hwlib::color(
                                            data.red, data.green, data.blue)));

                    display.flush();
                } else if (frame.type == r2d2::frame_type::DISPLAY_CHARACTER) {
                    const auto data = frame.as_frame_type<frame_type::DISPLAY_CHARACTER>();
                    display.set_character(data.x, data.y, data.character, display.color_to_pixel(hwlib::color(data.red, data.green, data.blue)));
                    display.flush();
                }
            }
        }
    };
} // namespace r2d2::display