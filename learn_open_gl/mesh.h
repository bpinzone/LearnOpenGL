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
// Conceptually, it has it's structural data and a material.
// It DOES NOT store the vertex data in a separate container after construction. It only puts it in the buffer.
// It DOES store the indices in case you want to reverse the winding order.

// NOTE: V is a type representing a vertex. The material's shader MUST use this layout.
// This is a potential problem: programming by convention here.
class Mesh {
public:

    std::shared_ptr<Material> material;

    // Give mesh data directly.
    template<typename V>
    Mesh(
        const std::vector<V>& vertices_in,
        const std::vector<unsigned int>& indices_in,
        std::shared_ptr<Material> material_in
    );

    // Let mesh read from assimp assets.
    // Vertex type will be the actual "Vertex" struct.
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

    // Using assimp, returns the vertex vector, but puts the indices into the indices member.
    std::vector<Vertex> load_vertex_and_index_data(aiMesh* mesh, const aiScene* scene);
    std::shared_ptr<Material> load_material(
        std::shared_ptr<Shader> shader,
        aiMesh* mesh, const aiScene* scene, const std::string& model_dir
    );

    //  Mesh Data

    // ACTUALLY: don't really need this after we setup the vao.
    // std::vector<V> vertices;

    std::vector<unsigned int> indices;

    unsigned int vao, vbo, ebo;

    // Requires: This V is the same as the V in constructor call, if this is called from a templated constructor.
    template<typename V>
    void setup_vao(const std::vector<V>& vertices);

};

#endif
