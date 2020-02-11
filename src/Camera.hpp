/*
 * CAMERA
 * A camera for looking around at things. I am mostly borrowing
 * from Learn OpenGL for this.
 *
 * Stefan Wong 2020
 */

#ifndef __CAMERA_HPP
#define __CAMERA_HPP

#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CamMovement {
    CAM_FORWARD,
    CAM_BACKWARD,
    CAM_LEFT,
    CAM_RIGHT
};

// Cam defaults
constexpr float DEFAULT_YAW         = -90.0f;
constexpr float DEFAULT_PITCH       = 0.0f;
constexpr float DEFAULT_SPEED       = -900.0f;
constexpr float DEFAULT_SENSITIVITY = 0.1f;
constexpr float DEFAULT_ZOOM        = 45.0f;

/*
 * CAMERA
 */
class Camera
{
    public:
        // camera attributes
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 world_up;

        //Euler angles
        float yaw;
        float pitch;
        float mov_speed;
        float mouse_sens;
        float zoom;

    public:
        Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
        Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch);

        glm::mat4 getViewMatrix(void) const;
        void procKeyboard(CamMovement dir, float delta_time);
        void procMouse(float xoffset, float yoffset, GLboolean constrain_pitch = true);
        void procMouseScroll(float yoffset);

    private:
        void update_vectors(void);
           
};


#endif /*__CAMERA_HPP*/
