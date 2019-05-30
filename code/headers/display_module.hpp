#pragma once
// DO NOT RUN CLANG FORMAT ON THIS FILE 

#include <base_module.hpp>
#include <display_adapter.hpp>
#include <hwlib.hpp>

namespace r2d2::display {
    template <std::size_t CursorCount, class DisplayScreen>
    class module_c : public base_module_c {
    protected:
        display_c<CursorCount, DisplayScreen> &display;

    public:
        /**
         * @param comm
         * @param led
         */
        module_c(base_comm_c &comm,
                 display_c<CursorCount, DisplayScreen> &display)
            : base_module_c(comm), display(display) {

            // Set up listeners
            comm.listen_for_frames(
                {r2d2::frame_type::DISPLAY_RECTANGLE,
                 r2d2::frame_type::DISPLAY_8X8_CHARACTER,
                 r2d2::frame_type::DISPLAY_8X8_CHARACTER_VIA_CURSOR,
                 r2d2::frame_type::DISPLAY_CIRCLE,
                 r2d2::frame_type::DISPLAY_CIRCLE_VIA_CURSOR,
                 r2d2::frame_type::CURSOR_POSITION,
                 r2d2::frame_type::CURSOR_COLOR});
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

                switch (frame.type) {
                    case r2d2::frame_type::DISPLAY_RECTANGLE: {
                        // Get the data from the frame
                        const auto data = frame.as_frame_type<
                            frame_type::DISPLAY_RECTANGLE
                        >();

                        display.set_pixels(data.x, data.y, data.width, data.height,
                            display.color_to_pixel(
                                hwlib::color(data.red, data.green, data.blue)
                            )
                        );

                        display.flush();

                    } break;

                    case r2d2::frame_type::DISPLAY_8X8_CHARACTER: {
                        const auto data = frame.as_frame_type<
                            frame_type::DISPLAY_8X8_CHARACTER
                        >();

                        display.set_character(data.x, data.y, data.characters, 
                            display.color_to_pixel(
                                hwlib::color(data.red, data.green, data.blue)
                            )
                        );

                        display.flush();

                    } break;

                    case r2d2::frame_type::DISPLAY_8X8_CHARACTER_VIA_CURSOR: {
                        const auto data = frame.as_frame_type<
                            frame_type::DISPLAY_8X8_CHARACTER_VIA_CURSOR
                        >();

                        display.set_character(data.cursor_id, data.characters);

                    } break;

                    case r2d2::frame_type::DISPLAY_CIRCLE: {
                        // Get the data from the frame
                        const auto data = frame.as_frame_type<
                            frame_type::DISPLAY_CIRCLE
                        >();

                        display.set_pixels_circle(
                            data.x, data.y, data.radius, data.filled,
                            display.color_to_pixel(
                                hwlib::color(data.red, data.green, data.blue)
                                )
                        );

                        display.flush();

                    } break;

                    case r2d2::frame_type::DISPLAY_CIRCLE_VIA_CURSOR: {
                        // Get the data from the frame
                        const auto data = frame.as_frame_type<
                            frame_type::DISPLAY_CIRCLE_VIA_CURSOR
                        >();

                        display.set_pixels_circle(data.cursor_id, data.radius,
                            data.filled
                        );

                        display.flush();

                    } break;

                    case r2d2::frame_type::CURSOR_POSITION: {
                        const auto data = frame.as_frame_type<
                            frame_type::CURSOR_POSITION
                        >();

                        display.set_cursor_positon(data.cursor_id, data.cursor_x,
                            data.cursor_y
                        );

                    } break;

                    case r2d2::frame_type::CURSOR_COLOR: {
                        const auto data = frame.as_frame_type<
                            frame_type::CURSOR_COLOR
                        >();

                        display.set_cursor_color(data.cursor_id,
                            hwlib::color(data.red, data.green, data.blue)
                        );

                    } break;

                    default:{
                    } break;
                }
            }
        }
    };
} // namespace r2d2::display