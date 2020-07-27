#ifndef OUTLINE_MODEL_RENDERER_H
#define OUTLINE_MODEL_RENDERER_H

#include "component.h"
#include "model.h"
#include "material.h"
#include "model_renderer.h"

#include <memory>

class Outline_model_renderer : public Model_renderer {

public:
    Outline_model_renderer(
        std::shared_ptr<Model> model_in,
        std::shared_ptr<Material> outline_material_in
    );

    void update() override;

private:
    std::shared_ptr<Material> material;
    std::shared_ptr<Material> outline_material;
};

#endif
