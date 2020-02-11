
#include <glad/glad.h>
#include <glfw3.h>

#include <glm.hpp>
#include <matrix_transform.hpp>
#include <type_ptr.hpp>

#include "stb_image.h"
#include "shader.h"

#include <iostream>
#include <cmath>

using std::cout; using std::endl;
using std::sin;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// The callback functions receives the cursor position, measured in screen coordinates but relative to the top-left corner of the window content area. On platforms that provide it, the full sub-pixel cursor position is passed on.
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
// I think these are in pixels.
static int window_width = 1600;
static int window_height = 1200;

// mouse tracking. Looks like these are relative to k_screen_dimensions.
bool is_first_mouse_input = true;
float last_x = 400;
float last_y = 300;

// I'm shuddering
glm::vec3 camera_pos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up    = glm::vec3(0.0f, 1.0f,  0.0f);

// camera fov. degrees
float fov = 45.0f;

float delta_time = 0.0f;  // time between current and last frame.
float last_frame_start_time = 0.0f;  // The time when the last frame started rendering.

// The negative will only affect the sin of yaw, which only the z-axis looks at.
float yaw = -90.0f;  // we have too many negative signs` for this -z thing. Should make that a single point of maintenance.
float pitch = 0.0f;

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


    // FIRST TEXTURE
    // The whole process of generating a texture thus looks something like this: ================
    unsigned int texture_id;
    glGenTextures(1, &texture_id);
    // Just like other objects we need to bind it so any subsequent texture commands will configure the currently bound texture:
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // WRAPPING OPTIONS for the currently bound texture object:
    // (texture target, specify axis, mode for how that axis should wrap)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    /*
    // specify the border color in case "clamp to border" wrap mode is used.
    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    */

    // Filtering options for the cur bound texture object.
    // (texture 2D, when to use this mode <upscaling vs downscaling>, what filter mode to use.)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and generate the texture
    int width, height, nrChannels; // number of color channels.
    // read texture data from disk using library.
    unsigned char *data = stbi_load("wood_container.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        // actually generate the texture data
        // (target, level of mipmap youre creating, format of how to store, width, height, legacy 0,
        // format we're feeding, data type of src image (how we stored them, unsigned char), actual data. )

        // This is operating on texture bound to target GL_TEXTURE_2D, which is "texture_id".
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        // generate mip maps automatically. Alternatively, do the above a bunch, incrementing 2nd arg.
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    // bound texture object now has the data in it.
    // free data given by stbi_load
    stbi_image_free(data);



    // SECOND TEXTURE
    unsigned int face_texture_id;
    glGenTextures(1, &face_texture_id);
    glBindTexture(GL_TEXTURE_2D, face_texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upside down image? This happens because OpenGL expects the 0.0 coordinate on the y-axis to be on the bottom side of the image, but images usually have 0.0 at the top of the y-axis. Luckily for us, stb_image.h can flip the y-axis during image loading by adding the following statment before loading any image:
    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
    assert(data);
    // todo why isn't third parameter "GL_RBGA" ? Looks like we're not ACTUALLY utilizing transparency right now, seeing as the shader uses "mix", aka lerp.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    // =========================================


    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // float vertices[] = {
    //     // positions          // colors           // texture coords
    //     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
    //     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    //     -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    //     -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    // };
    // for a cube.
    float vertices[] = {
        // pos                // tex chords?
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /*
    A VAO stores the glBindBuffer calls when the target is GL_ELEMENT_ARRAY_BUFFER.
    This also means it stores its unbind calls so make sure you don't unbind the
    element array buffer before unbinding your VAO, otherwise it doesn't have an EBO configured.
    */
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    unsigned int indices[] = {
        1, 2, 3,  // first triangle
        0, 1, 3   // second triangle
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Stride is now 32 (8*sizeof(float))
    // (location, number things in this attribute, type, normalize?, stride (distance between vertices), offset to get to this attribute for first vertex.)
    // position attribute.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void*>(0));
    glEnableVertexAttribArray(0);
    // color attribute
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));
    // glEnableVertexAttribArray(1);
    // texture coord attribute.
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // BUT, looks like its only allowed for the ARRAY_BUFFER, and not the ELEMENT_ARRAY_BUFFER

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);


    Shader my_shader{"vert.vert", "frag.frag"};

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL).

    glEnable(GL_DEPTH_TEST); // if you use this, you need to clear it to. See the call to glClear() below.

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

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

        // use the shader
        my_shader.use();
        // my_shader.set_float("intensity", sin(glfwGetTime() * 2) / 2.0f + 0.5f);
        my_shader.set_float("intensity", 1.0f);
        my_shader.set_int("texture1", 0); // map the texture1 sampler variable to texture unit 0.
        my_shader.set_int("texture2", 1); // map the texture2 sampler variable to texture unit 1.

        // MODEL
        // Put it on the floor transform.
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        // VIEW
        // positive z axis is the vector from screen to you.
        // glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
        // glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
        // The name direction vector is not the best chosen name, since it is actually pointing in the reverse direction of what it is targeting.
        // glm::vec3 camera_dir = glm::normalize(camera_pos - camera_target);
        // glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        // glm::vec3 camera_right = glm::normalize(glm::cross(up, camera_dir));
        // glm::vec3 camera_up = glm::cross(camera_dir, camera_right); // this seems unnecessary.
        // Want to create lookAt, luckily glm will do it for us.
        // glm::mat4 view;
        // view = glm::lookAt(
        //     glm::vec3(0.0f, 0.0f, 3.0f), // camera pos. (pos z is screen to me)
        //     glm::vec3(0.0f, 0.0f, 0.0f), // target, the thing you want to look at.
        //     glm::vec3(0.0f, 1.0f, 0.0f)  // up vector for the camera.
        // );
        // const float radius = 10.0f;
        // float cam_x = sin(glfwGetTime()) * radius;
        // float cam_z = cos(glfwGetTime()) * radius;
        // view = glm::lookAt(glm::vec3(cam_x, 0.0f, cam_z), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        // glm::mat4 view = glm::mat4(1.0f);
        // note that we're translating the scene in the reverse direction of where we want to move
        // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 view{glm::lookAt(camera_pos, camera_pos + camera_front, camera_up)};

        // PROJECTION
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(fov), static_cast<float>(window_width) / window_height, 0.1f, 100.0f);

        // model
        unsigned int model_loc = glGetUniformLocation(my_shader.program_id, "model");
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
        // view
        unsigned int view_loc = glGetUniformLocation(my_shader.program_id, "view");
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
        // projection
        unsigned int projection_loc = glGetUniformLocation(my_shader.program_id, "projection");
        glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));



        // ======= random transformation set 1. ========================================
        // glm::mat4 trans = glm::mat4(1.0f);
        // trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        // // (tran, radians, axis of rotation)
        // trans = glm::rotate(trans, static_cast<float>(glfwGetTime() * 3), glm::vec3{0.0f, 0.0f, 1.0f});
        // trans = glm::scale(trans, static_cast<float>(sin(glfwGetTime() * 2)) * glm::vec3{1.0f});

        // unsigned int transform_loc = glGetUniformLocation(my_shader.program_id, "transform");
        // // (location, how many matrices to send, transpose?, //value ptr is like address-of operator)
        // glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(trans));
        // ============================================================================

        // Populate (numbered) texture units with desired textures.
        // Activate unit, then bind the texture to it. Repeat.
        // put texture_id into texture unit 0.
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        // put face_texture_id into texture unit 1.
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, face_texture_id);

        // After activating a texture unit, a subsequent glBindTexture call will bind that texture to the currently active texture unit. Texture unit GL_TEXTURE0 is always by default activated, so we didn't have to activate any texture units in the previous example when using glBindTexture.


        // bind VAO
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        for(unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            unsigned int model_loc = glGetUniformLocation(my_shader.program_id, "model");
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
            // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // draw 6 vertices (number of elements in "indices"), 0 is an unused offset here.
        }


        glBindVertexArray(0); // no need to unbind it every time

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();



    // random learning
    // To create an orthographic projection matrix we make use of GLM's built-in function glm::ortho
    // ** REMEMBER, THE THINGS GOING INTO THIS ARE ALREADY IN VIEW SPACE!!!!! **
    // THIS MATRIX TAKES US FROM VIEW SPACE TO CLIP SPACE. Its one of the two forms a projection matrix takes.
    // AND we're still working on vertices!
    glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
    // The first two parameters specify the left and right coordinate of the frustum and the third and fourth parameter specify the bottom and top part of the frustum. With those 4 points we've defined the size of the near and far planes and the 5th and 6th parameter then define the distances between the near and far plane. This specific projection matrix transforms all coordinates between these x, y and z range values to normalized device coordinates.

    // (left coord, right coord, bottom coord, top coord of frustum, 5 and 6 define the distances between the near and far plane...)

    // Perspective viewing
    // higher w -> appear farthur away.
    // Once the coordinates are transformed to clip space they are in the range -w to w. They are in this range BEFORE perspective division! (anything outside this range is clipped)
    // create perspective projection matrix.
    // (fov in radians, aspect ratio: width/height, near clipping plane distance, far plane distance.)
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);



    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    const float camera_speed = 3.5f * delta_time; // adjust as needed, not controlling frequency of checking for input. Depends on processor.
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera_pos += camera_speed * camera_front;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera_pos -= camera_speed * camera_front;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
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

void mouse_callback(GLFWwindow* window, double x_pos, double y_pos) {
    if(is_first_mouse_input){
        is_first_mouse_input = false;
        last_x = x_pos;
        last_y = y_pos;
    }
    float x_offset = x_pos - last_x;
    float y_offset = y_pos - last_y;
    last_x = x_pos;
    last_y = y_pos;

    static const float sensitivity = 0.05f;
    x_offset *= sensitivity;
    y_offset *= sensitivity * -1;

    yaw += x_offset;
    pitch += y_offset;

    if(pitch > 89.0f){
        pitch = 89.0f;
    }
    if(pitch < -89.0f){
        pitch = -89.0f;
    }

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    camera_front = glm::normalize(direction);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){

    fov -= yoffset;
    if(fov < 1.0f){
        fov = 1.0f;
    }
    else if(fov > 45.0f){
        fov = 45.0f;
    }
}
