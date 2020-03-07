#include "model.h"

#include "vertex.h"

#include <glm.hpp>

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
    );

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        throw std::runtime_error{
            "Failed to load model at path: " + model_path + " because " + importer.GetErrorString()
        };
    }

    load_meshes(scene->mRootNode, scene, shader);
}

void Model::draw(){
    for(auto& mesh : meshes){
        mesh->draw();
    }
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

