#ifndef LIGHT_H
#define LIGHT_H

#include <vector>

// TODO: probably incomplete.

#include "../component.h"

class Hierarchy;

/*
Abstract

Derived classes will hold parameters like color, attenuation, etc.
    Provide getters for material to call.
    Uses position from game object transform.
    Uses its own direction variable instead of game object rotation. TODO: change this.
*/

// TODO: lighting volumes.

// TODO: make sure lights are updated after all the other objects.

class Light : public Component {

public:

    // Generate this light's depth map based on this hierarchy.
    // Objects in the hierarchy should already be updated.
    virtual void generate_depth_map_from(Hierarchy* hier) = 0;

    // Size will be one for a directional and spot lights. 6 for point lights.
    // Light does not construct its own Texture object because it doesn't know the base name.
    virtual std::vector<unsigned int> get_texture_ids() = 0;

    virtual ~Light() = default;

};

#endif