#ifndef VERTEX_H
#define VERTEX_H

#include <glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coord;
};

#endif
