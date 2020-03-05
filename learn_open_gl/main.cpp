// Broken...

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

#include <iostream>
#include <vector>
#include <cmath>

using std::vector;
using std::cout; using std::endl;
using std::sin;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

static int window_width = 1600;
static int window_height = 1200;

float delta_time = 0.0f;
float last_frame_start_time = 0.0f;  // The time when the last frame started rendering.

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

    // Texture face_texture{"awesomeface.png"};
    // Texture wood_texture{"wood_container.jpg"};
    // Shader basic_image_shader{"basic_texture.vert", "basic_texture.frag"};
    // Material basic_image_material{
    //     basic_image_shader,
    //     {
    //         {"primary", wood_texture},
    //         {"secondary", face_texture}
    //     }
    // };

    // Texture box_texture{"box.png"};
    // Texture box_specular_texture{"box_specular.png"};
    // Texture box_emission_texture{"box_emission_texture.png"};
    Shader lit_shader{"lit.vert", "lit.frag"};
    // Material lit_material{lit_shader, {
    //     {"material.diffuse", box_texture},
    //     {"material.specular", box_specular_texture},
    //     {"material.emission", box_emission_texture}
    // }};

    // Mesh cube_mesh {Mesh::Primitive::Cube};

    // vector<glm::vec3> cube_positions = {
    //     { 0.0f,  0.0f,  -0.0f}, { 2.0f,  5.0f, -15.0f},
    //     {-1.5f, -2.2f, -2.5f}, {-3.8f, -2.0f, -12.3f},
    //     { 2.4f, -0.4f, -3.5f}, {-1.7f,  3.0f, -7.5f},
    //     { 1.3f, -2.0f, -2.5f}, { 1.5f,  2.0f, -2.5f},
    //     { 1.5f,  0.2f, -1.5f}, {-1.3f,  1.0f, -1.5f}
    // };

    // vector<Gameobject> game_objects;
    // for(const auto& pos : cube_positions){
    //     glm::mat4 model = glm::mat4(1.0);
    //     model = glm::translate(model, pos);
    //     game_objects.push_back(Gameobject{lit_material, cube_mesh, model});
    // }

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL).

    // Shader sphere_shader("sphere.vert", "sphere.frag");

    Model figure{lit_shader, "./nanosuit/nanosuit.obj"};
    Gameobject figure_object{glm::mat4(1), figure};
    // Model sphere{"./globe-sphere/globe-sphere.obj"};
    // Model sphere{"./my-sphere/untitled.obj"};

    glEnable(GL_DEPTH_TEST); // if you use this, you need to clear it to. See the call to glClear() below.

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {

        float current_frame_start_time = glfwGetTime();
        delta_time = current_frame_start_time - last_frame_start_time;
        last_frame_start_time = current_frame_start_time;

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

        // lit_material.use();
        lit_shader.set_vec3("camera_pos", camera.get_position());

        // set lit material color reflection properties.
        lit_shader.set_float("material.shininess", 32.0f);

        // Pass light source stuff to lit material.
        glm::vec3 random_color{
            sin(glfwGetTime() * 2.0f), sin(glfwGetTime() * 0.7f), sin(glfwGetTime() * 1.3f)
            // 1, 1, 1
        };
        glm::vec3 white_light {1, 1, 1};

        // dir light
        // lit_shader.set_vec3("dir_light.ambient",  white_light * 0.5f * 0.2f);
        // lit_shader.set_vec3("dir_light.diffuse",  white_light * 0.5f); // darken diffuse light a bit
        lit_shader.set_vec3("dir_light.ambient",  white_light);
        lit_shader.set_vec3("dir_light.diffuse",  white_light); // darken diffuse light a bit
        lit_shader.set_vec3("dir_light.specular", white_light);
        lit_shader.set_vec3("dir_light.direction", {0, 0, 20});

        glm::vec3 point_light_positions[] = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
        };

        // point lights
        for(int i = 0; i < 4; ++i){
            std::string i_str = std::to_string(i);
            lit_shader.set_vec3( "point_lights[" + i_str + "].ambient",  white_light * 0.5f * 0.2f);
            lit_shader.set_vec3( "point_lights[" + i_str + "].diffuse",  white_light * 0.5f); // darken diffuse light a bit
            lit_shader.set_vec3( "point_lights[" + i_str + "].specular", white_light);
            lit_shader.set_vec3( "point_lights[" + i_str + "].position", point_light_positions[i]);
            lit_shader.set_float("point_lights[" + i_str + "].constant", 1.0f);
            lit_shader.set_float("point_lights[" + i_str + "].linear", 0.09f);
            lit_shader.set_float("point_lights[" + i_str + "].quadratic", 0.032f);
        }

        // spot light
        lit_shader.set_vec3("spot_light.ambient",  white_light * 0.5f * 0.2f);
        lit_shader.set_vec3("spot_light.diffuse",  white_light * 0.5f); // darken diffuse light a bit
        lit_shader.set_vec3("spot_light.specular", white_light);
        lit_shader.set_vec3("spot_light.position",  camera.get_position());
        lit_shader.set_vec3("spot_light.direction", camera.get_front());
        lit_shader.set_float("spot_light.inner_cutoff",   glm::cos(glm::radians(12.5f)));  // Don't want acos in frag shader.
        lit_shader.set_float("spot_light.outer_cutoff",   glm::cos(glm::radians(17.5f)));


        // lit_shader.use();
        // sphere_shader.use();
        // sphere.draw(lit_shader);
        figure_object.draw();

        // for(auto& go : game_objects){
        //     go.update(delta_time);
        //     go.draw();
        // }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera.ProcessKeyboard(Camera::Movement::FORWARD, delta_time);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera.ProcessKeyboard(Camera::Movement::BACKWARD, delta_time);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.ProcessKeyboard(Camera::Movement::LEFT, delta_time);
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.ProcessKeyboard(Camera::Movement::RIGHT, delta_time);
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
