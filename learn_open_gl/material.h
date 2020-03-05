#ifndef MATERIAL_H
#define MATERIAL_H

#include "shader.h"
#include "texture.h"

#include <glm.hpp>
#include <assimp/scene.h>

#include <vector>
#include <string>


// A shader and textures.
class Material {
public:

    // Constructor philosophy:
    // Give me what I need directly,
    // or a way to manually generate everything.

    Material(
        Shader shader_in,
        const std::vector<Texture>& textures_in
    );

    Material(
        Shader shader_in,
        aiMaterial* mat,
        const std::string& model_dir,
        std::vector<Texture>& loaded_textures
    );

    void use() const;

    Shader shader;

private:

    void assign_textures_to_units();

    void load_textures(aiMaterial* mat, const std::string& model_dir, std::vector<Texture>& loaded_textures);

    void load_textures_of_type(aiMaterial* mat, Texture::Type type, const std::string& model_dir, std::vector<Texture>& loaded_textures);

    // Idx = Tex unit it will be mapped to.
    std::vector<Texture> textures;
};

#endif