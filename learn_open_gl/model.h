#ifndef MODEL_H
#define MODEL_H

#include "shader.h"
#include "mesh.h"
#include "texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>

class Model {
public:

    Model(const std::string& path);
    void draw(Shader shader);

private:

    std::vector<Mesh> meshes;
    std::string directory;

    void load_model(const std::string& path);
    void process_node(aiNode* node, const aiScene* scene);
    Mesh process_mesh(aiMesh* mesh, const aiScene* scene);

    // type_name is "diffuse" or "specular"
    std::vector<Texture> load_material_textures(aiMaterial* mat, aiTextureType type, std::string type_name);

    std::vector<Texture> textures_loaded;
};

#endif