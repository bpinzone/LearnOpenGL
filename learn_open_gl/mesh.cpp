#include "mesh.h"

#include <glad/glad.h>
#include <glfw3.h>

#include <string>
#include <utility>

using std::move;
using std::string;
using std::vector;

Mesh::Mesh(
        const std::vector<Vertex> vertices_in,
        const std::vector<unsigned int> indices_in,
        Material material_in)
    : vertices{vertices_in}, indices{indices_in}, material{material_in}{

    setup_vao();
}

Mesh::Mesh(
        Shader shader,
        aiMesh* mesh, const aiScene* scene,
        const std::string& model_dir,
        std::vector<Texture>& loaded_textures)
        : material {load_material(shader, mesh, scene, model_dir, loaded_textures)} {

    load_vertex_data(mesh, scene);
    setup_vao();
}

void Mesh::draw() {

    material.use();
    glBindVertexArray(vao);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::load_vertex_data(aiMesh* mesh, const aiScene* scene){

    for(size_t vert_idx = 0; vert_idx < mesh->mNumVertices; ++vert_idx){
        Vertex vertex;
        vertex.position = glm::vec3(
            // Assimp calls their vertex position array mVertices which isn't really too intuitive.
            mesh->mVertices[vert_idx].x,
            mesh->mVertices[vert_idx].y,
            mesh->mVertices[vert_idx].z
        );
        vertex.normal = glm::vec3(
            mesh->mNormals[vert_idx].x,
            mesh->mNormals[vert_idx].y,
            mesh->mNormals[vert_idx].z
        );
        if(mesh->mTextureCoords[0]){
            // The mesh has at least 1 texture coord.
            // Only care about the first one.
            vertex.tex_coord = glm::vec2(
                mesh->mTextureCoords[0][vert_idx].x,
                mesh->mTextureCoords[0][vert_idx].y
            );
        }
        else{
            vertex.tex_coord = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(move(vertex));
    }

    // process indices
    for(size_t face_idx = 0; face_idx < mesh->mNumFaces; ++face_idx){
        aiFace face = mesh->mFaces[face_idx];
        for(unsigned int indice_idx = 0; indice_idx < face.mNumIndices; ++indice_idx){
            indices.push_back(face.mIndices[indice_idx]);
        }
    }
}

Material Mesh::load_material(
        Shader shader, aiMesh* mesh, const aiScene* scene,
        const string& model_dir, vector<Texture>& loaded_textures){

    // Mesh only contains an index to a single material object.
    // Of course, one material may have multiple textures.
    if(mesh->mMaterialIndex >= 0){

        aiMaterial* ai_material = scene->mMaterials[mesh->mMaterialIndex];
        return Material(shader, ai_material, model_dir, loaded_textures);
    }
    else{
        return Material(shader, {});
    }

}

void Mesh::setup_vao(){

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        vertices.data(), GL_STATIC_DRAW
    );

    /*
    A VAO stores the glBindBuffer calls when the target is GL_ELEMENT_ARRAY_BUFFER.
    This also means it stores its unbind calls so make sure you don't unbind the
    element array buffer before unbinding your VAO, otherwise it doesn't have an EBO configured.
    */
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int),
        indices.data(), GL_STATIC_DRAW
    );

    // About glVertexAttribPointer:
    // (location, number things in this attribute, type, normalize?, stride (distance between vertices), offset to get to this attribute for first vertex.)

    // Position attribute.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
    // NOTE TODO: These are now flipped.
    // Normal attribute.
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
    // Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tex_coord)));

    glBindVertexArray(0);
}
