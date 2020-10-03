#ifndef QUAD_POST_PROCESSOR_H
#define QUAD_POST_PROCESSOR_H

#include <memory>

class Material;

// Draws a single, full screen quad to the specified framebuffer using the provided material.
class Quad_post_processor {

public:
    Quad_post_processor(unsigned int _framebuffer_id, std::shared_ptr<Material> _material);

    void render_quad();

private:

    unsigned int framebuffer_id;
    std::shared_ptr<Material> material;
};

#endif