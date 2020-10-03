#ifndef LIGHT_H
#define LIGHT_H

#include <memory>

// TODO: probably incomplete.

#include "../component.h"

class Hierarchy;
class Shader;

/*
Abstract

Holds position, orientation, lighting params (color, size, attenuation),
and depth maps for a light source.

Generates depth maps from an object hierarchy.

Loads lighting parameters and depth maps into a lighting pass shader.
*/

// TODO: make sure lights are updated after all the other objects.
// Maybe gonna have to separate render updating and regular component updating?
// Well, probably not, because the geometry pass will be extremely simple.

// *** So Model_renderer's will only be involved in the geometry pass, and thats fine if it happens before shadow maps are generated!
// so yeah, just make sure lights are updated last.
class Light : public Component {

public:

    // Generate this light's depth map based on this hierarchy.
    // Objects in the hierarchy should already be updated.
    virtual void generate_depth_map_from(Hierarchy* hier) = 0;

    // Forward my lighting parameters to the appropriate uniform.
    // Will use the type and light_type_arr_idx.
    virtual void forward_lighting_params_to_shader_uniforms(std::shared_ptr<Shader> shader) = 0;

    // Will use the type and light_type_arr_idx.
    virtual void assign_texture_unit_uniform(std::shared_ptr<Shader> shader) = 0;

    virtual void bind_texture_to_unit() = 0;

    virtual ~Light() = default;

};


#endif