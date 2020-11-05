#ifndef DIR_LIGHT_H
#define DIR_LIGHT_H

#include "light.h"

#include <memory>
#include <glm/glm.hpp>

class Shader;
class Hierarchy;
class Texture;

// TODO: Forget lighting volumes for now. Get shadows up.
// TODO: very incomplete.

class Dir_light : public Light  {

public:


    // TODO: add other parameters.
    Dir_light(
        const glm::vec3& _ambient, const glm::vec3& _diffuse,
        const glm::vec3& _specular, const glm::vec3& _direction);


    // Generate this light's depth map based on this hierarchy.
    // Objects in the hierarchy should already be updated.
    void generate_depth_map_from(Hierarchy* hier) override;

    // Size will be one for a directional and spot lights. 6 for point lights.
    std::vector<unsigned int> get_texture_ids() override;

private:

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 direction;

    unsigned int depth_map_fbo;

    // We do not construct the texture because we don't know the base name.
    unsigned int depth_map_texture_id;
};



#endif