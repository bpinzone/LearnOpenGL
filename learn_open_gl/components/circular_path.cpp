#include "circular_path.h"

#include "game_object.h"
#include "transform.h"
#include "utility.h"

#include <cmath>

Circular_path::Circular_path(
        float depth_in, double starting_degrees_in,
        double radius_in, double translational_speed_in, Axis axis_in)

    : depth{depth_in}, starting_degrees{starting_degrees_in},
    radius{radius_in}, translational_speed{translational_speed_in},
    angular_speed{translational_speed_in / radius_in},
    degrees{0}, axis{axis_in} {

}

// Move the object to its first point.
void Circular_path::start(){
    switch(axis){
        case Axis::X :
            game_object->get_transform().set_position(glm::vec3(depth, 0, 0));
            break;
        case Axis::Y :
            game_object->get_transform().set_position(glm::vec3(0, depth, 0));
            break;
        case Axis::Z :
            game_object->get_transform().set_position(glm::vec3(0, 0, depth));
            break;
    }
    game_object->get_transform().translate(glm::vec3(radius, 0, 0));
}

void Circular_path::update(){

    double degrees_differential = Time::get_instance().get_delta_time() * angular_speed;
    degrees += degrees_differential;
    switch(axis){
        case Axis::X :
            game_object->get_transform().set_position(glm::vec3(
                depth, cos(degrees) * radius, sin(degrees) * radius
            ));
            break;
        case Axis::Y :
            game_object->get_transform().set_position(glm::vec3(
                cos(degrees) * radius, depth, sin(degrees) * radius
            ));
            break;
        case Axis::Z :
            game_object->get_transform().set_position(glm::vec3(
                cos(degrees) * radius, sin(degrees) * radius, depth
            ));
            break;

    }
}
