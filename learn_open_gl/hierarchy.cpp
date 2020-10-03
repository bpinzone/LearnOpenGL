#include "hierarchy.h"

#include <functional>

#include "game_object.h"

void Hierarchy::start() {
    for_each_in_hierarchy(&Gameobject::start);
}

void Hierarchy::update() {
    for_each_in_hierarchy(&Gameobject::update);
}

// TODO: do not duplicate work.
void Hierarchy::render_start(){
    for_each_in_hierarchy(&Gameobject::render_start);
}
void Hierarchy::render_update(){
    for_each_in_hierarchy(&Gameobject::render_update);
}

void Hierarchy::for_each_in_hierarchy(Gameobject_fn_t go_fn){
    for(auto& o : opaque_objects){
        std::invoke(go_fn, o);
    }
    if(skybox){
        std::invoke(go_fn, skybox);
    }
    for(auto& o : transparent_objects){
        std::invoke(go_fn, o);
    }
}