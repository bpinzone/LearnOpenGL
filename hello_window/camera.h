#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

class Camera {
public:

    using vec3 = glm::vec3;

    enum Movement {
        FORWARD, BACKWARD,
        LEFT, RIGHT
    };

    // Place the camera at (0, 0, 10), looking forward.
    Camera();

    // Returns the view matrix for this camera.
    const glm::mat4& GetViewMatrix() {
        return view_matrix;
    }

    // Let camera process directional inputs.
    void ProcessKeyboard(Movement direction, float deltaTime);

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset);

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset);

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
    void update_cache();
};
#endif