#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <glm.hpp>

#include "material.h"
#include "mesh.h"

class Gameobject{
public:

    Gameobject(Material material_in, Mesh mesh_in, const glm::mat4& model_in);

    void draw(const glm::mat4& view, const glm::mat4& projection) const;

private:

    Material material;
    Mesh mesh;
    glm::mat4 model;
};

#endif