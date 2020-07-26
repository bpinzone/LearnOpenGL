#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm.hpp>

// TODO: Why did the previous rotation stuff ever work?
class Transform {

public:

    Transform();

    // Translation
    // Matrices
    void set_translation(const glm::mat4 translation_in);
    // Vectors
    glm::vec3 get_position();
    void translate(const glm::vec3& differential);
    void set_position(const glm::vec3& position);

    // Rotation
    // todo: stupid, these should be references.
    void set_rotation(const glm::mat4 rotation_in);

    // Scale
    void set_scale(const glm::mat4 scale_in);
    void set_scale(const glm::vec3 scale_in);
    glm::vec3 get_scale() const;

    void load_into_shader_global();

    const glm::mat4& get_model() const;
    const glm::mat3& get_normal() const;

private:

    void recalculate_model();

    // Fundamental
    glm::mat4 translation, rotation, scale;

    // Cached
    glm::mat4 model;
    glm::mat3 normal;

};

#endif