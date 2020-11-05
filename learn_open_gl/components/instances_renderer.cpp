#include "instances_renderer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../material.h"

#include <algorithm>

using std::transform;
using std::shared_ptr;

Instances_renderer::Instances_renderer(shared_ptr<Model> common_model_in, std::vector<Instance_t> instances_in)
    : common_model{common_model_in}, instances{instances_in} {

    instance_vbo_staging.resize(instances.size());

    glGenBuffers(1, &instance_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, instance_vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        instance_vbo_staging.size() * sizeof(decltype(instance_vbo_staging)::value_type),
        nullptr, GL_DYNAMIC_DRAW);

    common_model->bind_only_mesh_vao();
    int attrib_idx = common_model->get_only_mesh_next_avail_vert_attrib_idx();

    // One attribute for every column.

    // Setup mat4 instance_model attributes
    for(int col = 0; col < 4; ++col){
        glEnableVertexAttribArray(attrib_idx);
        glVertexAttribPointer(
            attrib_idx, 4, GL_FLOAT, GL_FALSE, sizeof(Instance_data),
            reinterpret_cast<void*>(offsetof(Instance_data, model) + (col * sizeof(glm::vec4))));
        glVertexAttribDivisor(attrib_idx, 1);
        ++attrib_idx;
    }

    // Setup mat3 instance_normal attributes
    for(int col = 0; col < 3; ++col){
        glEnableVertexAttribArray(attrib_idx);
        glVertexAttribPointer(
            attrib_idx, 3, GL_FLOAT, GL_FALSE, sizeof(Instance_data),
            reinterpret_cast<void*>(offsetof(Instance_data, normal) + (col * sizeof(glm::vec3))));
        glVertexAttribDivisor(attrib_idx, 1);
        ++attrib_idx;
    }
}

void Instances_renderer::render_update(shared_ptr<Material> mat_override) {

    // Put data for instances into staging area.
    transform(
        instances.begin(), instances.end(),
        instance_vbo_staging.begin(),
        [](const auto& instance){
            return Instance_data {
                instance->get_transform().get_model(),
                instance->get_transform().get_normal()
            };
        }
    );

    // Put into buffer.
    glBindBuffer(GL_ARRAY_BUFFER, instance_vbo);
    glBufferSubData(
        GL_ARRAY_BUFFER, 0,
        instance_vbo_staging.size() * sizeof(decltype(instance_vbo_staging)::value_type),
        instance_vbo_staging.data());

    common_model->draw_instanced(instances.size(), mat_override);
}
