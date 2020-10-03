#include "game_object.h"
#include "shader_globals.h"
#include "components/component.h"
#include "components/render_component.h"

// #include <glfw3.h>
#include <glm/glm.hpp>

#include <random>
#include <cmath>
#include <functional>
#include <algorithm>

using std::shared_ptr;
using std::for_each;

Gameobject::Gameobject(){
}

Transform& Gameobject::get_transform(){
    return transform;
}

void Gameobject::add_component(shared_ptr<Component> new_component){

    new_component->set_game_object(shared_from_this());

    if(auto p = std::dynamic_pointer_cast<Render_component>(new_component); p){
        render_components.push_back(p);
    }
    else{
        components.push_back(new_component);
    }
}

void Gameobject::start(){
    for(auto& c : components) { c->start(); };
}

void Gameobject::update(){
    for(auto& c : components) { c->update(); };
}

void Gameobject::render_start(){
    for(auto& c : render_components) { c->start(); };
}
void Gameobject::render_update(){
    for(auto& c : render_components) { c->update(); };
}
