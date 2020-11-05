#ifndef VIS_NORMAL_RENDERER_H
#define VIS_NORMAL_RENDERER_H

#include "model_renderer.h"
#include "../model.h"
#include "../material.h"

#include <memory>

class Material;

class Vis_normal_renderer : public Model_renderer {

public:
    // Requires: the model already has its material set.
    Vis_normal_renderer(std::shared_ptr<Model> model_in);

    void render_update(std::shared_ptr<Material> mat_override = nullptr) override;

private:
    std::shared_ptr<Material> material;
};


#endif
