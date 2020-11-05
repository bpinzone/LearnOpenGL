#include "mesh.h"

#include <glad/glad.h>
// #include <glfw3.h>

#include "material.h"

#include <string>
#include <utility>
#include <algorithm>

using std::move;
using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::reverse;

template<typename V>
Mesh::Mesh(
        const std::vector<V>& vertices_in,
        const std::vector<unsigned int>& indices_in,
        shared_ptr<Material> material_in)
    : indices{indices_in}, material{material_in}{

    setup_vao(vertices_in);
}

Mesh::Mesh(
        shared_ptr<Shader> shader,
        aiMesh* mesh, const aiScene* scene,
        const string& model_dir) {

    material = load_material(shader, mesh, scene, model_dir);
    auto vertices = load_vertex_and_index_data(mesh, scene);

    setup_vao(vertices);
}

void Mesh::draw(shared_ptr<Material> mat_override) {

    if(mat_override){
        mat_override->use();
    }
    else{
        material->use();
    }
    // Automatically binds the ebo, where it takes the indices from.
    glBindVertexArray(vao);

    // Potential Problem: settings for geometry shader testing.
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    // glDrawArrays(GL_POINTS, 0, 4);

    glBindVertexArray(0);
}

void Mesh::draw_instanced(int num_instances, shared_ptr<Material> mat_override) {

    if(mat_override){
        mat_override->use_instance_variant();
    }
    else{
        material->use_instance_variant();
    }
    glBindVertexArray(vao);
    glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, num_instances);
}

void Mesh::reverse_winding_order(){
    reverse(indices.begin(), indices.end());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // TODO: Should change to buffer sub data.
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        // indices.size() * sizeof(unsigned int),
        indices.size() * sizeof(decltype(indices)::value_type),
        indices.data(), GL_STATIC_DRAW
    );
}

void Mesh::bind_vao() const {
    glBindVertexArray(vao);
}

int Mesh::get_next_available_attribute_idx() const {
    return next_available_attribute_idx;
}

vector<Vertex> Mesh::load_vertex_and_index_data(aiMesh* mesh, const aiScene* scene){

    vector<Vertex> vertices;

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

    return vertices;
}

shared_ptr<Material> Mesh::load_material(
        shared_ptr<Shader> shader,
        aiMesh* mesh, const aiScene* scene, const string& model_dir){

    // Mesh only contains an index to a single material object.
    // Of course, one material may have multiple textures.
    if(mesh->mMaterialIndex >= 0){

        aiMaterial* ai_material = scene->mMaterials[mesh->mMaterialIndex];
        return make_shared<Material>(shader, ai_material, model_dir);
    }
    return make_shared<Material>(shader, Material::Textures_t());
}

template<typename V>
void Mesh::setup_vao(const vector<V>& vertices){

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(V),
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
        indices.size() * sizeof(decltype(indices)::value_type),
        // indices.size() * sizeof(unsigned int),
        indices.data(), GL_STATIC_DRAW
    );

    next_available_attribute_idx = V::setup_vertex_attrib_ptrs();

    glBindVertexArray(0);

    /*
    More advanced buffer manipulation
    https://learnopengl.com/Advanced-OpenGL/Advanced-Data

    Manipulate buffer through a mapped pointer.
    Write to subsections of a buffer
    Copy buffers
    Alternate vertex attribute configuration.
    */
}


// Explicit instantiations

template Mesh::Mesh(
    const vector<Vertex>& vertices_in,
    const vector<unsigned int>& indices_in,
    shared_ptr<Material> material_in);

template Mesh::Mesh(
    const vector<GeomTestVertex>& vertices_in,
    const vector<unsigned int>& indices_in,
    shared_ptr<Material> material_in);

template Mesh::Mesh(
    const vector<PostProcessQuadVertex>& vertices_in,
    const vector<unsigned int>& indices_in,
    shared_ptr<Material> material_in);

template void Mesh::setup_vao(const std::vector<Vertex>& vertices);
template void Mesh::setup_vao(const std::vector<GeomTestVertex>& vertices);
template void Mesh::setup_vao(const std::vector<PostProcessQuadVertex>& vertices);
