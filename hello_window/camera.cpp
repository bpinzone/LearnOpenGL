#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

// Initial values
static constexpr glm::vec3 c_initial_position{0, 0, 10};
static constexpr float c_initial_yaw = 90.0f;  // angle from positive x-axis.
static constexpr float c_initial_pitch = 0.0f;
static constexpr float c_initial_fov = 45.0f;

// Constraints
static constexpr float c_min_fov = 1.0f;
static constexpr float c_max_fov = 45.0f;
static constexpr float c_min_pitch = -89.0f;
static constexpr float c_max_pitch = 89.0f;

// Settings
static constexpr float c_movement_speed = 2.5f;
static constexpr float c_look_sensitivity = 0.1f;
static constexpr glm::vec3 c_world_up{0, 1, 0};

Camera::Camera()
    : position{c_initial_position}, yaw{c_initial_yaw},
    pitch{c_initial_pitch}, fov{c_initial_fov}{

    update_cache();
}

void Camera::ProcessKeyboard(Movement direction, float deltaTime) {
    float velocity = c_movement_speed * deltaTime;
    switch(direction){
        case FORWARD:  position += front * velocity; break;
        case BACKWARD: position -= front * velocity; break;
        case RIGHT:    position += right * velocity; break;
        case LEFT:     position -= right * velocity; break;
    }
    update_cache();
}

// x offset positive -> mouse move right, want yaw to decrease.
// y offset positive -> mouse moved down, want pitch to decrease.
void Camera::ProcessMouseMovement(float xoffset, float yoffset) {
    yaw   -= xoffset * c_look_sensitivity;
    pitch -= yoffset * c_look_sensitivity;
    pitch = glm::clamp(pitch, c_min_pitch, c_max_pitch);
    update_cache();
}

void Camera::ProcessMouseScroll(float yoffset) {
    fov += yoffset;
    fov = glm::clamp(fov, c_min_fov, c_max_fov);
}

void Camera::update_cache() {

    /*
    Picture a plane flying going forward.
        Wings are x axis. Right wing is positive x.
        Pilot's head points to positive y.
        Flying into the -z axis.
    When looking straight forward, pitch = 0, yaw = 90.
    */
    front = glm::normalize(glm::vec3{
        cos(glm::radians(pitch)) * cos(glm::radians(yaw)),
        sin(glm::radians(pitch)),
        cos(glm::radians(pitch)) * -sin(glm::radians(yaw))
    });

    right = glm::normalize(glm::cross(front, c_world_up));
    glm::vec3 up = glm::normalize(glm::cross(right, front));

    // Could make view matrix myself.
    view_matrix = glm::lookAt(position, position + front, up);
}
