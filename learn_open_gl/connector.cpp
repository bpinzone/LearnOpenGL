#include "connector.h"

#include "utility.h"

#include <glm.hpp>
#include <matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

Connector::Connector(Connectee_t c1, Connectee_t c2)
: connectee_1{c1}, connectee_2{c2} {
}

void Connector::start(){
   update_transform();
}

void Connector::update(){
   update_transform();
}

void Connector::set_connected_objects(Connectee_t c1, Connectee_t c2) {
   connectee_1 = c1;
   connectee_2 = c2;
}

void Connector::update_transform() {

    static const glm::vec3 starting_angle = glm::vec3{1, 0, 0};
    glm::mat4 new_transform(1.0);
    glm::vec3 c1_to_c2 =
        connectee_2->get_transform().get_position() -
        connectee_1->get_transform().get_position();
    float distance = length(c1_to_c2);

    new_transform = glm::translate(
        new_transform,
        connectee_1->get_transform().get_position() + (c1_to_c2 / 2.0f)
    );

    new_transform = glm::rotate(
        new_transform,
        glm::angle(starting_angle, glm::normalize(c1_to_c2)),
        glm::normalize(glm::cross(starting_angle, c1_to_c2))
    );

    new_transform = glm::scale(new_transform, glm::vec3{distance / 2, 0.2, 0.2});

    game_object->get_transform().set_transform(new_transform);

}
