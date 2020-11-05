#ifndef SCENE_GENERATION_H
#define SCENE_GENERATION_H

#include <utility>
#include <memory>

class Hierarchy;
class Camera;

// Provides functions for generating objects to put into a scene.
// Always pushes back into the hierarchy. Caller is responsible for calling these functions in the correct order if applicable. (Instancing update order, coordinating the MST update order, etc.)
void add_mst_objects(Hierarchy* hier);
void add_cam_following_dir_light(Hierarchy* hier, Camera* cam);

#endif