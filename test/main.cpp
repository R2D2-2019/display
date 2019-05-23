#include <base_module.hpp>
#include <mock_bus.hpp>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <display_dummy.hpp>
#include <display_module.hpp>
#include <hwlib.hpp>

TEST_CASE("Default cursor initialization", "[cursor]") {
    // Dummy display does nothing in set_pixel
    r2d2::display::display_dummy_c<
        static_cast<std::size_t>(r2d2::claimed_display_cursor::CURSORS_COUNT),
        128, 160>
        test_display;
    auto curs = test_display.get_cursor(
        static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR));
    REQUIRE(curs.cursor_x == 0);
    REQUIRE(curs.cursor_y == 0);
    REQUIRE(curs.cursor_color == hwlib::color(0, 0, 0));
}

TEST_CASE("Manipulate cursor positon", "[cursor]") {
    // Dummy display does nothing in set_pixel
    r2d2::display::display_dummy_c<
        static_cast<std::size_t>(r2d2::claimed_display_cursor::CURSORS_COUNT),
        128, 160>
        test_display;

    test_display.set_cursor_positon(
        static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR), 50,
        75);
    auto curs = test_display.get_cursor(
        static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR));
    REQUIRE(curs.cursor_x == 50);
    REQUIRE(curs.cursor_y == 75);
}

TEST_CASE("Manipulate cursor position through frame",
          "[cursor, internal_communication]") {
    // The bus itself doesn't take any constructor arguments
    r2d2::mock_comm_c mock_bus;

    // Dummy display does nothing in set_pixel
    r2d2::display::display_dummy_c<
        static_cast<std::size_t>(r2d2::claimed_display_cursor::CURSORS_COUNT),
        128, 160>
        test_display;
    /*
     * The Display module.
     * The module constructor receives a base_comm_c& and an hwlib::pin_out&.
     * As you can see, using the constructor to receive interfaces on your
     * module really helps with testing and abstraction!
     */
    r2d2::display::module_c module(mock_bus, test_display);

    // Create a frame
    auto frame = mock_bus.create_frame<r2d2::frame_type::CURSOR_POSITION>(
        {static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR), 
        50,
        75});

    // Actually place the frame on the bus, so the module
    // can see and process it.
    mock_bus.accept_frame(frame);

    // Manually call the process function on the module
    // to allow it to process everything.
    module.process();

    auto curs = test_display.get_cursor(
        static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR));

    REQUIRE(curs.cursor_x == 50);
    REQUIRE(curs.cursor_y == 75);
}
