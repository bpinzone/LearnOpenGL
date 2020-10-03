#ifndef GEOM_PASS_H
#define GEOM_PASS_H

#include <memory>

#include "utility.h"

class Hierarchy;
class Texture;

// Can probably do efficient TBN. I think the point is, you need the multiplies, but do them in the vertex shader instead of the fragment shader???

// Overall design of deferred shading:
// All objects should have a shader with the same interface - Giving the g-pass what it needs.
// Having an emissive texture will allow more flexibility for unlit effects.
// No anti-aliasing...

// Then the lighting pass will take these textures and render its lighting volumes. Except for emissive it will just be a quad instead of a volume.
// CAREFUL with the lighting pass, for example, a potentially lit pixel that is BEHIND the light volume. STILL need to consider distance to the light.

/*
Holds Framebuffer object and all the textures for the geometry pass.
Can render a hierarchy, requiring that ALL the objects in the hierarchy use a valid variant of a g-pass shader.
Provides textures for each of the geometry textures when finished.
    (so the lighting pass can take themextern
*/
class Geom_pass {

public:
    Geom_pass();
    // TODO: dtor.

    // Requires: All objects in hierarchy must be using a fragment shader which writes appropriately to the g buffer.
    // Requires: Shader_global's view and projection are already set.
    void bind_framebuffer_and_render_start(Hierarchy* hier);

    // Requires: All objects in hierarchy must be using a fragment shader which writes appropriately to the g buffer.
    // Requires: Shader_global's view and projection are already set.
    void bind_framebuffer_and_render_update(Hierarchy* hier);

    std::shared_ptr<Texture> get_position_tex() const;
    std::shared_ptr<Texture> get_normal_tex() const;
    std::shared_ptr<Texture> get_albedo_spec_tex() const;

private:

    void bind_framebuffer_and_clear();

    unsigned int g_framebuffer_id;

    // Color attachments
    // In world space.
    std::shared_ptr<Texture> g_position_tex;
    std::shared_ptr<Texture> g_normal_tex;

    std::shared_ptr<Texture> g_albedo_spec_tex;

    // Depth and stencil attachment
    unsigned int g_renderbuffer_id;
};

#endif