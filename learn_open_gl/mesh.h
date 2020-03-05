#ifndef MESH_H
#define MESH_H

#include "vertex.h"
#include "shader.h"
#include "texture.h"
#include "material.h"

#include <assimp/scene.h>

#include <vector>

// A single drawable object.
// Has it's structural data and a material.
class Mesh {
public:

    //  Mesh Data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Material material;

    Mesh(
        const std::vector<Vertex> vertices_in,
        const std::vector<unsigned int> indices_in,
        Material material_in
    );

    Mesh(
        Shader shader,
        aiMesh* mesh, const aiScene* scene,
        const std::string& model_dir,
        std::vector<Texture>& loaded_textures
    );

    void set_material(Material material_in){
        material = material_in;
    }

    void draw();

    // TODO dtor, sometime:
    // glDeleteVertexArrays(1, &vao);
    // glDeleteBuffers(1, &vbo);
    // glDeleteBuffers(1, &ebo);

private:

    void load_vertex_data(aiMesh* mesh, const aiScene* scene);
    Material load_material(
        Shader shader, aiMesh* mesh, const aiScene* scene,
        const std::string& model_dir, std::vector<Texture>& loaded_textures
    );

    unsigned int vao, vbo, ebo;

    void setup_vao();

};

#endif