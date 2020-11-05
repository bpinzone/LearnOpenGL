#include "model.h"

#include "vertex.h"

#include "material.h"

#include <glm/glm.hpp>

#include <stdexcept>

using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;

Model::Model(const Meshes_t& meshes_in)
    : meshes{meshes_in}{
}

Model::Model(shared_ptr<Shader> shader, const string& model_path) {

    directory = model_path.substr(0, model_path.find_last_of('/')) + "/";

    Assimp::Importer importer;

    // Last flag here will put multiple normals on one vertex if necessary.
    const aiScene* scene = importer.ReadFile(
        model_path,
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals
        // aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals
    );

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        throw std::runtime_error{
            "Failed to load model at path: " + model_path + " because " + importer.GetErrorString()
        };
    }

    load_meshes(scene->mRootNode, scene, shader);
}

Model::Model(shared_ptr<Material> mat, const std::string& model_path)
    : Model(mat->get_shader(), model_path){
    set_materials(mat);
}

void Model::draw(shared_ptr<Material> mat_override){
    for(auto& mesh : meshes){
        mesh->draw(mat_override);
    }
}

void Model::draw_instanced(int num_instances, shared_ptr<Material> mat_override){
    for(auto& mesh : meshes){
        mesh->draw_instanced(num_instances, mat_override);
    }
}

// Requires: This model only has a single mesh. Binds that mesh's vao.
// Throws: If this model doesn't have exactly one mesh.
void Model::bind_only_mesh_vao() const {
    if(meshes.size() != 1){
        throw std::runtime_error{"Model has more than one mesh."};
    }
    meshes.front()->bind_vao();
}

// Throws: If this model doesn't have exactly one mesh.
int Model::get_only_mesh_next_avail_vert_attrib_idx() const {
    if(meshes.size() != 1){
        throw std::runtime_error{"Model has more than one mesh."};
    }
    return meshes.front()->get_next_available_attribute_idx();
}

void Model::load_meshes(aiNode* node, const aiScene* scene, shared_ptr<Shader> shader) {

    for(unsigned int i = 0; i < node->mNumMeshes; ++i){
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(make_shared<Mesh>(shader, mesh, scene, directory));
    }

    for(unsigned int i = 0; i < node->mNumChildren; ++i){
        load_meshes(node->mChildren[i], scene, shader);
    }
}

