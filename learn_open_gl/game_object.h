#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "components/transform.h"
#include "components/component.h"
#include "components/render_component.h"

#include <glm/glm.hpp>

#include <vector>
#include <optional>
#include <memory>

class Gameobject : public std::enable_shared_from_this<Gameobject> {
public:

    Gameobject();

    Transform& get_transform();

    void add_component(std::shared_ptr<Component> new_component);

    template<typename T>
    std::shared_ptr<T> get_component() {

        for(auto& comp : components){
            if(auto p = std::dynamic_pointer_cast<T>(comp); p){
                return p;
            }
        }
        for(auto& comp : render_components){
            if(auto p = std::dynamic_pointer_cast<T>(comp); p){
                return p;
            }
        }
        return nullptr;
    }

    // Calls start / update on non-render components.
    void start();
    void update();

    // Calls start / update on render components.
    void render_start();
    void render_update();

private:

    Transform transform;

    std::vector<std::shared_ptr<Component>> components;
    std::vector<std::shared_ptr<Render_component>> render_components;
};

#endif
