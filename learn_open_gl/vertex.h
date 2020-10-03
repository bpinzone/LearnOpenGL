#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

#include <glad/glad.h>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coord;

    // Assumes the proper VAO is ALREADY bound!
    // Makes calls to glEnableVertexAttribArray and glVertexAttribPointer
    // Returns the next available vert attrib idx.
    static int setup_vertex_attrib_ptrs();
};

struct GeomTestVertex {
    glm::vec2 position;
    glm::vec3 color;

    static int setup_vertex_attrib_ptrs();
};

struct PostProcessQuadVertex {
    glm::vec3 position;
    glm::vec2 tex_coord;

    static int setup_vertex_attrib_ptrs();
};

// NOTE: When adding another vertex class, add to the end of mesh.cpp
// The ctor AND the setup_vao decl.

#endif
