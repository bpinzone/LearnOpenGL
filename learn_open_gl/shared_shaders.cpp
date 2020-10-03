#include "shared_shaders.h"

#include "shader.h"

using std::shared_ptr;
using std::make_shared;

Shared_shaders& Shared_shaders::get_instance(){
    static Shared_shaders ss;
    return ss;
}

Shared_shaders::Shared_shaders(){
    geo_pass_shader = make_shared<Shader>(
        "./shaders/geom_pass.vert", "./shaders/geom_pass.frag");

    instance_geo_pass_shader = make_shared<Shader>(
        "./shaders/instance_geom_pass.vert", "./shaders/instance_geom_pass.frag");
}