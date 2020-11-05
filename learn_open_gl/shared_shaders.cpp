#include "shared_shaders.h"

#include "shader.h"
#include "dir_light_depth_material.h"

using std::shared_ptr;
using std::make_shared;

Shared_shaders& Shared_shaders::get_instance(){
    static Shared_shaders ss;
    return ss;
}

Shared_shaders::Shared_shaders(){
    geo_pass_shader = make_shared<Shader>(
        "./shaders/geom_pass.vert", "./shaders/geom_pass.frag");

    auto dir_light_depth_shader = make_shared<Shader>(
        "./shaders/dir_light_depth.vert", "./shaders/dir_light_depth.frag");

    dir_light_depth_material = make_shared<Dir_light_depth_material>(
        dir_light_depth_shader, Material::Textures_t{});
}