#include "scene_generation.h"

#include <algorithm>
#include <iterator>

#include "hierarchy.h"
#include "texture.h"

#include "shader.h"
#include "texture.h"
#include "material.h"
#include "model.h"
#include "game_object.h"

#include "components/component.h"
#include "components/circular_path.h"
#include "components/instances_renderer.h"
#include "components/connector.h"
#include "components/mst.h"

using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::copy;
using std::back_inserter;

// TODO: temporarily returning shader from here to set values.
shared_ptr<Shader> add_mst_objects(Hierarchy* hier){

    // Textures
    // TODO: Poor design. Bool determines if we flip vertically on stbi load.
    auto blue_diffuse = make_shared<Texture>("./textures/blue.png", Texture::Type::DIFFUSE, false);
    auto red_diffuse = make_shared<Texture>("./textures/red.png", Texture::Type::DIFFUSE, false);
    auto gray_specular = make_shared<Texture>("./textures/gray.png", Texture::Type::SPECULAR, false);

    // Shaders
    // Special shader for instanced rendering.
    auto instance_directional_shader = make_shared<Shader>(
        "./shaders/instance_dir_lit.vert", "./shaders/instance_dir_lit.frag");

    // Materials
    auto instance_blue_material = make_shared<Material>(
        instance_directional_shader, Material::Textures_t{ blue_diffuse, gray_specular });

    auto instance_red_material = make_shared<Material>(
        instance_directional_shader, Material::Textures_t{ red_diffuse, gray_specular });

    // Models
    auto sphere_model = make_shared<Model>(instance_directional_shader, "./primitive_models/sphere.obj");
    auto cube_model = make_shared<Model>(instance_directional_shader, "./primitive_models/cube.obj");
    sphere_model->set_materials(instance_blue_material);
    cube_model->set_materials(instance_red_material);

    // Sphere objects
    // Arrange them such that they move to form 4 cone shapes on 2 axes.
    vector<shared_ptr<Gameobject>> sphere_objects;
    constexpr int spheres_per_axis = 10;
    constexpr int num_axes = 2;
    constexpr int num_spheres = spheres_per_axis * num_axes;
    constexpr double depth_mult = 0.75;
    constexpr double radius_mult = 0.75;
    constexpr double speed_mult = 0.0065;
    for(int axis_x = 0; axis_x < num_axes; ++axis_x){
        for(int sphere_x = 0; sphere_x < spheres_per_axis; ++sphere_x){
            shared_ptr<Gameobject> new_sphere_object = make_shared<Gameobject>();
            const double sign_mult = (sphere_x % 2 == 0) ? 1 : -1;
            const double depth = sign_mult * depth_mult * sphere_x;
            const double speed = sign_mult * speed_mult * pow(sphere_x, 2.0);
            const double radius = radius_mult * sphere_x;
            constexpr double start_degs = 0;

            new_sphere_object->add_component(make_shared<Circular_path>(
                depth, start_degs, radius, speed,
                static_cast<Axis>(axis_x)));

            sphere_objects.push_back(new_sphere_object);
        }
    }
    shared_ptr<Gameobject> instanced_spheres_manager = make_shared<Gameobject>();
    instanced_spheres_manager->add_component(make_shared<Instances_renderer>(sphere_model, sphere_objects));

    // Cube objects
    vector<shared_ptr<Gameobject>> cube_objects;
    for(int i = 0; i < num_spheres - 1; ++i){

        shared_ptr<Gameobject> new_cube = make_shared<Gameobject>();
        new_cube->add_component(make_shared<Connector>(
            sphere_objects[i], sphere_objects[i + 1]));
        cube_objects.push_back(new_cube);
    }
    shared_ptr<Gameobject> instanced_cubes_manager = make_shared<Gameobject>();
    instanced_cubes_manager->add_component(make_shared<Instances_renderer>(cube_model, cube_objects));

    // MST Coordinator object
    shared_ptr<Gameobject> coordinator_object = make_shared<Gameobject>();
    coordinator_object->add_component(make_shared<MST_coordinator>(sphere_objects, cube_objects));

    // Add to hierarchy.
    // Order Matters.
    copy(sphere_objects.begin(), sphere_objects.end(), back_inserter(hier->opaque_objects));
    hier->opaque_objects.push_back(instanced_spheres_manager);

    hier->opaque_objects.push_back(coordinator_object);

    copy(cube_objects.begin(), cube_objects.end(), back_inserter(hier->opaque_objects));
    hier->opaque_objects.push_back(instanced_cubes_manager);

    return instance_directional_shader;

}