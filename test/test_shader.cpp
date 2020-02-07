/*
 * TEST_SHADER
 * Unit tests for Shader object
 *
 * Stefan Wong 2020
 */

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"
#include <string>

// unit(s) under test
#include "Shader.hpp"
#include "Display.hpp"

const std::string test_vert_shader = "shader/test.vert";
const std::string test_frag_shader = "shader/test.frag";

// Actually.. I may need to create a display here first..

TEST_CASE("test_init_shader", "[classic]")
{
    Display test_display(800, 600, "shader test");
    Shader test_shader(test_vert_shader, test_frag_shader);

    // TODO : what to test?
    while(!test_display.isClosed())
    {
        test_display.clear(0.3f, 0.1f, 0.0f, 1.0f);
        test_shader.use();
        test_display.update();
    }
}
