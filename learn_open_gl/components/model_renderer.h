#ifndef MODEL_RENDERER_H
#define MODEL_RENDERER_H

#include "component.h"
#include "render_component.h"
#include "../model.h"

#include <memory>


class Model_renderer : public Render_component {

public:

    Model_renderer(std::shared_ptr<Model> model_in);

    void update() override;

protected:

    std::shared_ptr<Model> model;
};

#endif
