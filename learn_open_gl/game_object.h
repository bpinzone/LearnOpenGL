#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "transform.h"
#include "component.h"

#include <glm.hpp>

#include <vector>
#include <optional>
#include <memory>

class Gameobject : public std::enable_shared_from_this<Gameobject> {
public:

    Gameobject();
    Gameobject(const glm::mat4& transform_in);

    Transform& get_transform();

    // TODO: Sets new_component's game_object to be this.
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
