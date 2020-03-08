#include "game_object.h"
#include "shader_globals.h"
#include "component.h"
#include "model_renderer.h"

#include <glfw3.h>
#include <glm.hpp>

#include <random>
#include <cmath>
#include <functional>

using std::shared_ptr;

Gameobject::Gameobject()
    : Gameobject(glm::mat4(1)){
}

Gameobject::Gameobject(const glm::mat4& transform_in)
    : transform{transform_in} {
}

Transform& Gameobject::get_transform(){
    return transform;
}

void Gameobject::add_component(shared_ptr<Component> new_component){
    new_component->set_game_object(shared_from_this());
    if(std::dynamic_pointer_cast<Model_renderer>(new_component)){
        render_component = new_component;
    }
    else{
        components.push_back(new_component);
    }
}

void Gameobject::start(){
    for(auto& comp : components){
        comp->start();
    }
    if(render_component){
        render_component->start();
    }
}

void Gameobject::update(){
    for(auto& comp : components){
        comp->update();
    }
    if(render_component){
        render_component->update();
    }
}
