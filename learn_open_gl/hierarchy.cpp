#include "hierarchy.h"

#include "material.h"
#include "components/lights/dir_light.h"

#include "game_object.h"

using std::shared_ptr;
using std::bind;
using std::function;

void Hierarchy::start() {
    for_each_in_hierarchy(&Gameobject::start);
}

void Hierarchy::update() {
    for_each_in_hierarchy(&Gameobject::update);
}

void Hierarchy::render_start(shared_ptr<Material> mat_override){
    for_each_in_hierarchy(bind(&Gameobject::render_start, std::placeholders::_1, mat_override));
}
void Hierarchy::render_update(shared_ptr<Material> mat_override){
    for_each_in_hierarchy(bind(&Gameobject::render_update, std::placeholders::_1, mat_override));
}

void Hierarchy::for_each_in_hierarchy(function<void(shared_ptr<Gameobject>)> go_mem_func){
    for(auto& o : opaque_objects){
        std::invoke(go_mem_func, o);
    }
    if(skybox){
        std::invoke(go_mem_func, skybox);
    }
    for(auto& o : transparent_objects){
        std::invoke(go_mem_func, o);
    }
}

void Hierarchy::generate_depth_maps(){

    for(auto dir_light : dir_lights){
        dir_light->generate_depth_map_from(this);
    }
}