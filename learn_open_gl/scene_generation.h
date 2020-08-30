#ifndef SCENE_GENERATION_H
#define SCENE_GENERATION_H

#include <utility>
#include <memory>

class Hierarchy;
class Shader;

// Provides functions for generating objects to put into a scene.
// Always pushes back into the hierarchy. Caller is responsible for calling these functions in the correct order if applicable. (Instancing update order, coordinating the MST update order, etc.)

std::shared_ptr<Shader> add_mst_objects(Hierarchy* hier);

#endif