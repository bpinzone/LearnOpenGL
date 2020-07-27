#ifndef CIRCULAR_PATH_H
#define CIRCULAR_PATH_H

#include <glm/glm.hpp>

#include "component.h"

// NOTE: For now, always rotates around the z-axis.
class Circular_path : public Component {

public:

    Circular_path(
        const glm::vec3& center_in, double starting_degrees_in,
        double radius_in, double translational_speed_in
    );

    void start() override;
    void update() override;

private:

    const glm::vec3 center;
    const double starting_degrees;
    const double radius;
    const double translational_speed;
    const double angular_speed;

    double degrees;
};

#endif
