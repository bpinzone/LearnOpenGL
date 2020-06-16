#ifndef MESH_H
#define MESH_H

#include "vertex.h"
#include "shader.h"
#include "texture.h"
#include "material.h"

#include <assimp/scene.h>

#include <vector>
#include <memory>

// A single drawable object.
// Has it's structural data and a material.
class Mesh {
public:

    std::shared_ptr<Material> material;

    Mesh(
        const std::vector<Vertex>& vertices_in,
        const std::vector<unsigned int>& indices_in,
        std::shared_ptr<Material> material_in
    );

    Mesh(
        std::shared_ptr<Shader> shader,
        aiMesh* mesh, const aiScene* scene,
        const std::string& model_dir
    );

   void set_material(std::shared_ptr<Material> material_in){
        material = material_in;
    }

    // Calls use on the material.
    // Draws elements using our vao generated from vertex data.
    void draw();

    void reverse_winding_order();

    // TODO dtor, sometime:
    // glDeleteVertexArrays(1, &vao);
    // glDeleteBuffers(1, &vbo);
    // glDeleteBuffers(1, &ebo);

private:

    void load_vertex_data(aiMesh* mesh, const aiScene* scene);
    std::shared_ptr<Material> load_material(
        std::shared_ptr<Shader> shader,
        aiMesh* mesh, const aiScene* scene, const std::string& model_dir
    );

    //  Mesh Data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    unsigned int vao, vbo, ebo;

    void setup_vao();

};

#endif
