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

static const std::string test_vert_shader = "shader/test.vert";
static const std::string test_frag_shader = "shader/test.frag";
static const std::string test_attrib      = "tex_coord";

// Actually.. I may need to create a display here first..

TEST_CASE("test_init_shader", "[classic]")
{
    int status = 0;
    Display test_display(800, 600, "shader test");
    //Shader test_shader(test_vert_shader, test_frag_shader);
    Shader test_shader;

    std::cout << "[" << __func__ << "] compiling shader..." << std::endl;
    status = (int) test_shader.compile(test_vert_shader, test_frag_shader, test_attrib);
    std::cout << "[" << __func__ << "] status was " << status << std::endl;
    REQUIRE(status == 0);
    REQUIRE(test_shader.ok() == true);

    while(!test_display.isClosed())
    {
        test_display.clear(0.3f, 0.1f, 0.0f, 1.0f);
        test_shader.use();
        test_display.update();
    }
}

// TODO : this fragment shader is not correct...
//TEST_CASE("test_fractal_shader", "[classic]")
//{
//    int status = 0;
//    Display test_display(800, 600, "shader test");
//    //Shader test_shader(test_vert_shader, test_frag_shader);
//    Shader test_shader;
//
//    std::cout << "[" << __func__ << "] compiling shader..." << std::endl;
//    status = (int) test_shader.compile("shader/fractal.vert", "shader/fractal.frag", test_attrib);
//    std::cout << "[" << __func__ << "] status was " << status << std::endl;
//    REQUIRE(status == 0);
//    REQUIRE(test_shader.ok() == true);
//
//    while(!test_display.isClosed())
//    {
//        test_display.clear(0.3f, 0.1f, 0.0f, 1.0f);
//        test_shader.use();
//        test_display.update();
//    }
//}
