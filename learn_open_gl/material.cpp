#include "material.h"

#include <glad/glad.h>
#include <glfw3.h>

using std::vector;
using std::string;

Material::Material(
        const Shader& s_in,
        const vector<Texture_uniform_assignment>& texture_assignments_in)
    : s{s_in}, texture_assignments{texture_assignments_in} {

    glUseProgram(s.program_id);

    int texture_unit = 0;
    for(const auto& texture_assignment : texture_assignments){
        // First iter, map <uniform name> sampler variable to texture unit 0.
        s.set_int(texture_assignment.uniform_name, texture_unit);
        ++texture_unit;
    }
}

void Material::use() const {

    s.use();

    int texture_unit_offset = 0;
    for(const auto& texture_assignment : texture_assignments){
        // Populate (numbered) texture units with desired textures.
        // Activate unit, then bind the texture to it. Repeat.

        // Normally arg is "GL_TEXTUREi"
        glActiveTexture(GL_TEXTURE0 + texture_unit_offset);
        glBindTexture(GL_TEXTURE_2D, texture_assignment.texture.texture_id);
        ++texture_unit_offset;
    }

    // After activating a texture unit, a subsequent glBindTexture call will bind that texture to the currently active texture unit. Texture unit GL_TEXTURE0 is always by default activated, so we didn't have to activate any texture units in the previous example when using glBindTexture.
}