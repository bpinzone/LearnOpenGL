#include "dir_light.h"

#include <glad/glad.h>

// TODO: extremely incomplete.

#include "../../hierarchy.h"
#include "../../shader.h"
#include "../../utility.h"

static constexpr int c_shadow_width = 1024;
static constexpr int c_shadow_height = 1024;

Dir_light::Dir_light(int _light_type_arr_idx, int _depth_map_texture_unit)
    : light_type_arr_idx{_light_type_arr_idx},
    depth_map_texture_unit{_depth_map_texture_unit} {

    glGenTextures(1, &depth_map_texture_id);
    glBindTexture(GL_TEXTURE_2D, depth_map_texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        c_shadow_width, c_shadow_height,
        0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenFramebuffers(1, &depth_map_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D, depth_map_texture_id, 0);
    // Don't need a color buffer, explicitly tell opengl this, so the fbo is complete.
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // TODO: any other stuff?

}

void Dir_light::generate_depth_map_from(Hierarchy* hier) {

    glViewport(0, 0, c_shadow_width, c_shadow_height);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);

    glClear(GL_DEPTH_BUFFER_BIT);
    hier->render_update();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, window_width, window_height);
}