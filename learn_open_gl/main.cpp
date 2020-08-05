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
#include "manual_meshes.h"

#include "components/component.h"
#include "components/circular_path.h"
#include "components/instances_renderer.h"
#include "components/model_renderer.h"
#include "components/random_rotator.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <algorithm>
#include <iterator>
#include <string>

#include <random>
#include <functional>

using std::vector;
using std::cout; using std::endl;
using std::shared_ptr;
using std::make_shared;
using std::copy;
using std::back_inserter;
using std::transform;
using std::string;
using std::sin;
using std::cos;

void init_open_gl_settings();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
vector<glm::vec3> generate_cube_positions();

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
    glfwWindowHint(GLFW_SAMPLES, 4);

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

    // Setup Cube Objects.
    shared_ptr<Texture> wood_diffuse = make_shared<Texture>(
        "./textures/box.png", Texture::Type::DIFFUSE, false);

    shared_ptr<Texture> wood_specular = make_shared<Texture>(
        "./textures/box_specular.png", Texture::Type::SPECULAR, false);

    shared_ptr<Shader> point_lit_shader = make_shared<Shader>(
        "./shaders/lit.vert", "./shaders/lit.frag");

    shared_ptr<Material> cube_material = make_shared<Material>(
        point_lit_shader, Material::Textures_t{wood_diffuse, wood_specular}
    );

    shared_ptr<Model> cube_model = make_shared<Model>(
        Model::Meshes_t{make_shared<Mesh>(cube_vertices, get_cube_indices(), cube_material)}
    );

    vector<glm::vec3> cube_positions = generate_cube_positions();

    vector<shared_ptr<Gameobject>> cube_objects;
    transform(
        cube_positions.begin(), cube_positions.end(), back_inserter(cube_objects),
        [](const auto& pos){
            auto cube = make_shared<Gameobject>();
            cube->get_transform().translate(pos);
            cube->add_component(make_shared<Random_rotator>());
            return cube;
        }
    );

    shared_ptr<Gameobject> cube_instances_manager = make_shared<Gameobject>();
    cube_instances_manager->add_component(make_shared<Instances_renderer>(
        cube_model, cube_objects
    ));

    // Setup Point light

    shared_ptr<Shader> light_source_shader = make_shared<Shader>(
        "./shaders/light_source.vert", "./shaders/light_source.frag");

    shared_ptr<Material> light_source_material = make_shared<Material>(
        light_source_shader, Material::Textures_t{});

    shared_ptr<Model> light_source_model = make_shared<Model>(light_source_shader, "./primitive_models/sphere.obj");
    light_source_model->set_materials(light_source_material);

    shared_ptr<Gameobject> light_source = make_shared<Gameobject>();
    light_source->get_transform().set_scale(glm::vec3(0.5f));
    light_source->add_component(make_shared<Model_renderer>(light_source_model));
    {
        constexpr float depth = 8;
        constexpr double starting_degrees = 0;
        constexpr double radius = 10;
        constexpr double speed = 10;
        light_source->add_component(make_shared<Circular_path>(
            depth, starting_degrees, radius, speed, Axis::Y
        ));
    }

    /*
    Put objects in game loop containers.
    Instances before their instance manager
    */
    vector<shared_ptr<Gameobject>> opaque_objects;
    copy(cube_objects.begin(), cube_objects.end(), back_inserter(opaque_objects));
    opaque_objects.push_back(cube_instances_manager);
    opaque_objects.push_back(light_source);

    // Set constant lighting parameters.
    const glm::vec3 white_light {1, 1, 1};
    point_lit_shader->set_float("material.shininess", 32.0f);
    point_lit_shader->set_vec3("point_light.ambient",  white_light);
    point_lit_shader->set_vec3("point_light.diffuse",  white_light * 0.75f); // darken diffuse light a bit
    point_lit_shader->set_vec3("point_light.specular", white_light);
    point_lit_shader->set_float("point_light.constant",  1.0f);
    point_lit_shader->set_float("point_light.linear",    0.09f * 0.5f);
    point_lit_shader->set_float("point_light.quadratic", 0.032f * 0.5f);

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
        static constexpr float near_clip = 0.1f;
        static constexpr float far_clip = 1000.0f;

        Shader_globals::get_instance().set_view(camera.GetViewMatrix());
        Shader_globals::get_instance().set_projection(glm::perspective(
            glm::radians(camera.get_fov()),
            static_cast<float>(window_width) / window_height,
            near_clip, far_clip
        ));

        point_lit_shader->set_vec3("camera_pos", camera.get_position());

        // NOTE: Position is one frame old. Not caring for now.
        point_lit_shader->set_vec3("point_light.position", light_source->get_transform().get_position());

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

vector<glm::vec3> generate_cube_positions(){

    vector<glm::vec3> cube_positions;

    constexpr float max_offset = 0.5f;

    static auto random_offset = std::bind(
        std::uniform_real_distribution<float>{0.0, max_offset},
        std::default_random_engine{}
    );

    constexpr int num_cubes = 40.0f;
    constexpr float belt_radius = 10.0f;

    for(int cube_x = 0; cube_x < num_cubes; cube_x++) {

        const float angle = (static_cast<float>(cube_x) / num_cubes) * 360;

        cube_positions.push_back(glm::vec3(
            sin(glm::radians(angle)) * belt_radius + random_offset(),
            random_offset(),
            cos(glm::radians(angle)) * belt_radius + random_offset()
        ));

    }

    return cube_positions;
}