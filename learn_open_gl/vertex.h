#ifndef VERTEX_H
#define VERTEX_H

#include <glm.hpp>

#include <glad/glad.h>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coord;

    // Assumes the proper VAO is ALREADY bound!
    // Makes calls to glEnableVertexAttribArray and glVertexAttribPointer
    static void setup_vertex_attrib_ptrs();
};

struct GeomTestVertex {
    glm::vec2 position;
    glm::vec3 color;

    static void setup_vertex_attrib_ptrs();
};

#endif
