#include "circular_path.h"

#include "game_object.h"
#include "transform.h"
#include "utility.h"

#include <cmath>

Circular_path::Circular_path(
        const glm::vec3& center_in, double starting_degrees_in,
        double radius_in, double translational_speed_in)

    : center{center_in}, starting_degrees{starting_degrees_in},
    radius{radius_in}, translational_speed{translational_speed_in},
    angular_speed{translational_speed_in / radius_in},
    degrees{0} {

}

// Move the object to its first point.
void Circular_path::start(){
    game_object->get_transform().set_position(center);
    game_object->get_transform().translate(glm::vec3(radius, 0, 0));
}

void Circular_path::update(){

    double degrees_differential = Time::get_instance().get_delta_time() * angular_speed;
    degrees += degrees_differential;
    game_object->get_transform().set_position(glm::vec3(
        cos(degrees) * radius, sin(degrees) * radius, center.z
    ));
}
