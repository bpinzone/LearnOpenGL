#ifndef CIRCULAR_PATH_H
#define CIRCULAR_PATH_H

#include <glm/glm.hpp>

#include "component.h"

enum Axis {
    X, Y, Z
};

// NOTE: For now, always rotates around the z-axis.
class Circular_path : public Component {

public:

    Circular_path(
        float depth_in, double starting_degrees_in,
        double radius_in, double translational_speed_in, Axis axis_in
    );

    void start() override;
    void update() override;

private:

    const double starting_degrees;
    const double radius;
    const double translational_speed;
    const double angular_speed;
    const Axis axis;
    const float depth;

    double degrees;
};

#endif
