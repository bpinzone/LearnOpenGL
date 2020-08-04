#include "random_rotator.h"

#include <glm/gtc/matrix_transform.hpp>

#include <random>
#include <functional>

#include "utility.h"
#include "game_object.h"
#include "transform.h"

Random_rotator::Random_rotator() {

    static auto random_unit_float = std::bind(
        std::uniform_real_distribution<float>{0.1, 1.0},
        std::default_random_engine{}
    );

    rotation_axis = glm::vec3{
        random_unit_float(), random_unit_float(), random_unit_float()};

    rotation_speed = random_unit_float() * 1.5f;
}

void Random_rotator::start() {
    Component::start();
    rotate_randomly();
}

void Random_rotator::update() {
    Component::update();
    rotate_randomly();
}

void Random_rotator::rotate_randomly() {

    game_object->get_transform().set_rotation(
        glm::rotate(
            game_object->get_transform().get_rotation(),
            static_cast<float>(rotation_speed * Time::get_instance().get_delta_time()),
            rotation_axis
        )
    );
}