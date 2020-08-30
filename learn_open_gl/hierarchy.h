#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <vector>
#include <memory>

class Gameobject;

// Stores, starts, updates your game objects. Makes coordinating update order easier.
class Hierarchy {

public:

    // Rendered in this order: opaque_objects, skybox, transparent objects.
    std::vector<std::shared_ptr<Gameobject>> opaque_objects;
    std::shared_ptr<Gameobject> skybox;
    std::vector<std::shared_ptr<Gameobject>> transparent_objects;

    void start();
    void update();

private:

};

#endif