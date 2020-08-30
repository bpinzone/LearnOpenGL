#include <glad/glad.h>

// Include exactly once.
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"
#include "shader_globals.h"
#include "utility.h"
#include "hierarchy.h"
#include "scene_generation.h"
#include "shader.h"

#include <memory>
#include <iostream>
#include <vector>
#include <cmath>

using std::vector;
using std::cout; using std::endl;
using std::shared_ptr;

void init_open_gl_settings();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

int window_width = 2560;
int window_height = 1440;
Hierarchy hierarchy;
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

    shared_ptr<Shader> instance_directional_shader = add_mst_objects(&hierarchy);

    // === Lighting constants ===
    const glm::vec3 white_light {1, 1, 1};
    instance_directional_shader->set_vec3("dir_light.ambient",  white_light);
    instance_directional_shader->set_vec3("dir_light.diffuse",  white_light * 0.8f); // darken diffuse light a bit
    instance_directional_shader->set_vec3("dir_light.specular", white_light);
    instance_directional_shader->set_vec3("dir_light.direction", glm::vec3(0, 0, -1));
    instance_directional_shader->set_float("material.shininess", 32.0f);

    hierarchy.start();

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

        hierarchy.update();

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
