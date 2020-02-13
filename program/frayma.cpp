/*
 * FRAYMA
 * Rendered program
 *
 * Stefan Wong 2020
 */

#include <iostream>
#include <string>

#include <glm/glm.hpp>

#include "Shader.hpp"
#include "Camera.hpp"

static const std::string frag_shader = "shader/fractal.frag";
static const std::string vert_shader = "shader/fractal.vert";
static const std::string shader_attrib_name = "v_position";

int main(int argc, char* argv[])
{

    int status = 0;
    // Get a Camera
    glm::vec3 cam_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cam_up  = glm::vec3(0.0f, 1.0f, 0.0f);

    Camera cam(
        cam_pos,
        cam_up,
        DEFAULT_YAW,
        DEFAULT_PITCH
    );

    // Get a shader and build the shader object
    Shader shader;
    status = shader.compile(
            frag_shader,
            vert_shader,
            shader_attrib_name
    );
    if(status < 0)
    {
        std::cout << "[" << __func__ << "] error compiling shader with input files "
            << frag_shader << " and " << vert_shader << std::endl;
        return -1;
    }

    return 0;
}
