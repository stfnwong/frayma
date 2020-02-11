/*
 * TEST_CAMERA
 * Unit tests for Camera object
 *
 * Stefan Wong 2020
 */

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <glm/glm.hpp>
// unit(s) under test
#include "Camera.hpp"


TEST_CASE("test_init_camera", "[classic]")
{
    glm::vec3 test_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 test_up  = glm::vec3(0.0f, 1.0f, 0.0f);

    Camera test_camera(
            test_pos,
            test_up,
            DEFAULT_YAW,
            DEFAULT_PITCH
    );

    REQUIRE(test_camera.yaw == DEFAULT_YAW);
    REQUIRE(test_camera.pitch == DEFAULT_PITCH);
}
