#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "component.h"

// Only exists for the sake of having a base class of all components that actually draw.
// Still abstract
class Render_component : public Component {

public:

protected:
    // make abstract
    Render_component(){
    }

private:

};


#endif