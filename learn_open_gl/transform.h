#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm.hpp>

// TODO: Why did the previous rotation stuff ever work?
class Transform {

public:

    Transform();

    void translate(const glm::vec3& differential);
    void set_position(const glm::vec3& position);
    glm::vec3 get_position();

    void set_translation(const glm::mat4 translation_in);
    void set_rotation(const glm::mat4 rotation_in);
    void set_scale(const glm::mat4 scale_in);

    void load_into_shader_global();

private:

    void recalculate_transform();

    // Fundamental
    glm::mat4 translation, rotation, scale;

    // Cached
    glm::mat4 transform;
    glm::mat3 normal;

};

#endif