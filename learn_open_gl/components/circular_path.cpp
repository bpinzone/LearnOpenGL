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
    glm::vec3 start_pos{0};
    switch(axis){
        case Axis::X : start_pos.x = depth; break;
        case Axis::Y : start_pos.y = depth; break;
        case Axis::Z : start_pos.z = depth; break;
    }
    game_object->get_transform().set_position(start_pos);
    game_object->get_transform().translate(glm::vec3(radius, 0, 0));
}

void Circular_path::update(){

    double degrees_differential = Time::get_instance().get_delta_time() * angular_speed;
    degrees += degrees_differential;
    const double sign = depth > 0 ? 1 : -1;
    const double mult = sign * radius;

    glm::vec3 new_position{0};

    // TODO: if this is std::sin(), it takes radians... Changing this will break cone demo though.
    const float cos_portion = mult * cos(degrees);
    const float sin_portion = mult * sin(degrees);

    // TODO: Not going to do scene graph just for this-
    // switch hack for choosing axis of rotation.

    switch(axis){
        case Axis::X :
            new_position = glm::vec3(depth, cos_portion, sin_portion);
            break;
        case Axis::Y :
            new_position = glm::vec3(cos_portion, depth, sin_portion);
            break;
        case Axis::Z :
            new_position = glm::vec3(cos_portion, sin_portion, depth);
            break;
    }
    game_object->get_transform().set_position(new_position);
}
