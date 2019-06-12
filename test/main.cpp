#include <base_module.hpp>
#include <mock_bus.hpp>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <display_dummy.hpp>
#include <display_module.hpp>
#include <hwlib.hpp>

/*
 * Tests the default initialization of the cursors.
 * The cursor used is the open_cursor
 */
TEST_CASE("Default cursor initialization", "[cursor]") {
    // Dummy display does nothing in set_pixel
    // The chosen screen defines the dimensions it has
    r2d2::display::display_dummy_c<
        
        r2d2::display::st7735_128x160_s>
        test_display;
    auto curs = test_display.get_cursor(
        static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR));
    REQUIRE(curs.cursor_x == 0);
    REQUIRE(curs.cursor_y == 0);
    REQUIRE(curs.cursor_color == hwlib::color(0, 0, 0));
}

/*
 * Testing the cursor position manipulation using the set_cursor_position frame
 * The cursor used is the open_cursor
 *
 * The test will move the cursor to 50,75 and then to 125,30
 */
TEST_CASE("Manipulate cursor position", "[cursor]") {
    // Dummy display does nothing in set_pixel
    r2d2::display::display_dummy_c<
        r2d2::display::st7735_128x160_s>
        test_display;

    SECTION("To 50,75") {
        test_display.set_cursor_position(
            static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR), 50,
            75);

        auto cursor_first = test_display.get_cursor(
            static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR));
        REQUIRE(cursor_first.cursor_x == 50);
        REQUIRE(cursor_first.cursor_y == 75);
    }

    SECTION("To 125,30") {
        test_display.set_cursor_position(
            static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR),
            125, 30);
        auto cursor_second = test_display.get_cursor(
            static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR));
        REQUIRE(cursor_second.cursor_x == 125);
        REQUIRE(cursor_second.cursor_y == 30);
    }
}

TEST_CASE("Manipulate cursor position out of bounds", 
          "[cursor, position, internal_communication]") {

    // The bus itself doesn't take any constructor arguments
    r2d2::mock_comm_c mock_bus;
    // Dummy display does nothing in set_pixel
    r2d2::display::display_dummy_c<r2d2::display::st7735_128x160_s> test_display;

    r2d2::display::module_c module(mock_bus, test_display);

    SECTION("X out of bounds"){
        constexpr uint8_t start_x = 50;
        constexpr uint8_t start_y = 75;
        // Send the cursor to 50,75
        auto frame_start_pos = mock_bus.create_frame<r2d2::frame_type::CURSOR_POSITION>(
        {static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR), start_x, start_y});

        // Actually place the frame on the bus, so the module
        // can see and process it.
        mock_bus.accept_frame(frame_start_pos);

        module.process();

        //send cursor to 130,50
        //130 being out of bounds 50 is within bounds.
        constexpr uint8_t new_x = 130;
        constexpr uint8_t new_y = 50;

        auto frame_pos = mock_bus.create_frame<r2d2::frame_type::CURSOR_POSITION>(
        {static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR), new_x, new_y});

        mock_bus.accept_frame(frame_pos);

        module.process();

        auto cursor_post = test_display.get_cursor(static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR));
        REQUIRE(cursor_post.cursor_x == start_x);
        REQUIRE(cursor_post.cursor_y == new_y);
    }

    SECTION("Y out of bounds"){
        constexpr uint8_t start_x = 50;
        constexpr uint8_t start_y = 75;
        // Send the cursor to 50,75
        auto frame_start_pos = mock_bus.create_frame<r2d2::frame_type::CURSOR_POSITION>(
        {static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR), start_x, start_y});

        // Actually place the frame on the bus, so the module
        // can see and process it.
        mock_bus.accept_frame(frame_start_pos);

        module.process();

        //send cursor to 50,180
        //180 being out of bounds 50 is within bounds.
        constexpr uint8_t new_x = 50;
        constexpr uint8_t new_y = 180;

        auto frame_pos = mock_bus.create_frame<r2d2::frame_type::CURSOR_POSITION>(
        {static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR), new_x, new_y});

        mock_bus.accept_frame(frame_pos);

        module.process();

        auto cursor_post = test_display.get_cursor(static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR));
        REQUIRE(cursor_post.cursor_x == new_x);
        REQUIRE(cursor_post.cursor_y == start_y);
    }

        SECTION("X and Y out of bounds"){
        constexpr uint8_t start_x = 50;
        constexpr uint8_t start_y = 75;
        // Send the cursor to 50,75
        auto frame_start_pos = mock_bus.create_frame<r2d2::frame_type::CURSOR_POSITION>(
        {static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR), start_x, start_y});

        // Actually place the frame on the bus, so the module
        // can see and process it.
        mock_bus.accept_frame(frame_start_pos);

        module.process();

        //send cursor to 130,180
        //130 and 180 being out of bounds.
        constexpr uint8_t new_x = 130;
        constexpr uint8_t new_y = 180;

        auto frame_pos = mock_bus.create_frame<r2d2::frame_type::CURSOR_POSITION>(
        {static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR), new_x, new_y});

        mock_bus.accept_frame(frame_pos);

        module.process();

        auto cursor_post = test_display.get_cursor(static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR));
        REQUIRE(cursor_post.cursor_x == start_x);
        REQUIRE(cursor_post.cursor_y == start_y);
    }

}


