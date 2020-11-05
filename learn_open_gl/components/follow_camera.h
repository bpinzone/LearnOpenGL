#ifndef FOLLOW_CAMERA_H
#define FOLLOW_CAMERA_H

#include "component.h"

class Camera;

class Follow_camera : public Component {

public:
    Follow_camera(Camera* _cam);

    void update() override;

private:

    Camera* cam;

};

#endif