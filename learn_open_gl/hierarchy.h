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

    void render_start();
    void render_update();

private:

    using Gameobject_fn_t = void (Gameobject::*)();

    void for_each_in_hierarchy(Gameobject_fn_t go_fn);

};

#endif