#ifndef MATERIAL_H
#define MATERIAL_H

#include "shader.h"
#include "texture.h"

#include <glm/glm.hpp>
#include <assimp/scene.h>

#include <vector>
#include <string>
#include <memory>


/*
Consists of two things generally.
(1) A shader
(2) All the uniform data the material is responsible for loading into the shader.
    Texture data
    Other uniforms

A material can have however many textures you want, but it should match the shader used.
This material will decide on an arbitrary texture slot mapping and keep it consistent.
*/
class Material {
public:

    using Textures_t = std::vector<std::shared_ptr<Texture>>;

    Material(
        std::shared_ptr<Shader> shader_in,
        const Textures_t& textures_in
    );

    Material(
        std::shared_ptr<Shader> shader_in,
        aiMaterial* mat,
        const std::string& model_dir
    );

    /*
    Calls use on the shader.
    Binds our textures to the appropriate texture units.
    Puts any uniform data we are responsible for into the shader.
        INCLUDES texture unit assignments! Other materials may have different unit orders for names!
    */
    void use();
    void use_instance_variant();

    std::shared_ptr<Shader> get_shader(){
        return shader;
    }

    // Class may be extended later.
    virtual ~Material() = default;

protected:

    // NOTE for derived classes:
    // If you have additional texture uniforms, pass them to material ctor.
    // If you have additional non-texture uniforms, override this to populate them, calling base version first.
    virtual void populate_uniforms();

    // Derived classes need to be able to set uniforms.
    std::shared_ptr<Shader> shader;

private:

    void assign_texture_unit_uniforms();

    void bind_textures_to_units();

    void load_textures(aiMaterial* mat, const std::string& model_dir);

    void load_textures_of_type(
        aiMaterial* mat, aiTextureType ai_type,
        const std::string& sampler_base_identifier,
        const std::string& model_dir);

    // Idx = Tex unit it will be mapped to.
    Textures_t textures;
};

#endif
