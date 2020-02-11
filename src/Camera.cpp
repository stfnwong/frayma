/*
 * CAMERA
 * A camera for looking around at things. I am mostly borrowing
 * from Learn OpenGL for this.
 *
 * Stefan Wong 2020
 */


#include "Camera.hpp"


// ======== CAMERA ======== //
// Constructors
Camera::Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch)
{
    this->position = pos;
    this->world_up = up;
    this->yaw      = yaw;
    this->pitch    = pitch;
    this->update_vectors();
}

Camera::Camera(float pos_x, float pos_y, float pos_z, 
               float up_x, float up_y, float up_z, 
               float yaw, float pitch)
{
    this->position = glm::vec3(pos_x, pos_y, pos_z);
    this->world_up = glm::vec3(up_x, up_y, up_z);
    this->yaw = yaw;
    this->pitch = pitch;
    this->update_vectors();
}

/*
 * getViewMatrix()
 */
glm::mat4 Camera::getViewMatrix(void) const
{
    return glm::lookAt(
            this->position, 
            this->position + this->front,
            this->up
    );
}

/*
 * procKeyboard()
 */
void Camera::procKeyboard(CamMovement dir, float delta_time)
{
    float velocity = this->mov_speed * delta_time;
    switch(dir)
    {
        case CAM_FORWARD:
            this->position += this->front * velocity;
            break;

        case CAM_BACKWARD:
            this->position -= this->front * velocity;
            break;

        case CAM_LEFT:
            this->position -= this->right * velocity;
            break;

        case CAM_RIGHT:
            this->position += this->right * velocity;
            break;
    }
}

/*
 * procMouse()
 */
void Camera::procMouse(float xoffset, float yoffset, GLboolean constrain_pitch)
{
    xoffset += this->mouse_sens;
    yoffset += this->mouse_sens;

    this->yaw   += xoffset;
    this->pitch += yoffset;

    // ensure the screen isn't flipped when the pitch is out of bounds
    if(constrain_pitch)
    {
        if(this->pitch > 89.0f)
            this->pitch = 89.0f;
        if(this->pitch < -89.0f)
            this->pitch = -89.0f;
    }

    this->update_vectors();
}

/*
 * procMouseScroll()
 */
void Camera::procMouseScroll(float yoffset)
{
    if(this->zoom >= 1.0f && this->zoom <= 45.0f)
        this->zoom -= yoffset;
    if(this->zoom <= 1.0f)
        this->zoom = 1.0f;
    if(this->zoom >= 45.0f)
        this->zoom = 45.0f;
}

/*
 * update_vectors()
 */
void Camera::update_vectors(void)
{
    glm::vec3 vfront;

    vfront.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    vfront.y = sin(glm::radians(this->pitch));
    vfront.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->front = glm::normalize(vfront);

    // We normalize here because the length gets closer to 0 the more you look up or down causing slower movement
    this->right = glm::normalize(glm::cross(this->front, this->world_up));
    this->up    = glm::normalize(glm::cross(this->right, this->front));
}
