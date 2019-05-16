#pragma once

#include <base_module.hpp>
#include <display_adapter.hpp>
#include <hwlib.hpp>

namespace r2d2::display {
    
    template<std::size_t Cursor_Count, uint8_t Display_Size_Width, uint8_t Display_Size_Height>
    class module_c : public base_module_c {
    protected:
        display_c<Cursor_Count, Display_Size_Width, Display_Size_Height> &display;

    public:
        /**
         * @param comm
         * @param led
         */
        module_c(base_comm_c &comm, display_c<Cursor_Count, Display_Size_Width, Display_Size_Height> &display)
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

                // const auto data = frame.as_frame_type<frame.type>();
                switch(frame.type){
                    case r2d2::frame_type::DISPLAY_FILLED_RECTANGLE: {
                        // Get the data from the frame
                        const auto data = frame.as_frame_type<frame_type::DISPLAY_FILLED_RECTANGLE>();

                        display.set_pixels(data.x, data.y, 
                                        data.width, data.height,
                                        display.color_to_pixel(hwlib::color(
                                            data.red, data.green, data.blue)
                                        )
                        );

                        display.flush();
                    } break;

                    case r2d2::frame_type::DISPLAY_8x8_CHARACTER: {
                        const auto data = frame.as_frame_type<frame_type::DISPLAY_8x8_CHARACTER>();
                        display.set_character(data.x, data.y, 
                                            data.character, 
                                            display.color_to_pixel(
                                                hwlib::color(
                                                    data.red, data.green, data.blue)
                                            )
                        );
                        display.flush();
                    } break;

                    case r2d2::frame_type::DISPLAY_8x8_CURSOR_CHARACTER: {
                        const auto data = frame.as_frame_type<frame_type::DISPLAY_8x8_CURSOR_CHARACTER>();
                        display.set_character(data.cursor_id, data.characters, data.amount_characters);
                    } break;

                    case r2d2::frame_type::CURSOR_POSITION: {
                        const auto data = frame.as_frame_type<frame_type::CURSOR_POSITION>();
                        display.set_cursor_positon(data.cursor_id, data.cursor_x, data.cursor_y);
                    } break;

                    case r2d2::frame_type::CURSOR_COLOR: {
                        const auto data = frame.as_frame_type<frame_type::CURSOR_COLOR>();
                        display.set_cursor_color(data.cursor_id, hwlib::color(data.red, data.green, data.blue));
                    } break;

                    default:
                        
                    break;
                }
            }
        }
    };
} // namespace r2d2::display