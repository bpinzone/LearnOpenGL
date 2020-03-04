#include "game_object.h"
#include "shader_globals.h"

#include <random>
#include <functional>

Gameobject::Gameobject(Material material_in, Mesh mesh_in, const glm::mat4& model_in)
    : material{material_in}, mesh{mesh_in} {

    set_model(model_in);

    static auto random_float = std::bind(
        std::uniform_real_distribution<float>{0.0, 1.0},
        std::default_random_engine{}
    );

    rotation_axis = glm::vec3{random_float(), random_float(), random_float()};
    rotation_speed = random_float() * 1;
}

void Gameobject::draw() const {

    // Shader_globals::get_instance().set_model(model);
    // Shader_globals::get_instance().set_normal(normal);

    // material.use(); // sets up shading.
    // mesh.use();  // binds vao

    // glDrawElements(GL_TRIANGLES, mesh.get_indices_size(), GL_UNSIGNED_INT, 0);
}

void Gameobject::update(float delta_time) {

    set_model(glm::rotate(
        model, rotation_speed * delta_time,
        rotation_axis
    ));
}

void Gameobject::set_model(const glm::mat4 model_in) {
    model = model_in;
    normal = glm::mat3(glm::transpose(glm::inverse(model)));
}
