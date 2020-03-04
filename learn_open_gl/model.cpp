#include "model.h"

#include "vertex.h"

#include <glm.hpp>

#include <stdexcept>

using std::string;
using std::vector;

Model::Model(const string& path){
    load_model(path);
}

void Model::draw(Shader shader){
    for(auto& mesh : meshes){
        mesh.draw(shader);
    }
}

void Model::load_model(const string& path) {

    Assimp::Importer importer;
    // todo because of the UV flip, you may not need to flip your textures!
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals
    );

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        throw std::runtime_error{
            "Failed to load model at path: " + path + " because " + importer.GetErrorString()
        };
    }

    directory = path.substr(0, path.find_last_of('/'));

    process_node(scene->mRootNode, scene);

}

void Model::process_node(aiNode* node, const aiScene* scene) {

    for(unsigned int i = 0; i < node->mNumMeshes; ++i){
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(process_mesh(mesh, scene));
    }

    for(unsigned int i = 0; i < node->mNumChildren; ++i){
        process_node(node->mChildren[i], scene);
    }
}

Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene) {

    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

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
        vertices.push_back(vertex);
    }

    // process indices
    for(size_t face_idx = 0; face_idx < mesh->mNumFaces; ++face_idx){
        aiFace face = mesh->mFaces[face_idx];
        for(unsigned int indice_idx = 0; indice_idx < face.mNumIndices; ++indice_idx){
            indices.push_back(face.mIndices[indice_idx]);
        }
    }

    // process materials
    // Mesh only contains an index to a single material object.
    // Of course, one material may have multiple textures.
    if(mesh->mMaterialIndex >= 0){
        // The mesh contains a material
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        vector<Texture> diffuse_maps = load_material_textures(material, aiTextureType_DIFFUSE, "diffuse");
        textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

        vector<Texture> specular_maps = load_material_textures(material, aiTextureType_SPECULAR, "specular");
        textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
    }

    return Mesh(vertices, indices, textures);
}

vector<Texture> Model::load_material_textures(aiMaterial* mat, aiTextureType type, std::string type_name) {

    vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); ++i){
        // not a full path????
        aiString file_name_astr;
        mat->GetTexture(type, i, &file_name_astr);

        /*
        Note that we make the assumption that texture file paths in model files are local to the actual model object e.g. in the same directory as the location of the model itself. We can then simply concatenate the texture location string and the directory string we retrieved earlier (in the loadModel function) to get the complete texture path (that's why the GetTexture function also needs the directory string).
        */
        string file_name = string(file_name_astr.C_Str());  // I think just the file name????

        // optimization
        bool skip = false;
        // todo improve this.
        for(auto& loaded_tex : textures_loaded){
            if(file_name == loaded_tex.file_name){
                textures.push_back(loaded_tex);
                skip = true;
                break;
            }
        }
        if(!skip){
            string full_path = directory + "/" + file_name;
            Texture texture{full_path, type_name, file_name};
            textures.push_back(texture);
        }
    }
    return textures;
}
