#include "material.h"

#include <glad/glad.h>
#include <glfw3.h>

Material::Material(const Texture& t1_in, const Texture& t2_in, const Shader& s_in)
    : t1{t1_in}, t2{t2_in}, s{s_in} {

    glUseProgram(s.program_id);
    s.set_int("texture1", 0); // map the texture1 sampler variable to texture unit 0.
    s.set_int("texture2", 1); // map the texture2 sampler variable to texture unit 1.

}

void Material::use(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const {

    s.use(model, view, projection);

    // Populate (numbered) texture units with desired textures.
    // Activate unit, then bind the texture to it. Repeat.
    // put t1 into texture unit 0.
    glActiveTexture(GL_TEXTURE0);  // Make GL_TEXTURE_2D refer to GL_TEXTURE0
    glBindTexture(GL_TEXTURE_2D, t1.texture_id);

    // put t2 into texture unit 1.
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, t2.texture_id);

    // After activating a texture unit, a subsequent glBindTexture call will bind that texture to the currently active texture unit. Texture unit GL_TEXTURE0 is always by default activated, so we didn't have to activate any texture units in the previous example when using glBindTexture.
}