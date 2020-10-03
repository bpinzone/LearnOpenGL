#ifndef DIR_LIGHT_H
#define DIR_LIGHT_H

// TODO: extremely incomplete.

#include "light.h"

#include <memory>
#include <glm/glm.hpp>

class Shader;
class Hierarchy;

// TODO: very incomplete.
// TOOD: perhaps becuase of things like skyboxes and other effects, we will need an emissive buffer as part of the G-buffer? Like diffuse, but it should be rendered as is, without regard to lighting.

// Points down its local -z axis. Rotate transform as desired.
// Design so has no effect on objects behind its local +z axis.
class Dir_light : public Light  {

public:

    // TODO: add other parameters.
    Dir_light(int _light_type_arr_idx, int _depth_map_texture_unit);

    // Assumes all non-render components of all game objects are finished updating.
    void generate_depth_map_from(Hierarchy* hier) override;

    // TODO: Implement this.
    // void forward_lighting_params_to_shader_uniforms(std::shared_ptr<Shader> shader) override;
    // void assign_texture_unit_uniform(std::shared_ptr<Shader> shader) override;
    // void bind_texture_to_unit() override;

    // TODO: decide what this should do. At least generate depth maps.
    // void update() override;

    // TODO: write code to render light volume for lighting pass.

private:

    unsigned int depth_map_fbo;
    unsigned int depth_map_texture_id;

    // Lighting volume data
    int near_plane;
    int far_plane;
    int width;  // length in local x.
    int height; // length in local y.

    glm::mat4 projection;

    // Access appropriate array in shader.
    // Shaders have an array for each type of light.
    // Use this to index into the array-of-your-type, in order to load your data.
    int light_type_arr_idx;

    // Forward to shader.
    int depth_map_texture_unit;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 direction;

    // TODO: more stuff here.
    // Derived from above.
    // glm::mat4 projection;

};



#endif