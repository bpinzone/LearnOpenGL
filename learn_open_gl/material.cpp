#include "material.h"

#include <glad/glad.h>
#include <glfw3.h>

using std::vector;
using std::string;

Material::Material(
        Shader shader_in,
        const std::vector<Texture>& textures_in)
    : shader{shader_in}, textures{textures_in} {
    assign_textures_to_units();
}

Material::Material(
        Shader shader_in,
        aiMaterial* mat,
        const std::string& model_dir,
        std::vector<Texture>& loaded_textures)
    : shader{shader_in} {

    load_textures(mat, model_dir, loaded_textures);
    assign_textures_to_units();
}

void Material::use() const {

    shader.use();

    int texture_unit = 0;
    for(const auto& texture : textures){
        // Populate (numbered) texture units with desired textures.
        // Activate unit, then bind the texture to it. Repeat.

        // Normally arg is "GL_TEXTUREi"
        glActiveTexture(GL_TEXTURE0 + texture_unit);
        glBindTexture(GL_TEXTURE_2D, texture.texture_id);
        ++texture_unit;
    }

    // After activating a texture unit, a subsequent glBindTexture call will bind that texture to the currently active texture unit. Texture unit GL_TEXTURE0 is always by default activated, so we didn't have to activate any texture units in the previous example when using glBindTexture.
}

void Material::assign_textures_to_units() {

    glUseProgram(shader.program_id);

    // Sampler names are diffuse1, diffuse2..., specular1, specular2...
    // Map sampler names to texture units.
    int diffuse_seen = 0;
    int specular_seen = 0;
    int next_tex_unit_to_assign = 0;
    for(const auto& texture : textures){
        string uni_sampler_name{"material."};
        switch(texture.type){
            case Texture::Type::DIFFUSE:
                ++diffuse_seen;
                uni_sampler_name += "diffuse" + std::to_string(diffuse_seen);
                break;
            case Texture::Type::SPECULAR:
                ++specular_seen;
                uni_sampler_name += "specular" + std::to_string(specular_seen);
                break;
        }
        shader.set_int(uni_sampler_name, next_tex_unit_to_assign);
        ++next_tex_unit_to_assign;
    }
}

void Material::load_textures(
        aiMaterial* mat,
        const std::string& model_dir,
        std::vector<Texture>& loaded_textures){

    load_textures_of_type(mat, Texture::Type::DIFFUSE, model_dir, loaded_textures);
    load_textures_of_type(mat, Texture::Type::SPECULAR, model_dir, loaded_textures);
}

// Requires: model_dir has trailing "/"
void Material::load_textures_of_type(
        aiMaterial* mat,
        Texture::Type type,
        const std::string& model_dir,
        std::vector<Texture>& loaded_textures) {

    aiTextureType ai_type;
    switch(type){
        case Texture::Type::DIFFUSE:
            ai_type = aiTextureType_DIFFUSE;
            break;
        case Texture::Type::SPECULAR:
            ai_type = aiTextureType_SPECULAR;
            break;
        default: return;
    }

    for(unsigned int i = 0; i < mat->GetTextureCount(ai_type); ++i){

        // Assumes this path is relative to the model file?
        /*
        Note that we make the assumption that texture file paths in model files are local to the actual model object e.g. in the same directory as the location of the model itself. We can then simply concatenate the texture location string and the directory string we retrieved earlier (in the loadModel function) to get the complete texture path (that's why the GetTexture function also needs the directory string).
        */
        aiString ai_path;
        mat->GetTexture(ai_type, i, &ai_path);
        string texture_path = model_dir + string(ai_path.C_Str());

        bool skip = false;
        for(const auto& loaded_texture : loaded_textures){
            if(loaded_texture.path == texture_path){
                textures.push_back(loaded_texture);
                skip = true;
                break;
            }
        }
        if(!skip){
            Texture texture{texture_path, type};
            loaded_textures.push_back(texture);
            textures.push_back(texture);
        }
    }
}
