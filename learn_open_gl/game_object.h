#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "components/transform.h"
#include "components/component.h"

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
        if(auto p = std::dynamic_pointer_cast<T>(render_component); p){
            return p;
        }
        return nullptr;
    }

    // Calls start and update on all components.
    // Models get called last.
    void start();
    void update();

private:

    Transform transform;

    std::vector<std::shared_ptr<Component>> components;
    std::shared_ptr<Component> render_component;

};

#endif
