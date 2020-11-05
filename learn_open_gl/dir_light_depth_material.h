#ifndef DIR_LIGHT_DEPTH_MATERIAL_H
#define DIR_LIGHT_DEPTH_MATERIAL_H

#include "material.h"

#include <glm/glm.hpp>

#include <memory>
#include <string>

class Shader;
class aiMaterial;

class Dir_light_depth_material : public Material {

public:

    Dir_light_depth_material(
        std::shared_ptr<Shader> shader_in,
        const Material::Textures_t& textures_in);

    Dir_light_depth_material(
        std::shared_ptr<Shader> shader_in,
        aiMaterial* mat,
        const std::string& model_dir);

    void set_world_space_to_light_clip(const glm::mat4& wstlc){
        world_space_to_light_clip = wstlc;
    }

protected:

    void populate_uniforms() override;

private:

    glm::mat4 world_space_to_light_clip;

};


#endif