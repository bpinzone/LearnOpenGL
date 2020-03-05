#include "game_object.h"
#include "shader_globals.h"

#include <random>
#include <functional>

Gameobject::Gameobject(
        const glm::mat4& transform_model_in, Model model_in)
    : model{model_in} {

    set_transform_model(transform_model_in);
}

void Gameobject::draw() {

    model.draw();
}

void Gameobject::update(float delta_time) {

}

void Gameobject::set_transform_model(const glm::mat4 transform_model_in) {
    transform_model = transform_model_in;
    normal = glm::mat3(glm::transpose(glm::inverse(transform_model)));
}
