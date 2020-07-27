#include "shader_globals.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

void Shader_globals::set_model(const mat4& model_in) {
    model = model_in;
}
void Shader_globals::set_view(const mat4& view_in) {
    view = view_in;
    // matrices point of maintenance.
    // Send to buffer
    glBindBuffer(GL_UNIFORM_BUFFER, matrices_ubo);
    const int offset = 0;
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void Shader_globals::set_projection(const mat4& projection_in) {
    projection = projection_in;
    // matrices point of maintenance.
    // Send to buffer
    glBindBuffer(GL_UNIFORM_BUFFER, matrices_ubo);
    const int offset = sizeof(glm::mat4);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
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

Shader_globals::Shader_globals(){

    glGenBuffers(1, &matrices_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, matrices_ubo);

    // Allocate the buffer
    // matrices point of maintenance.
    const size_t c_buffer_size = 2 * sizeof(glm::mat4);
    glBufferData(GL_UNIFORM_BUFFER, c_buffer_size, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Link range of buffer to binding point. (In this case, the whole range)
    glBindBufferRange(GL_UNIFORM_BUFFER, c_matrices_uniform_block_binding_point, matrices_ubo, 0, c_buffer_size);

}
