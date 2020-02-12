#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Fundamental
static constexpr float c_initial_yaw = 90.0f;  // angle from positive x-axis.
static constexpr float c_initial_pitch = 0.0f;
static constexpr float c_initial_fov = 45.0f;

static constexpr float c_min_fov = 1.0f;
static constexpr float c_max_fov = 45.0f;
static constexpr float c_min_pitch = -89.0f;
static constexpr float c_max_pitch = 89.0f;

static constexpr glm::vec3 c_world_up{0, 1, 0};

// Settings
static constexpr float c_movement_speed = 2.5f;
static constexpr float c_look_sensitivity = 0.1f;

// In the WORLD, positive x is right, positive y is up, positive z is forward.
class Camera {

public:

    using vec3 = glm::vec3;

    // Tweaks: sign of z in position, sign of yaw.

    // Position is in world. Move forward -> z coord getting larger.
    Camera()
        : position{0, 0, -10}, yaw{c_initial_yaw},
        pitch{c_initial_pitch}, fov{c_initial_fov}{

        update_cache();
    }

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    const glm::mat4& GetViewMatrix() {
        return view_matrix;
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime) {

        float velocity = c_movement_speed * deltaTime;
        switch(direction){
            case FORWARD:  position += front * velocity; break;
            case BACKWARD: position -= front * velocity; break;
            case RIGHT:    position += right * velocity; break;
            case LEFT:     position -= right * velocity; break;
        }
        update_cache();
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    // x offset positive -> mouse move right
    // y offset positive -> mouse moved down
    void ProcessMouseMovement(float xoffset, float yoffset) {

        // TODO understand mouse movement...
        // TODO understand mouse movement...
        // TODO understand mouse movement...
        // TODO understand mouse movement...
        yaw   -= xoffset * c_look_sensitivity;
        pitch -= yoffset * c_look_sensitivity;
        pitch = glm::clamp(pitch, c_min_pitch, c_max_pitch);
        update_cache();
    }

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset) {
        fov += yoffset;
        fov = glm::clamp(fov, c_min_fov, c_max_fov);
    }

    float get_fov() const {
        return fov;
    }

private:

    // Fundamental
    vec3 position;
    float yaw;
    float pitch;  // -89 <= pitch <= 89
    float fov;  // 1 <= fov <= 45

    // Cached Calculations.
    // For view matrix
    glm::mat4 view_matrix;
    // For Process keyboard
    vec3 front;
    vec3 right;

    // Calculates the front vector from the Camera's (updated) Euler Angles
    void update_cache() {

        /*
        Picture a plane going forward.
            Wings are x axis.
            Pilot's head points to y axis.
            Flying along z axis.
        When looking straight forward, pitch = 0, yaw = 90.
        */

        front = glm::normalize(vec3{
            cos(glm::radians(pitch)) * -cos(glm::radians(yaw)),
            sin(glm::radians(pitch)),
            // Flip z axis for open GL.
            cos(glm::radians(pitch)) * sin(glm::radians(yaw))
        });

        right = glm::normalize(glm::cross(front, c_world_up));

        vec3 up = glm::normalize(glm::cross(right, front));
        view_matrix = glm::lookAt(position, position + front, up);
    }
};
#endif