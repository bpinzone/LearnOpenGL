#include "vis_normal_renderer.h"

#include "game_object.h"
#include "transform.h"

using std::shared_ptr;
using std::make_shared;


Vis_normal_renderer::Vis_normal_renderer(shared_ptr<Model> model_in)
    : Model_renderer{model_in}, material{model_in->get_material()}{
}

void Vis_normal_renderer::update() {

    static const shared_ptr<Material> vis_normal_material = make_shared<Material>(
        make_shared<Shader>(
            "./shaders/vis_normals.vert",
            "./shaders/vis_normals.frag",
            "./shaders/vis_normals.geom"
        ),
        Material::Textures_t{}
    );

    Component::update();
    game_object->get_transform().load_into_shader_global();

    model->draw();
    model->set_materials(vis_normal_material);

    model->draw();
    model->set_materials(material);
}
