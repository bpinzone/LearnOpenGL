#include "component.h"
#include "game_object.h"

using std::shared_ptr;

void Component::start(){
}

void Component::update(){
}

void Component::set_game_object(shared_ptr<Gameobject> game_object_in){
    game_object = game_object_in;
}
