#include "connector.h"

#include "utility.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
    glm::vec3 c1_to_c2 =
        connectee_2->get_transform().get_position() -
        connectee_1->get_transform().get_position();
    float distance = length(c1_to_c2);

    Transform& transform = game_object->get_transform();

    transform.set_position(
        connectee_1->get_transform().get_position() + (c1_to_c2 / 2.0f)
    );

    transform.set_rotation(glm::rotate(
        glm::mat4(1),
        glm::angle(starting_angle, glm::normalize(c1_to_c2)),
        glm::normalize(glm::cross(starting_angle, c1_to_c2))
    ));

    transform.set_scale(glm::scale(
        glm::mat4(1),
        glm::vec3{distance / 2, 0.2, 0.2}
    ));
}
