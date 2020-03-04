#ifndef MESH_H
#define MESH_H

#include "vertex.h"
#include "shader.h"
#include "texture.h"

#include <vector>

// A single drawable object.
class Mesh {
public:

    //  Mesh Data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(
        const std::vector<Vertex>& vertices_in,
        const std::vector<unsigned int>& indices_in,
        const std::vector<Texture>& textures_in
    );

    void draw(Shader shader);

private:

    unsigned int vao, vbo, ebo;

    void setup_mesh();

};

#endif