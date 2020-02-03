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

const std::string test_shader_file = "shader/test";

TEST_CASE("test_init_shader", "[classic]")
{
    Shader test_shader(test_shader_file);

}