/*
 * When a character is written, the cursor has to move 8 pixels in order to
 * write the new character. This means that the cursor position can be
 * calculated according to starting position and number of characters moved.
 *
 * When a character would overwrite the current bounds, the cursor won't move
 * any further. This means that when a cursor will never go out of bounds.
 */
TEST_CASE("Manipulate cursor position through character writing",
          "[cursor, internal_communication]") {
    // The bus itself doesn't take any constructor arguments
    r2d2::mock_comm_c mock_bus;

    
    // Dummy display does nothing in set_pixel
    r2d2::display::display_dummy_c<
        r2d2::display::st7735_128x160_s>
        test_display;

    r2d2::display::module_c module(mock_bus, test_display);

    SECTION("Within bounds") {
        constexpr uint8_t start_x = 50;
        constexpr uint8_t start_y = 75;
        constexpr uint8_t string_length = 5;
        // Send the cursor to 50,75
        auto frame_pos =
            mock_bus.create_frame<r2d2::frame_type::CURSOR_POSITION>(
                {static_cast<uint8_t>(
                     r2d2::claimed_display_cursor::OPEN_CURSOR),
                 start_x, start_y});

        // Actually place the frame on the bus, so the module
        // can see and process it.
        mock_bus.accept_frame(frame_pos);

        // Manually call the process function on the module
        // to allow it to process everything.
        module.process();

        // Sanity check
        auto cursor_pre = test_display.get_cursor(
            static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR));

        REQUIRE(cursor_pre.cursor_x == start_x);
        REQUIRE(cursor_pre.cursor_y == start_y);

        auto frame_char = mock_bus.create_frame<
            r2d2::frame_type::DISPLAY_8X8_CHARACTER_VIA_CURSOR>(
            {static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR),
             {'T', 'e', 's', 't', '!'}});

        mock_bus.accept_frame(frame_char);
        module.process();

        auto cursor_post = test_display.get_cursor(
            static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR));

        REQUIRE(cursor_post.cursor_x == start_x + (string_length * 8));
        REQUIRE(cursor_post.cursor_y == start_y);
    }

    SECTION("Out of bounds") {
        constexpr uint8_t start_x = 100;
        constexpr uint8_t start_y = 75;

        // Send the cursor to 100,75
        auto frame_pos =
            mock_bus.create_frame<r2d2::frame_type::CURSOR_POSITION>(
                {static_cast<uint8_t>(
                     r2d2::claimed_display_cursor::OPEN_CURSOR),
                 start_x, start_y});

        // Actually place the frame on the bus, so the module
        // can see and process it.
        mock_bus.accept_frame(frame_pos);

        // Manually call the process function on the module
        // to allow it to process everything.
        module.process();

        // Sanity check
        auto cursor_pre = test_display.get_cursor(
            static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR));

        REQUIRE(cursor_pre.cursor_x == start_x);
        REQUIRE(cursor_pre.cursor_y == start_y);

        auto frame_char = mock_bus.create_frame<
            r2d2::frame_type::DISPLAY_8X8_CHARACTER_VIA_CURSOR>(
            {static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR),
             {'T', 'e', 's', 't', '!'}});

        mock_bus.accept_frame(frame_char);
        module.process();

        auto cursor_post = test_display.get_cursor(
            static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR));

        REQUIRE(cursor_post.cursor_x ==
                (start_x + ((r2d2::display::st7735_128x160_s::width - start_x) / 8) * 8));//124
        REQUIRE(cursor_post.cursor_y == start_y);
    }
}

