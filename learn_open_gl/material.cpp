#include "material.h"

#include <glad/glad.h>
#include <glfw3.h>

using std::vector;
using std::string;
using std::shared_ptr;
using std::make_shared;

Material::Material(
        shared_ptr<Shader> shader_in,
        const Textures_t& textures_in)
    : shader{shader_in}, textures{textures_in} {
}

Material::Material(
        shared_ptr<Shader> shader_in,
        aiMaterial* mat,
        const std::string& model_dir)
    : shader{shader_in} {

    load_textures(mat, model_dir);
}

void Material::use() {
    shader->use();
    shader->forward_shader_globals_to_uniforms();
    assign_texture_unit_uniforms();

    // Would push any other uniform data into shader here...

    bind_textures_to_units();
}

// Set the shader's uniform variables to the correct texture unit number.
void Material::assign_texture_unit_uniforms() {

    // Sampler names are diffuse1, diffuse2..., specular1, specular2...
    // Map sampler names to texture units.
    int diffuse_seen = 0;
    int specular_seen = 0;
    int cube_seen = 0;
    int next_tex_unit_to_assign = 0;
    for(const auto& texture_sp : textures){
        string uni_sampler_name{"material."};
        switch(texture_sp->type){
            case Texture::Type::DIFFUSE:
                ++diffuse_seen;
                uni_sampler_name += "diffuse" + std::to_string(diffuse_seen);
                break;
            case Texture::Type::SPECULAR:
                ++specular_seen;
                uni_sampler_name += "specular" + std::to_string(specular_seen);
                break;
            case Texture::Type::CUBE:
                ++cube_seen;
                uni_sampler_name += "cube" + std::to_string(cube_seen);
                break;
            default: assert(false);
        }
        shader->set_int(uni_sampler_name, next_tex_unit_to_assign);
        ++next_tex_unit_to_assign;
    }
}

// Put our textures into the appropriate texture unit.
void Material::bind_textures_to_units(){
    int texture_unit = 0;
    for(const auto& texture : textures){
        // Populate (numbered) texture units with desired textures.
        // Activate unit, then bind the texture to it. Repeat.
        glActiveTexture(GL_TEXTURE0 + texture_unit);
        glBindTexture(GL_TEXTURE_2D, texture->texture_id);
        ++texture_unit;
    }

    // After activating a texture unit, a subsequent glBindTexture call will bind that texture to the currently active texture unit. Texture unit GL_TEXTURE0 is always by default activated, so we didn't have to activate any texture units in the previous example when using glBindTexture.
}

void Material::load_textures(aiMaterial* mat, const std::string& model_dir){
    load_textures_of_type(mat, Texture::Type::DIFFUSE, model_dir);
    load_textures_of_type(mat, Texture::Type::SPECULAR, model_dir);
}

// Requires: model_dir has trailing "/"
void Material::load_textures_of_type(
        aiMaterial* mat,
        Texture::Type type,
        const std::string& model_dir){

    aiTextureType ai_type;
    switch(type){
        case Texture::Type::DIFFUSE:
            ai_type = aiTextureType_DIFFUSE;
            break;
        case Texture::Type::SPECULAR:
            ai_type = aiTextureType_SPECULAR;
            break;
        default: assert(false);
    }

    for(unsigned int i = 0; i < mat->GetTextureCount(ai_type); ++i){

        // Assumes this path is relative to the model file?
        /*
        Note that we make the assumption that texture file paths in model files are local to the actual model object e.g. in the same directory as the location of the model itself. We can then simply concatenate the texture location string and the directory string we retrieved earlier (in the loadModel function) to get the complete texture path (that's why the GetTexture function also needs the directory string).
        */
        aiString ai_path;
        mat->GetTexture(ai_type, i, &ai_path);
        string texture_path = model_dir + string(ai_path.C_Str());

        auto tex_it = Texture::loaded_textures.find(texture_path);
        if(tex_it != Texture::loaded_textures.end()){
            // We already loaded this.
            textures.push_back(*tex_it);
        }
        else{
            // Load it now.
            shared_ptr<Texture> texture = make_shared<Texture>(texture_path, type, true);
            Texture::loaded_textures.insert(texture);
            textures.push_back(texture);
        }
    }
}
