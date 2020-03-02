#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <glm.hpp>

#include "material.h"
#include "mesh.h"

class Gameobject{
public:

    Gameobject(Material material_in, Mesh mesh_in, const glm::mat4& model_in);

    void draw() const;

    void update(float delta_time);

    void set_model(const glm::mat4 model_in);

private:

    Material material;
    Mesh mesh;
    glm::mat4 model;
    glm::mat3 normal;

    glm::vec3 rotation_axis;
    float rotation_speed;

};

#endif