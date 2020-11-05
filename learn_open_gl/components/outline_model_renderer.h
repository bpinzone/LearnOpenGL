#ifndef OUTLINE_MODEL_RENDERER_H
#define OUTLINE_MODEL_RENDERER_H

#include "component.h"
#include "../model.h"
#include "../material.h"
#include "model_renderer.h"

#include <memory>

class Material;

class Outline_model_renderer : public Model_renderer {

public:

    // Requires: model_in must already have the desired base (non outline) material set.
    Outline_model_renderer(
        std::shared_ptr<Model> model_in,
        std::shared_ptr<Material> outline_material_in
    );

    void render_update(std::shared_ptr<Material> mat_override = nullptr) override;

private:
    std::shared_ptr<Material> material;
    std::shared_ptr<Material> outline_material;
};

#endif
