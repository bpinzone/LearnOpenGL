#ifndef RANDOM_ROTATOR_H
#define RANDOM_ROTATOR_H

#include <glm/glm.hpp>

#include "component.h"

class Random_rotator : public Component {

public:

    Random_rotator();

    void start() override;
    void update() override;

private:

    void rotate_randomly();

    glm::vec3 rotation_axis;
    float rotation_speed;
};


#endif