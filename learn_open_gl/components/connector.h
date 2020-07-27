#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "game_object.h"
#include "component.h"

#include <memory>

// Assumes this is on a cube object.
// Transforms the cube to make it look like the two other game objects are connected.

// Assumes our update is called after our object's update functions.
class Connector : public Component {
public:

    using Connectee_t = std::shared_ptr<Gameobject>;

    Connector(Connectee_t c1, Connectee_t c2);

    void start() override;
    void update() override;

    void set_connected_objects(Connectee_t c1, Connectee_t c2);

private:

    void update_transform();

    Connectee_t connectee_1;
    Connectee_t connectee_2;

};

#endif
