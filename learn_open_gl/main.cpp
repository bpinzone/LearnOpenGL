#include <glad/glad.h>

// Include exactly once.
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "camera.h"
#include "texture.h"
#include "material.h"
#include "game_object.h"
#include "shader_globals.h"
#include "model.h"
#include "utility.h"

#include "components/component.h"
#include "components/circular_path.h"
#include "components/instances_renderer.h"
#include "components/connector.h"
#include "components/mst.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <algorithm>
#include <iterator>
#include <string>

using std::vector;
using std::cout; using std::endl;
using std::shared_ptr;
using std::make_shared;
using std::copy;
using std::back_inserter;
using std::string;

void init_open_gl_settings();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

int window_width = 2560;
int window_height = 1440;
Camera camera;

int main() {

    // glfw: initialize and configure window
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    glfwWindowHint(GLFW_SAMPLES, 8);

    /*
    This line is causing:
        2020-06-08 14:29:23.998296-0400 learn_open_gl[1722:19555] Metal API Validation Enabled
        2020-06-08 14:29:24.034033-0400 learn_open_gl[1722:19857] flock failed to lock maps file: errno = 35
        2020-06-08 14:29:24.034781-0400 learn_open_gl[1722:19857] flock failed to lock maps file: errno = 35
    */
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "LearnOpenGL", nullptr, nullptr);

    if (!window) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        glfwTerminate();
        return -1;
    }

    // setup mouse input
    // -----------------
    // GLFW should hide the cursor and "capture" it. Capturing means that once the app has focus, the mouse cursor stays within the center of the window (unless app loses focus or quits.)
    // mouse won't be visible and should not leave window.
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // other settings
    // --------------
    init_open_gl_settings();



    // ====================================================================================
    // The rest of main essentially serves as what you would do in an actual engine editor.
    // Put objects in scene, give them components, etc.
    // ====================================================================================


    // Textures
    // TODO: Poor design. Bool determines if we flip vertically on stbi load.
    shared_ptr<Texture> blue_diffuse = make_shared<Texture>("./textures/blue.png", Texture::Type::DIFFUSE, false);
    shared_ptr<Texture> red_diffuse = make_shared<Texture>("./textures/red.png", Texture::Type::DIFFUSE, false);
    shared_ptr<Texture> gray_specular = make_shared<Texture>("./textures/gray.png", Texture::Type::SPECULAR, false);

    // Shaders
    // Special shader for instanced rendering.
    shared_ptr<Shader> instance_directional_shader = make_shared<Shader>("./shaders/instance_dir_lit.vert", "./shaders/instance_dir_lit.frag");

    // Materials
    shared_ptr<Material> instance_blue_material = make_shared<Material>(
        instance_directional_shader, Material::Textures_t{ blue_diffuse, gray_specular }
    );
    shared_ptr<Material> instance_red_material = make_shared<Material>(
        instance_directional_shader, Material::Textures_t{ red_diffuse, gray_specular }
    );

    // Models
    shared_ptr<Model> sphere_model = make_shared<Model>(instance_directional_shader, "./primitive_models/sphere.obj");
    sphere_model->set_materials(instance_blue_material);
    shared_ptr<Model> cube_model = make_shared<Model>(instance_directional_shader, "./primitive_models/cube.obj");
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


    /*
    Put objects in game loop containers.
    Copy all Opaque objects
    Order matters: sphere before coordinator. Coordinator before cubes.
    Instances before their instance manager
    */
    vector<shared_ptr<Gameobject>> opaque_objects;

    // spheres
    copy(sphere_objects.begin(), sphere_objects.end(), back_inserter(opaque_objects));
    opaque_objects.push_back(instanced_spheres_manager);

    // coordinator
    opaque_objects.push_back(coordinator_object);

    // cube
    copy(cube_objects.begin(), cube_objects.end(), back_inserter(opaque_objects));
    opaque_objects.push_back(instanced_cubes_manager);

    // === Lighting constants ===
    const glm::vec3 white_light {1, 1, 1};
    instance_directional_shader->set_vec3("dir_light.ambient",  white_light);
    instance_directional_shader->set_vec3("dir_light.diffuse",  white_light * 0.8f); // darken diffuse light a bit
    instance_directional_shader->set_vec3("dir_light.specular", white_light);
    instance_directional_shader->set_vec3("dir_light.direction", glm::vec3(0, 0, -1));
    instance_directional_shader->set_float("material.shininess", 32.0f);

    for(auto& go : opaque_objects){
        go->start();
    }

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {

        Time::get_instance().update_delta_time();

        // input
        // -----
        processInput(window);

        // render
        // ------
        static constexpr float near_clip = 3.0f;
        static constexpr float far_clip = 2600.0f;

        Shader_globals::get_instance().set_view(camera.GetViewMatrix());
        Shader_globals::get_instance().set_projection(glm::perspective(
            glm::radians(camera.get_fov()),
            static_cast<float>(window_width) / window_height,
            near_clip, far_clip
        ));

        instance_directional_shader->set_vec3("camera_pos", camera.get_position());

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Draw
        for(auto& go : opaque_objects){
            go->update();
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

    GLenum error = glGetError();
    if(error != 0){
        cout << "Exiting main with error code: " << error << endl;
    }

    return 0;
}

void init_open_gl_settings(){

    // Gamma correction.
    // Be aware of sRGB textures. If a texture is in sRGB space, be careful not to gamma correct twice.
    // First thing you do to such a texture is to convert it to linear space before working with it:
    // float gamma = 2.2;
    // vec3 diffuseColor = pow(texture(diffuse, texCoords).rgb, vec3(gamma));
    // Alternatively, use the  GL_SRGB internal texture format.

    // Most diffuse textures are in sRGB space.
    // TODO: Need to change attenuation parameters now that we added this.
    glEnable(GL_FRAMEBUFFER_SRGB);

    // Multi sampling
    glEnable(GL_MULTISAMPLE);

    // Face Culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);  // DO not render back faces.
    glCullFace(GL_BACK);

    // Enable blending
    // The glBlendFunc(GLenum sfactor, GLenum dfactor) function expects two parameters ...
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // === Stencil test ===
    // Default: On, Writable, always passes.
    glEnable(GL_STENCIL_TEST);

    // mask is and'd with the stencil value of fragment.
    glStencilMask(0xFF); // each bit is written to the stencil buffer as is

    // glStencilMask(0x00); // each bit ends up as 0 in the stencil buffer (disabling writes)
    glStencilFunc(GL_ALWAYS, 1, 0xFF);

    // === Depth test ===
    glEnable(GL_DEPTH_TEST); // if you use this, you need to clear it too. See the call to glClear() below.

    // For skybox testing. Because depth values start at 1.
    glDepthFunc(GL_LEQUAL);

    // OpenGL allows us to disable writing to the depth buffer by setting its depth mask to GL_FALSE:
    // glDepthMask(GL_FALSE);

    // We can set the comparison operator (or depth function) by calling glDepthFunc:
    // glDepthFunc(GL_LESS);  //default
    // glDepthFunc(GL_ALWAYS);

    // === Polygon drawing ===
    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL).
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera.ProcessKeyboard(Camera::Movement::FORWARD);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera.ProcessKeyboard(Camera::Movement::BACKWARD);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.ProcessKeyboard(Camera::Movement::LEFT);
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.ProcessKeyboard(Camera::Movement::RIGHT);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions;
    // note that width and height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    window_width = width;
    window_height = height;
}

// [in]	xpos	The new cursor x-coordinate, relative to the left edge of the content area.
// [in]	ypos	The new cursor y-coordinate, relative to the top edge of the content area.
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos) {

    static bool is_first_mouse_input = true;
    static float last_x = 400;
    static float last_y = 300;

    if(is_first_mouse_input){
        is_first_mouse_input = false;
        last_x = x_pos;
        last_y = y_pos;
    }

    float x_offset = x_pos - last_x;
    float y_offset = y_pos - last_y;
    last_x = x_pos;
    last_y = y_pos;
    camera.ProcessMouseMovement(x_offset, y_offset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.ProcessMouseScroll(yoffset);
}
