#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "component.h"

#include <memory>

class Material;

// Only exists for the sake of having a base class of all components that actually draw.
// Still abstract
class Render_component : public Component {

public:

    // TODO: Poor design. Render components need extra parameter for material override,
    // so they do not use regular component's start() and update()
    // Could add another base class, and Component and Render_component inherit from that.
    // But lets not for now.
    void start() final { }
    void update() final { }

    // Calls start / update on render components.
    // TODO: get rid of render start()'s. This notion doesn't really need to exist...
    virtual void render_start(std::shared_ptr<Material> mat_override = nullptr) {

    }

    virtual void render_update(std::shared_ptr<Material> mat_override = nullptr) = 0;

protected:
    // make abstract
    Render_component(){
    }

private:

};


#endif