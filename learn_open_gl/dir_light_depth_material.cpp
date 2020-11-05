#include "dir_light_depth_material.h"

Dir_light_depth_material::Dir_light_depth_material(
        std::shared_ptr<Shader> shader_in,
        const Material::Textures_t& textures_in)

    : Material{shader_in, textures_in}{
}

Dir_light_depth_material::Dir_light_depth_material(
        std::shared_ptr<Shader> shader_in,
        aiMaterial* mat,
        const std::string& model_dir)

    : Material{shader_in, mat, model_dir}{
}

void Dir_light_depth_material::populate_uniforms(){

    Material::populate_uniforms();
    shader->set_mat4fv("world_space_to_light_clip", world_space_to_light_clip);
}