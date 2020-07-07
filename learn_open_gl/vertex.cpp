#include "vertex.h"

void Vertex::setup_vertex_attrib_ptrs(){

    // About glVertexAttribPointer:
    // (location, number things in this attribute, type, normalize?, stride (distance between vertices), offset to get to this attribute for first vertex.)

    int idx = 0;
    // Position attribute.
    glEnableVertexAttribArray(idx);
    glVertexAttribPointer(idx, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, position)));
    ++idx;

    // Normal attribute.
    glEnableVertexAttribArray(idx);
    glVertexAttribPointer(idx, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, normal)));
    ++idx;

    // Texture Coord attribute.
    glEnableVertexAttribArray(idx);
    glVertexAttribPointer(idx, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, tex_coord)));
    ++idx;
}

void GeomTestVertex::setup_vertex_attrib_ptrs(){

    int idx = 0;
    // Position attribute.
    glEnableVertexAttribArray(idx);
    glVertexAttribPointer(idx, 2, GL_FLOAT, GL_FALSE, sizeof(GeomTestVertex),
        reinterpret_cast<void*>(offsetof(GeomTestVertex, position)));
    ++idx;

    glEnableVertexAttribArray(idx);
    glVertexAttribPointer(idx, 3, GL_FLOAT, GL_FALSE, sizeof(GeomTestVertex),
        reinterpret_cast<void*>(offsetof(GeomTestVertex, color)));
    ++idx;
}