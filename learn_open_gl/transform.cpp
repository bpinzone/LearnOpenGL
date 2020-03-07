#include "transform.h"
#include <matrix_transform.hpp>

#include "shader_globals.h"

Transform::Transform(){
    set_transform(glm::mat4(1.0));
}

Transform::Transform(const glm::mat4& transform_in) {
    set_transform(transform_in);
}

void Transform::translate(const glm::vec3& differential){
    set_transform(glm::translate(transform, differential));
}

// TODO: Don't overwrite rotation.
void Transform::set_position(const glm::vec3& position){
    set_transform(glm::translate(glm::mat4(1.0), position));
}

glm::vec3 Transform::get_position() {
    return glm::vec3(transform[3]);
}

void Transform::rotate(float angle, const glm::vec3& axis){
   set_transform(glm::rotate(transform, angle, axis));
}

void Transform::load_into_shader_global() {

    Shader_globals::get_instance().set_model(transform);
    Shader_globals::get_instance().set_normal(normal);
}

void Transform::set_transform(const glm::mat4& transform_in) {
    transform = transform_in;
    normal = glm::mat3(glm::transpose(glm::inverse(transform)));
}
