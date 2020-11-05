#include "follow_camera.h"

#include "../camera.h"
#include "../game_object.h"

Follow_camera::Follow_camera(Camera* _cam)
    : cam{_cam}{
}

void Follow_camera::update(){
    game_object->get_transform().set_position(cam->get_position());
}