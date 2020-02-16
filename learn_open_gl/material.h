#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm.hpp>

#include "shader.h"
#include "texture.h"

class Material {
public:

    Material(const Texture& t1_in, const Texture& t2_in, const Shader& s_in);
    void use(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const;

private:

    Shader s;
    Texture t1;
    Texture t2;
};

#endif