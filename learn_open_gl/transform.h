#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm.hpp>

// TODO: Why did the previous rotation stuff ever work?
class Transform {

public:

    Transform();
    Transform(const glm::mat4& transform_in);

    void translate(const glm::vec3& differential);

    void set_position(const glm::vec3& position);
    glm::vec3 get_position();

    // degrees
    void rotate(float angle, const glm::vec3& axis);

    void load_into_shader_global();

private:

    void set_transform(const glm::mat4& transform_in);

    glm::mat4 transform;
    glm::mat3 normal;

};

#endif