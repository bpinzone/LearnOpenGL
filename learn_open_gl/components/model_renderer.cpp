#include "model_renderer.h"

#include "../game_object.h"
#include "../shader_globals.h"
#include "../material.h"

using std::shared_ptr;

Model_renderer::Model_renderer(shared_ptr<Model> model_in)
    : model{model_in}{
}

// Draw the model.
void Model_renderer::render_update(shared_ptr<Material> mat_override) {
    game_object->get_transform().load_into_shader_global();
    model->draw(mat_override);
}