/*
 * The cursor also stores color, and thus has to be be able to be changed.
 * The test will take a color, change it to red, green, blue and white.
 */

TEST_CASE("Change cursor color", "[cursor, internal_communication]") {
    // The bus itself doesn't take any constructor arguments
    r2d2::mock_comm_c mock_bus;

    // Dummy display does nothing in set_pixel
    r2d2::display::display_dummy_c<
        r2d2::display::st7735_128x160_s>
        test_display;

    r2d2::display::module_c module(mock_bus, test_display);
    SECTION("To red") {
        uint8_t red = 255;
        uint8_t green = 0;
        uint8_t blue = 0;
        // Change the color to red
        auto frame_color =
            mock_bus.create_frame<r2d2::frame_type::CURSOR_COLOR>(
                {static_cast<uint8_t>(
                     r2d2::claimed_display_cursor::OPEN_CURSOR),
                 red, green, blue});

        // Actually place the frame on the bus, so the module
        // can see and process it.
        mock_bus.accept_frame(frame_color);

        // Manually call the process function on the module
        // to allow it to process everything.
        module.process();
        // Sanity check
        auto cursor = test_display.get_cursor(
            static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR));

        auto cursor_color = cursor.cursor_color;
        REQUIRE(cursor_color.red == red);
        REQUIRE(cursor_color.green == green);
        REQUIRE(cursor_color.blue == blue);
    }

    SECTION("To green") {
        uint8_t red = 0;
        uint8_t green = 255;
        uint8_t blue = 0;
        // Change the color to green
        auto frame_color =
            mock_bus.create_frame<r2d2::frame_type::CURSOR_COLOR>(
                {static_cast<uint8_t>(
                     r2d2::claimed_display_cursor::OPEN_CURSOR),
                 red, green, blue});

        // Actually place the frame on the bus, so the module
        // can see and process it.
        mock_bus.accept_frame(frame_color);

        // Manually call the process function on the module
        // to allow it to process everything.
        module.process();
        // Sanity check
        auto cursor = test_display.get_cursor(
            static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR));

        auto cursor_color = cursor.cursor_color;
        REQUIRE(cursor_color.red == red);
        REQUIRE(cursor_color.green == green);
        REQUIRE(cursor_color.blue == blue);
    }

    SECTION("To blue") {
        uint8_t red = 0;
        uint8_t green = 0;
        uint8_t blue = 255;
        // Change the color to blue
        auto frame_color =
            mock_bus.create_frame<r2d2::frame_type::CURSOR_COLOR>(
                {static_cast<uint8_t>(
                     r2d2::claimed_display_cursor::OPEN_CURSOR),
                 red, green, blue});

        // Actually place the frame on the bus, so the module
        // can see and process it.
        mock_bus.accept_frame(frame_color);

        // Manually call the process function on the module
        // to allow it to process everything.
        module.process();
        // Sanity check
        auto cursor = test_display.get_cursor(
            static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR));

        auto cursor_color = cursor.cursor_color;
        REQUIRE(cursor_color.red == red);
        REQUIRE(cursor_color.green == green);
        REQUIRE(cursor_color.blue == blue);
    }

    SECTION("To white") {
        uint8_t red = 255;
        uint8_t green = 255;
        uint8_t blue = 255;
        // Change the color to white
        auto frame_color =
            mock_bus.create_frame<r2d2::frame_type::CURSOR_COLOR>(
                {static_cast<uint8_t>(
                     r2d2::claimed_display_cursor::OPEN_CURSOR),
                 red, green, blue});

        // Actually place the frame on the bus, so the module
        // can see and process it.
        mock_bus.accept_frame(frame_color);

        // Manually call the process function on the module
        // to allow it to process everything.
        module.process();
        // Sanity check
        auto cursor = test_display.get_cursor(
            static_cast<uint8_t>(r2d2::claimed_display_cursor::OPEN_CURSOR));

        auto cursor_color = cursor.cursor_color;
        REQUIRE(cursor_color.red == red);
        REQUIRE(cursor_color.green == green);
        REQUIRE(cursor_color.blue == blue);
    }
}