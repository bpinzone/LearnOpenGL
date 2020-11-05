#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <vector>
#include <memory>
#include <functional>

class Gameobject;
class Material;
class Dir_light;

// Stores, starts, updates your game objects. Makes coordinating update order easier.
class Hierarchy {

public:

    // Rendered in this order: opaque_objects, skybox, transparent objects.
    std::vector<std::shared_ptr<Gameobject>> opaque_objects;
    std::shared_ptr<Gameobject> skybox;
    std::vector<std::shared_ptr<Gameobject>> transparent_objects;

    // Lights
    std::vector<std::shared_ptr<Dir_light>> dir_lights;

    void start();
    void update();

    void render_start(std::shared_ptr<Material> mat_override = nullptr);
    void render_update(std::shared_ptr<Material> mat_override = nullptr);

    void generate_depth_maps();

private:

    void for_each_in_hierarchy(std::function<void(std::shared_ptr<Gameobject>)> go_mem_func);

};

#endif