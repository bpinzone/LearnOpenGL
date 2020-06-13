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

#include "component.h"
#include "circular_path.h"
#include "model_renderer.h"
#include "outline_model_renderer.h"
#include "connector.h"
#include "mst.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <algorithm>
#include <iterator>

using std::vector;
using std::cout; using std::endl;
using std::sin;
using std::shared_ptr;
using std::make_shared;
using std::copy;
using std::back_inserter;
using std::transform;

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

    // setup mouse input.
    // GLFW should hide the cursor and "capture" it. Capturing means that once the app has focus, the mouse cursor stays within the center of the window (unless app loses focus or quits.)
    // mouse won't be visible and should not leave window.
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Face Culling
    // potential problem: My vertex data in correct CCW winding order?
    glEnable(GL_CULL_FACE);
    // Define front as CCW.
    glFrontFace(GL_CCW);
    // DO not render back faces.
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

    // OpenGL allows us to disable writing to the depth buffer by setting its depth mask to GL_FALSE:
    // glDepthMask(GL_FALSE);

    //  We can set the comparison operator (or depth function) by calling glDepthFunc:
    // glDepthFunc(GL_LESS);  //default
    // glDepthFunc(GL_ALWAYS);

    // === Polygon drawing ===
    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL).


    // === Render to a texture ===
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // generate texture (color attachment)
    unsigned int texColorBuffer;
    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window_width, window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); // allocate.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

    // generate render buffer attachment for depth and stencil test.
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window_width, window_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    // attach it to the currently bound frame buffer object
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // Make sure frameBuffer is complete
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    }
    // unbind the frame buffer we just made.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);




    // Textures
    shared_ptr<Texture> blue_diffuse = make_shared<Texture>("./textures/blue.png", Texture::Type::DIFFUSE);
    shared_ptr<Texture> red_diffuse = make_shared<Texture>("./textures/red.png", Texture::Type::DIFFUSE);
    shared_ptr<Texture> gray_specular = make_shared<Texture>("./textures/gray.png", Texture::Type::SPECULAR);

    shared_ptr<Texture> grass_diffuse = make_shared<Texture>("./textures/grass.png", Texture::Type::DIFFUSE);
    shared_ptr<Texture> window_diffuse = make_shared<Texture>("./textures/window.png", Texture::Type::DIFFUSE);
    shared_ptr<Texture> black_specular = make_shared<Texture>("./textures/black.png", Texture::Type::SPECULAR);

    shared_ptr<Texture> portal_texture = make_shared<Texture>(texColorBuffer, Texture::Type::DIFFUSE);

    // Shaders
    shared_ptr<Shader> directional_shader = make_shared<Shader>("./shaders/dir_lit.vert", "./shaders/dir_lit.frag");
    shared_ptr<Shader> color_shader = make_shared<Shader>("./shaders/color.vert", "./shaders/color.frag");
    shared_ptr<Shader> portal_shader = make_shared<Shader>("./shaders/from_texture.vert", "./shaders/from_texture.frag");

    // Materials
    shared_ptr<Material> blue_material = make_shared<Material>(
        directional_shader, Material::Textures_t{ blue_diffuse, gray_specular }
    );
    shared_ptr<Material> red_material = make_shared<Material>(
        directional_shader, Material::Textures_t{ red_diffuse, gray_specular }
    );
    shared_ptr<Material> color_material = make_shared<Material>(
        color_shader, Material::Textures_t{}
    );

    shared_ptr<Material> grass_material = make_shared<Material>(
        directional_shader, Material::Textures_t{grass_diffuse, black_specular}
    );

    shared_ptr<Material> window_material = make_shared<Material>(
        directional_shader, Material::Textures_t{window_diffuse, black_specular}
    );

    shared_ptr<Material> portal_material = make_shared<Material>(
        portal_shader, Material::Textures_t{portal_texture}
    );

    // Models
    shared_ptr<Model> sphere_model = make_shared<Model>(directional_shader, "./primitive_models/sphere.obj");
    sphere_model->set_materials(blue_material);

    shared_ptr<Model> cube_model = make_shared<Model>(directional_shader, "./primitive_models/cube.obj");
    cube_model->set_materials(red_material);

    shared_ptr<Model> grass_model = make_shared<Model>(directional_shader, "./primitive_models/quad.obj");
    grass_model->set_materials(grass_material);

    shared_ptr<Model> window_model = make_shared<Model>(directional_shader, "./primitive_models/quad.obj");
    window_model->set_materials(window_material);

    shared_ptr<Model> portal_model = make_shared<Model>(portal_shader, "./primitive_models/quad.obj");
    portal_model->set_materials(portal_material);

    // Sphere objects
    vector<shared_ptr<Gameobject>> sphere_objects;
    int num_spheres = 250;
    for(int i = 0; i < num_spheres; ++i){
        shared_ptr<Gameobject> new_object = make_shared<Gameobject>();
        new_object->add_component(make_shared<Outline_model_renderer>(sphere_model, color_material));

        glm::vec3 start_pos = glm::vec3{0, 0, pow(-1, i % 2) * i};
        double start_degs = 0;
        double radius = (i + 1) * 0.5;
        double speed = 0.005 * pow(i, 2);
        new_object->add_component(make_shared<Circular_path>(
            start_pos, start_degs, radius, speed
        ));
        sphere_objects.push_back(new_object);
    }

    // Cube objects
    vector<shared_ptr<Gameobject>> cube_objects;
    for(int i = 0; i < num_spheres - 1; ++i){
        shared_ptr<Gameobject> new_object = make_shared<Gameobject>();
        new_object->add_component(make_shared<Model_renderer>(cube_model));
        new_object->add_component(make_shared<Connector>(
            sphere_objects[i], sphere_objects[i + 1]
        ));
        cube_objects.push_back(new_object);
    }

    // Coordinator object
    shared_ptr<Gameobject> coordinator_object = make_shared<Gameobject>();
    coordinator_object->add_component(make_shared<MST_coordinator>(sphere_objects, cube_objects));

    // Grass
    shared_ptr<Gameobject> grass_object = make_shared<Gameobject>();
    grass_object->add_component(make_shared<Model_renderer>(grass_model));
    grass_object->get_transform().set_scale(glm::vec3(4, 4, 4));
    // NOTE: documentation is wrong? It takes radians.
    grass_object->get_transform().set_rotation(glm::rotate(
        glm::mat4(1), glm::radians(90.0f), glm::vec3(1, 0, 0)
    ));

    // Windows
    vector<glm::vec3> window_positions{
        {-15.0f,  0.0f, -5.0f}, { 15.0f,  0.0f,  51.0f},
        { 0.0f,  0.0f,  7.0f}, {-3.0f,  0.0f, -23.0f}, { 5.0f,  0.0f, -6.0f}
    };
    vector<shared_ptr<Gameobject>> window_objects;
    transform(window_positions.begin(), window_positions.end(), back_inserter(window_objects),
        [&window_model](const auto& pos){

            shared_ptr<Gameobject> window_object = make_shared<Gameobject>();
            window_object->add_component(make_shared<Model_renderer>(window_model));
            window_object->get_transform().set_scale(glm::vec3(4, 4, 4));
            // NOTE: documentation is wrong? It takes radians.
            window_object->get_transform().set_rotation(glm::rotate(
                glm::mat4(1), glm::radians(90.0f), glm::vec3(1, 0, 0)
            ));
            window_object->get_transform().set_position(pos);
            return window_object;
    });

    // Extremely simple blending
    // Less = Rendered First = Farthur away.
    static const auto blend_sorter = [](shared_ptr<Gameobject> go1, shared_ptr<Gameobject> go2){
        float go1_distance = glm::distance(go1->get_transform().get_position(), camera.get_position());
        float go2_distance = glm::distance(go2->get_transform().get_position(), camera.get_position());
        return go1_distance > go2_distance;
    };

    // quad object (scene will be drawn onto this object.)
    shared_ptr<Gameobject> portal_object = make_shared<Gameobject>();
    portal_object->add_component(make_shared<Model_renderer>(portal_model));
    portal_object->get_transform().set_scale(glm::vec3(40, 40, 40));
    portal_object->get_transform().set_rotation(
        glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(1, 0, 0))
    );
    portal_object->get_transform().set_position(glm::vec3(0, 0, -10));

    // Opaque objects
    vector<shared_ptr<Gameobject>> opaque_objects;
    copy(sphere_objects.begin(), sphere_objects.end(), back_inserter(opaque_objects));
    opaque_objects.push_back(coordinator_object);
    copy(cube_objects.begin(), cube_objects.end(), back_inserter(opaque_objects));

    // transparent objects
    vector<shared_ptr<Gameobject>> transparent_objects;
    transparent_objects.push_back(grass_object);
    copy(window_objects.begin(), window_objects.end(), back_inserter(transparent_objects));

    // === Lighting constants ===

    glm::vec3 white_light {1, 1, 1};

    // Directional shader
    // Really, makes the dir_shader the active program, and populates the uniforms.
    // The uniforms belong to the shader program.
    // So if you switch to another shader and back, you DON'T need to populate uniforms again if they haven't changed.
    directional_shader->set_vec3("dir_light.ambient",  white_light * 0.8f * 0.6f);
    directional_shader->set_vec3("dir_light.diffuse",  white_light * 0.8f); // darken diffuse light a bit
    directional_shader->set_vec3("dir_light.specular", white_light);
    directional_shader->set_vec3("dir_light.direction",  glm::vec3(0, 0, -1));
    directional_shader->set_float("material.shininess", 32.0f);

    sort(transparent_objects.begin(), transparent_objects.end(), blend_sorter);

    for(auto& go : opaque_objects){
        go->start();
    }
    for(auto& go : transparent_objects){
        go->start();
    }

    portal_object->start();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {

        Time::get_instance().update_delta_time();

        // input
        // -----
        processInput(window);

        // render
        // ------
        static const float near_clip = 3.0f;
        static const float far_clip = 800.0f;
        Shader_globals::get_instance().set_view(camera.GetViewMatrix());
        Shader_globals::get_instance().set_projection(glm::perspective(
            glm::radians(camera.get_fov()),
            static_cast<float>(window_width) / window_height,
            near_clip, far_clip
        ));
        directional_shader->set_vec3("camera_pos", camera.get_position());
        portal_shader->set_vec3("camera_pos", camera.get_position());

        // Render to framebuffer with texture.
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        // Draw
        sort(transparent_objects.begin(), transparent_objects.end(), blend_sorter);
        for(auto& go : opaque_objects){
            go->update();
        }
        for(auto& go : transparent_objects){
            go->update();
        }

        // Render to window using quad textured with last render buffer texture.
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        portal_object->update();

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
