#include <ostream>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE("Display shows the correct character", "[display_module]") {
    // The bus itself doesn't take any constructor arguments
    mock_comm_c mock_bus;

    // hwlib pin_out_store is handy for tests, because it allows
    // us to inspect the current state.
    hwlib::pin_out_store out;

    /*
     * The Display module.
     * The module constructor receives a base_comm_c& and an hwlib::pin_out&.
     * As you can see, using the constructor to receive interfaces on your module really
     * helps with testing and abstraction!
     */
    display::module_c module(mock_bus, out);

    // Create a frame
    auto frame = mock_bus.create_frame<
        frame_type::DISPLAY_8x8_CHARACTER
    >();
    auto frame2 = mock_bus.create_frame<
        frame_type::frame_display_filled_rectangle_s
    >();
    auto frame3 = mock_bus.create_frame<
        frame_type::frame_display_8x8_character_via_cursor_s
    >();

    // Display should display a character
    // "as_frame_type" interprets the frame as the struct that corresponds
    // to the given enumeration value.
    // 
    // E.g. frame_type::ACTIVITY_LED_STATE is matched to frame_activity_led_state_s,
    // so calling as_frame_type with this enumartion value will yield a frame_activity_led_state_s instance.
    // 
    // Please note that this data is not copied, it is a non-owning reference to the frame
    // created earlier. 
    frame.as_frame_type<frame_type::DISPLAY_8x8_CHARACTER>().x = 100;
    frame.as_frame_type<frame_type::DISPLAY_8x8_CHARACTER>().y = 100;
    
    frame.as_frame_type<frame_type::DISPLAY_8x8_CHARACTER>().red = 100;
    frame.as_frame_type<frame_type::DISPLAY_8x8_CHARACTER>().green = 100;
    frame.as_frame_type<frame_type::DISPLAY_8x8_CHARACTER>().blue = 100;

    frame.as_frame_type<frame_type::DISPLAY_8x8_CHARACTER>().characters[243] = [1, 50];

    // Frames for writing a filled rectangle.
    frame2.as_frame_type<frame_type::frame_display_filled_rectangle_s>().x = 100;
    frame2.as_frame_type<frame_type::frame_display_filled_rectangle_s>().y = 100;

    frame2.as_frame_type<frame_type::frame_display_filled_rectangle_s>().width = 100;
    frame2.as_frame_type<frame_type::frame_display_filled_rectangle_s>().height = 100;
    
    frame2.as_frame_type<frame_type::frame_display_filled_rectangle_s>().red = 100;
    frame2.as_frame_type<frame_type::frame_display_filled_rectangle_s>().green = 100;
    frame2.as_frame_type<frame_type::frame_display_filled_rectangle_s>().blue = 100;

    // Frames for writing a character to a cursor position.
    frame3.as_frame_type<frame_type::frame_display_8x8_character_via_cursor_s>().cursor_id = 1;
    frame3.as_frame_type<frame_type::frame_display_8x8_character_via_cursor_s>().characters[247] = 100;
    
    // Actually place the frame on the bus, so the module
    // can see and process it.
    mock_bus.accept_frame(frame);
    mock_bus.accept_frame(frame2);
    mock_bus.accept_frame(frame3);

    // Sanity check
    REQUIRE(out.value == false);

    // Manually call the process function on the module
    // to allow it to process everything.
    module.process();

    REQUIRE(out.value == true);
}