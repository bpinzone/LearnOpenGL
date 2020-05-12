// #include <glfw3.h> // do not include this apparently.

#include "outline_model_renderer.h"
#include "transform.h"
#include "game_object.h"

using std::shared_ptr;

Outline_model_renderer::Outline_model_renderer(
        shared_ptr<Model> model_in,
        shared_ptr<Material> outline_material_in)

    : Model_renderer{model_in}, outline_material{outline_material_in} {

    material = model_in->get_material();
}

void Outline_model_renderer::update() {

    Component::update();

    static const int base_flag = 1;

    // BASE
    glClear(GL_STENCIL_BUFFER_BIT);
    glStencilFunc(GL_ALWAYS, base_flag, 0xFF);
    glStencilMask(0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    game_object->get_transform().load_into_shader_global();
    model->draw();

    // OUTLINE
    // Stencil settings
    glStencilFunc(GL_NOTEQUAL, base_flag, 0xFF);
    glStencilMask(0x00);

    // scale
    const glm::vec3 original_scale = game_object->get_transform().get_scale();
    const glm::vec3 larger_scale = original_scale * 1.1f;

    // Prep
    // Dont' disable depth test since I am stenciling per-object, not per group of highlighted objects.
    // glDisable(GL_DEPTH_TEST);
    model->set_materials(outline_material);
    game_object->get_transform().set_scale(larger_scale);

    // draw
    game_object->get_transform().load_into_shader_global();
    model->draw();

    // revert
    glEnable(GL_DEPTH_TEST);
    model->set_materials(material);
    game_object->get_transform().set_scale(original_scale);

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

}
