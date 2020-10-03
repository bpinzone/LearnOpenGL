#include "material.h"

#include <glad/glad.h>

#include <map>

// I don't think this should be here.
// #include <glfw3.h>

using std::vector;
using std::map;
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

    // potential problem: when rendering the pp quad, these uniforms I'm setting will not exist! Does that matter???
    // No, its fine.
    // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml
    // If location is equal to -1, the data passed in will be silently ignored and the specified uniform variable will not be changed.

    shader->load_uniforms_with_shader_globals();

    // TODO: Don't have to do this every time???
    // I think don't have to do it every time, as long as shader member doesn't change.
    assign_texture_unit_uniforms();

    // Would push any other uniform data into shader here...

    bind_textures_to_units();
}

// Set the shader's uniform variables to the correct texture unit number.
void Material::assign_texture_unit_uniforms() {

    // Sampler names are diffuse1, diffuse2..., specular1, specular2...
    // Map sampler names to texture units.
    map<string, int> sampler_iden_count;
    int next_tex_unit_to_assign = 0;

    for(const auto& texture_sp : textures){

        string sampler_identifier = texture_sp->get_sampler_base_identifier();
        sampler_identifier += std::to_string(++sampler_iden_count[sampler_identifier]);

        shader->set_int(sampler_identifier, next_tex_unit_to_assign);
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
        glBindTexture(GL_TEXTURE_2D, texture->get_id());
        ++texture_unit;
    }

    // After activating a texture unit, a subsequent glBindTexture call will bind that texture to the currently active texture unit. Texture unit GL_TEXTURE0 is always by default activated, so we didn't have to activate any texture units in the previous example when using glBindTexture.
}

void Material::load_textures(aiMaterial* mat, const std::string& model_dir){
    load_textures_of_type(mat, aiTextureType_DIFFUSE, "material.diffuse", model_dir);
    load_textures_of_type(mat, aiTextureType_SPECULAR, "material.specular", model_dir);
}

// Requires: model_dir has trailing "/"
void Material::load_textures_of_type(
        aiMaterial* mat,
        aiTextureType ai_type,
        const string& sampler_base_identifier,
        const string& model_dir){

    for(unsigned int i = 0; i < mat->GetTextureCount(ai_type); ++i){

        // Assumes this path is relative to the model file?
        /*
        Note that we make the assumption that texture file paths in model files are local to the actual model object e.g. in the same directory as the location of the model itself. We can then simply concatenate the texture location string and the directory string we retrieved earlier (in the loadModel function) to get the complete texture path (that's why the GetTexture function also needs the directory string).
        */
        aiString ai_path;
        mat->GetTexture(ai_type, i, &ai_path);
        string texture_path = model_dir + string(ai_path.C_Str());

        shared_ptr<Texture> texture = Texture::construct_texture_from_img_file(
            texture_path, sampler_base_identifier, true);

        textures.push_back(texture);
    }
}
