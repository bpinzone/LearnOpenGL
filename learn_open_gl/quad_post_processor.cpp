#include "quad_post_processor.h"

#include <glad/glad.h>

#include <vector>

#include "material.h"
#include "mesh.h"
#include "manual_meshes.h"

using std::vector;
using std::shared_ptr;

Quad_post_processor::Quad_post_processor(
        unsigned int _framebuffer_id, shared_ptr<Material> _material)
    : framebuffer_id{_framebuffer_id}, material{_material}{
}

void Quad_post_processor::render_quad() {


    static Mesh quad_mesh{quad_vertices, quad_indices, nullptr};

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    if(quad_mesh.get_material() != material){
        quad_mesh.set_material(material);
    }
    quad_mesh.draw();
}