#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm.hpp>
#include <vector>
#include <string>

#include "shader.h"
#include "texture.h"

struct Texture_uniform_assignment {
    std::string uniform_name;
    Texture texture;
};

class Material {
public:

    Material(
        const Shader& s_in,
        const std::vector<Texture_uniform_assignment>& texture_assignments_in
    );
    void use() const;

    Shader s;

private:

    // Mapped to the texture unit corresponding to it's index.
    const std::vector<Texture_uniform_assignment> texture_assignments;
};

#endif