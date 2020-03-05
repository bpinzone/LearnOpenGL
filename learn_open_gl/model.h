#ifndef MODEL_H
#define MODEL_H

#include "shader.h"
#include "mesh.h"
#include "texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>  // gen smooth normals

#include <string>
#include <vector>

class Model {
public:

    Model(std::vector<Mesh> meshes_in);
    Model(Shader shader, const std::string& model_path);

    void draw();

private:

    void load_meshes(aiNode* node, const aiScene* scene, Shader shader);

    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;
};

#endif