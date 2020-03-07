#include <glad/glad.h>
#include <glfw3.h>

#include <glm.hpp>
#include <matrix_transform.hpp>

#include "shader.h"
#include "camera.h"
#include "texture.h"
#include "material.h"
#include "game_object.h"
#include "shader_globals.h"
#include "model.h"
#include "utility.h"
#include "model_renderer.h"
#include "component.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <memory>

using std::vector;
using std::cout; using std::endl;
using std::sin;
using std::shared_ptr;
using std::make_shared;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

static int window_width = 1600;
static int window_height = 1200;

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

    // setup mouse input.
    // GLFW should hide the cursor and "capture" it. Capturing means that once the app has focus, the mouse cursor stays within the center of the window (unless app loses focus or quits.)
    // mouse won't be visible and should not leave window.
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glEnable(GL_DEPTH_TEST); // if you use this, you need to clear it to. See the call to glClear() below.

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL).

    // === SPHERES ===
    shared_ptr<Texture> blue_diffuse = make_shared<Texture>("./textures/blue.png", Texture::Type::DIFFUSE);
    shared_ptr<Texture> gray_specular = make_shared<Texture>("./textures/gray.png", Texture::Type::SPECULAR);

    shared_ptr<Shader> directional_shader = make_shared<Shader>("./shaders/dir_lit.vert", "./shaders/dir_lit.frag");

    shared_ptr<Material> blue_material = make_shared<Material>(
        directional_shader,
        Material::Textures_t{ blue_diffuse, gray_specular }
    );

    shared_ptr<Model> sphere_model = make_shared<Model>(directional_shader, "./primitive_models/sphere.obj");
    sphere_model->set_materials(blue_material);


    vector<shared_ptr<Gameobject>> sphere_objects;
    int num_spheres = 10;
    for(int i = 0; i < num_spheres; ++i){
        shared_ptr<Gameobject> new_object = make_shared<Gameobject>(glm::translate(glm::mat4(1), glm::vec3{2*i, 0, 0}));
        new_object->add_component(make_shared<Model_renderer>(sphere_model));
        sphere_objects.push_back(new_object);
    }

    // === Nano suit ===
    shared_ptr<Shader> lit_shader = make_shared<Shader>("./shaders/lit.vert", "./shaders/lit.frag");
    shared_ptr<Model> nano_model = make_shared<Model>(lit_shader, "./nanosuit/nanosuit.obj");
    shared_ptr<Gameobject> nano_object = make_shared<Gameobject>();
    nano_object->add_component(make_shared<Model_renderer>(nano_model));

    // === Lighting constants ===

    glm::vec3 white_light {1, 1, 1};


    // Directional shader
    directional_shader->use(); // Yes, you need this! TODO: why again?
    directional_shader->set_vec3("dir_light.ambient",  white_light * 0.5f * 0.2f);
    directional_shader->set_vec3("dir_light.diffuse",  white_light * 0.5f); // darken diffuse light a bit
    directional_shader->set_vec3("dir_light.specular", white_light);
    directional_shader->set_vec3("dir_light.direction",  glm::vec3(0, 0, -1));
    directional_shader->set_float("material.shininess", 32.0f);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {

        Time::get_instance().update_delta_time();


        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Shader_globals::get_instance().set_view(camera.GetViewMatrix());
        Shader_globals::get_instance().set_projection(glm::perspective(
            glm::radians(camera.get_fov()),
            static_cast<float>(window_width) / window_height, 0.1f,
            100.0f
        ));

        directional_shader->set_vec3("camera_pos", camera.get_position());

        /// hmm


    // Lit shader

    lit_shader->use();
    lit_shader->set_vec3("camera_pos", camera.get_position());
    lit_shader->set_vec3("dir_light.ambient",  white_light);
    lit_shader->set_vec3("dir_light.diffuse",  white_light); // darken diffuse light a bit
    lit_shader->set_vec3("dir_light.specular", white_light);
    lit_shader->set_vec3("dir_light.direction", {0, 0, 20});

    glm::vec3 point_light_positions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    // point lights
    for(int i = 0; i < 4; ++i){
        std::string i_str = std::to_string(i);
        lit_shader->set_vec3( "point_lights[" + i_str + "].ambient",  white_light * 0.5f * 0.2f);
        lit_shader->set_vec3( "point_lights[" + i_str + "].diffuse",  white_light * 0.5f); // darken diffuse light a bit
        lit_shader->set_vec3( "point_lights[" + i_str + "].specular", white_light);
        lit_shader->set_vec3( "point_lights[" + i_str + "].position", point_light_positions[i]);
        lit_shader->set_float("point_lights[" + i_str + "].constant", 1.0f);
        lit_shader->set_float("point_lights[" + i_str + "].linear", 0.09f);
        lit_shader->set_float("point_lights[" + i_str + "].quadratic", 0.032f);
    }

    // spot light
    lit_shader->set_vec3("spot_light.ambient",  white_light * 0.5f * 0.2f);
    lit_shader->set_vec3("spot_light.diffuse",  white_light * 0.5f); // darken diffuse light a bit
    lit_shader->set_vec3("spot_light.specular", white_light);
    lit_shader->set_vec3("spot_light.position",  camera.get_position());
    lit_shader->set_vec3("spot_light.direction", camera.get_front());
    lit_shader->set_float("spot_light.inner_cutoff",   glm::cos(glm::radians(12.5f)));  // Don't want acos in frag shader.
    lit_shader->set_float("spot_light.outer_cutoff",   glm::cos(glm::radians(17.5f)));
    lit_shader->set_float("material.shininess", 32.0f);

    // end lit shader

        /// hmmm

        // set lit material color reflection properties.

        for(auto& go : sphere_objects){
            go->update();
        }

        nano_object->update();

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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera.ProcessKeyboard(Camera::Movement::FORWARD, Time::get_instance().get_delta_time());
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera.ProcessKeyboard(Camera::Movement::BACKWARD, Time::get_instance().get_delta_time());
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.ProcessKeyboard(Camera::Movement::LEFT, Time::get_instance().get_delta_time());
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.ProcessKeyboard(Camera::Movement::RIGHT, Time::get_instance().get_delta_time());
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
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
