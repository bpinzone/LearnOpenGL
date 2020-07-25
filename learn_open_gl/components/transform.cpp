#include "transform.h"
#include <matrix_transform.hpp>

#include "shader_globals.h"

Transform::Transform()
    : translation{glm::mat4(1.0)}, rotation{glm::mat4(1.0)},
    scale{glm::mat4(1.0)} {

    recalculate_transform();
}

void Transform::translate(const glm::vec3& differential){

    translation = glm::translate(
        translation, differential
    );
    recalculate_transform();
}

void Transform::set_position(const glm::vec3& position){
    translation = glm::mat4(1.0);
    translate(position);
}

glm::vec3 Transform::get_position() {
    return glm::vec3(translation[3]);
}

void Transform::set_translation(const glm::mat4 translation_in){
    translation = translation_in;
    recalculate_transform();
}

void Transform::set_rotation(const glm::mat4 rotation_in){
    rotation = rotation_in;
    recalculate_transform();
}

void Transform::set_scale(const glm::mat4 scale_in){
    scale = scale_in;
    recalculate_transform();
}

void Transform::set_scale(const glm::vec3 scale_in){
    scale = glm::scale(glm::mat4(1.0), scale_in);
    recalculate_transform();
}

glm::vec3 Transform::get_scale() const {
    return glm::vec3(
        scale[0][0], scale[1][1], scale[2][2]
    );
}


void Transform::load_into_shader_global() {

    Shader_globals::get_instance().set_model(transform);
    Shader_globals::get_instance().set_normal(normal);
}

void Transform::recalculate_transform(){
    transform = translation * rotation * scale;
    normal = glm::mat3(glm::transpose(glm::inverse(transform)));
}
