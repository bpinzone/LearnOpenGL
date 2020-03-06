#include "game_object.h"
#include "shader_globals.h"

#include <glfw3.h>
#include <glm.hpp>

#include <random>
#include <cmath>
#include <functional>

Gameobject::Gameobject(
        const glm::mat4& transform_model_in, Model model_in, double radius_in, double speed_in)
    : model{model_in}, radius{radius_in}, speed{speed_in}, degrees{0} {

    set_transform_model(transform_model_in);
}

void Gameobject::draw() {

    Shader_globals::get_instance().set_model(transform_model);
    Shader_globals::get_instance().set_normal(normal);

    model.draw();
}

void Gameobject::update(float delta_time) {

    double angular_speed = speed / radius;
    degrees += angular_speed * delta_time;

    set_transform_model(glm::translate(
        glm::mat4(1),
        glm::vec3(
            cos(degrees) * radius,
            sin(degrees) * radius,
            0
        )
    ));
}

void Gameobject::set_transform_model(const glm::mat4 transform_model_in) {
    transform_model = transform_model_in;
    normal = glm::mat3(glm::transpose(glm::inverse(transform_model)));
}

glm::vec3 Gameobject::get_position(){
    return transform_model[3].xyz;
}
