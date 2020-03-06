#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <glm.hpp>

#include "model.h"

class Gameobject{
public:

    Gameobject(const glm::mat4& transform_model_in, Model model_in, double radius_in, double speed_in);

    void draw();

    void update(float delta_time);

    void set_transform_model(const glm::mat4 transform_model_in);

    glm::vec3 get_position();

private:

    glm::mat4 transform_model;
    glm::mat3 normal;

    Model model;

    double radius;
    double speed;

    double degrees;


};

#endif