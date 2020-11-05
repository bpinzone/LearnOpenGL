#include "dir_light.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

// TODO: get rid.
#include "../../utility.h"

#include "../../hierarchy.h"
#include "../../shader.h"
#include "../../utility.h"
#include "../../texture.h"
#include "../../game_object.h"
#include "../../shared_shaders.h"

#include "../../dir_light_depth_material.h"

using std::vector;

static constexpr int c_shadow_width = 1024;
static constexpr int c_shadow_height = 1024;

Dir_light::Dir_light(
        const glm::vec3& _ambient, const glm::vec3& _diffuse,
        const glm::vec3& _specular, const glm::vec3& _direction)

    : ambient{_ambient}, diffuse{_diffuse},
    specular{_specular}, direction{_direction} {

    glGenFramebuffers(1, &depth_map_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);

    glGenTextures(1, &depth_map_texture_id);
    glBindTexture(GL_TEXTURE_2D, depth_map_texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        c_shadow_width, c_shadow_height,
        0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D, depth_map_texture_id, 0);
    // Don't need a color buffer, explicitly tell opengl this, so the fbo is complete.
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Dir_light::generate_depth_map_from(Hierarchy* hier) {

    glViewport(0, 0, c_shadow_width, c_shadow_height);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);

    glClear(GL_DEPTH_BUFFER_BIT);

    // TODO: critical: configure shader and matrices.
    // TODO: Hoist this out and recompute only when needed:
    static constexpr float near_plane = 0.05f;
    static constexpr float far_plane = 2000.0f;
    static constexpr float width = 1000.0f;
    const glm::mat4 projection = glm::ortho(-width, width, -width, width, near_plane, far_plane);

    // const glm::vec3& pos = game_object->get_transform().get_position();

    const glm::vec3& pos = glm::vec3(0, 0, 20);

    static constexpr glm::vec3 y_axis{0, 1, 0};
    const glm::vec3 right = glm::cross(direction, y_axis);
    // const glm::mat4 view = glm::lookAt(
    //     pos, pos + direction,
    //     glm::normalize(glm::cross(right, direction))
    // );
    const glm::mat4 view = camera.GetViewMatrix();

    const glm::mat4 world_space_to_light_clip = projection * view;

    Shared_shaders::get_instance().dir_light_depth_material->set_world_space_to_light_clip(
        world_space_to_light_clip);

    hier->render_update(Shared_shaders::get_instance().dir_light_depth_material);

    glViewport(0, 0, window_width, window_height);
}

vector<unsigned int> Dir_light::get_texture_ids() {
    return {depth_map_texture_id};
}