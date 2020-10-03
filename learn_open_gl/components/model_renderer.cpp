#include "model_renderer.h"

#include "../game_object.h"
#include "../shader_globals.h"

using std::shared_ptr;

Model_renderer::Model_renderer(shared_ptr<Model> model_in)
    : model{model_in}{
}

// Draw the model.
void Model_renderer::update() {
    game_object->get_transform().load_into_shader_global();
    model->draw();
}
