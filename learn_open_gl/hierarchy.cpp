#include "hierarchy.h"

#include "game_object.h"

void Hierarchy::start() {
    for(auto& o : opaque_objects){
        o->start();
    }
    skybox->start();
    for(auto& o : transparent_objects){
        o->start();
    }
}

void Hierarchy::update() {
    for(auto& o : opaque_objects){
        o->update();
    }
    skybox->update();
    for(auto& o : transparent_objects){
        o->update();
    }
}