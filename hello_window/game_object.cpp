#include "game_object.h"


Gameobject::Gameobject(Material material_in, Mesh mesh_in, const glm::mat4& model_in)
    : material{material_in}, mesh{mesh_in}, model{model_in} {

}

void Gameobject::draw(const glm::mat4& view, const glm::mat4& projection) const {

    material.use(view, projection); // sets up shading.
    mesh.use();  // binds vao

    material.s.set_mat4fv("model", model);
    glDrawElements(GL_TRIANGLES, mesh.get_indices_size(), GL_UNSIGNED_INT, 0);
    // Another style not using EBO's : glDrawArrays(GL_TRIANGLES, 0, 36);
}