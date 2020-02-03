/*
 * TEST_DISPLAY
 * Unit tests for Display object
 *
 * Stefan Wong 2020
 */

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"
#include <string>


// unit(s) under test
#include "Display.hpp"


TEST_CASE("test_display_init", "[classic]")
{
    Display test_display(800, 600, "Hello World");

    while(!test_display.isClosed())
    {
        test_display.clear(0.31f, 0.44f, 0.3f, 1.0f);
        test_display.update();
    }
}
