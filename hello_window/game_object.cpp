#include "game_object.h"

#include <random>
#include <functional>

Gameobject::Gameobject(Material material_in, Mesh mesh_in, const glm::mat4& model_in)
    : material{material_in}, mesh{mesh_in}, model{model_in} {

    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution{};
    auto random_double = std::bind(distribution, generator);

    rotation_axis = glm::vec3{
        random_double(), random_double(), random_double()
    };
    rotation_speed = random_double() * 5;

}

void Gameobject::draw(const glm::mat4& view, const glm::mat4& projection) const {

    material.use(view, projection); // sets up shading.
    mesh.use();  // binds vao

    material.s.set_mat4fv("model", model);
    glDrawElements(GL_TRIANGLES, mesh.get_indices_size(), GL_UNSIGNED_INT, 0);
    // Another style not using EBO's : glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Gameobject::update(float delta_time) {

    model = glm::rotate(
        model, rotation_speed * delta_time,
        rotation_axis
    );
}