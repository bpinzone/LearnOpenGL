#include "shader_globals.h"

Shader_globals& Shader_globals::get_instance() {
    static Shader_globals sg;
    return sg;
}

void Shader_globals::set_model(const mat4& model_in) {
    model = model_in;
}
void Shader_globals::set_view(const mat4& view_in) {
    view = view_in;
}
void Shader_globals::set_projection(const mat4& projection_in) {
    projection = projection_in;
}

void Shader_globals::set_normal(const mat4& normal_in) {
    normal = normal_in;
}

const glm::mat4& Shader_globals::get_model() const {
    return model;
}
const glm::mat4& Shader_globals::get_view() const {
    return view;
}
const glm::mat4& Shader_globals::get_projection() const {
    return projection;
}
const glm::mat4& Shader_globals::get_normal() const {
    return normal;
}

Shader_globals::Shader_globals() {

}
