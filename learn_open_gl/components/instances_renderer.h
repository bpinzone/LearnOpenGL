#ifndef INSTANCES_RENDERER_H
#define INSTANCES_RENDERER_H

#include "render_component.h"
#include "../game_object.h"
#include "../model.h"

#include <glm/glm.hpp>

#include <memory>
#include <vector>

// Acts as the renderer for several game objects. Instances should not have a renderer.
// Instances are still responsible for updating THEMSELVES, BEFORE this is updated.
// Instances_renderer will extract data from the instances as necessary.
class Instances_renderer : public Render_component {

public:

    using Instance_t = std::shared_ptr<Gameobject>;

    Instances_renderer(std::shared_ptr<Model> common_model_in, std::vector<Instance_t> instances_in);

    void update() override;

private:

    struct Instance_data {
        glm::mat4 model;
        glm::mat3 normal;
    };

    std::shared_ptr<Model> common_model;

    std::vector<Instance_t> instances;

    unsigned int instance_vbo;

    // Stage data here first during update to avoid many calls to Buffer Sub Data.
    std::vector<Instance_data> instance_vbo_staging;

};


#endif
