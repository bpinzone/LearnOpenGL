//
//  main.cpp
//  hello_window
//
//  Created by Ben Pinzone on 1/3/20.
//  Copyright © 2020 Ben Pinzone. All rights reserved.
//

#include <iostream>

#include <glad/glad.h>
#include <glfw3.h>

using std::cout; using std::endl;

static constexpr long window_width = 800;
static constexpr long window_height = 600;
static constexpr long kinfo_log_size = 512;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

int main(int argc, const char * argv[]) {

    glfwInit();

    // Specify window settings.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // No backward compatibility.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create Window
    // Window size
    GLFWwindow* window = glfwCreateWindow(window_width, k_window_height"LearnOpenGL", nullptr, nullptr);
    if(!window) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    // Set the main context of the current thread to be the window's context.
    glfwMakeContextCurrent(window);

    // Load openGL functions.
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        glfwTerminate();
        return -1;
    }

    // Viewport location, viewport size.
    // The location says that the bottom right of the viewport is referred to as (0, 0).
    // This does not control where the window is on your monitor.
    // Size sets the dimensions of the rendering window in pixels, which is equal to window size.
    // Maps range [-1, 1] to [0, 800] and [0, 600]
    glViewport(0, 0, window_width, k_window_height

    // Resize the viewport when the window is resized.
    // When the window is first displayed framebuffer_size_callback gets called as well with the resulting window dimensions.
    // For retina displays width and height will end up significantly higher than the original input values.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // We register the callback functions after we've created the window and before the render loop is initiated.

    while(!glfwWindowShouldClose(window)) {

        // INPUT
        // We wrote this.
        process_input(window);

        // ========================
        // RENDERING COMMANDS
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
        };

        unsigned int VBO;
        glGenBuffers(1, &VBO); // get a buffer object to access via ID.
        glBindBuffer(GL_ARRAY_BUFFER, VBO); // GL_ARRAY_BUFFER is a target. Make VBO the current GLAB
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // set the data.
        // 4th param is one of: GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW

        /* As of now we stored the vertex data within memory on the graphics card as managed by a vertex buffer object named VBO.
        Next we want to create a vertex and fragment shader that actually processes this data, so let's start building those.
        */

        // VERTEX SHADER
        const char *vertexShaderSource =
            "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "void main() {\n"
            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";

        // Make a vertex shader object
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);

        // set its source & compile
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr); // shader object, how many strings we sending, array of those strings (src), the length of those strings. nullptr->they're null terminated.
        glCompileShader(vertexShader);

        // make sure it compiled successfully
        int success;
        char infoLog[kinfo_log_size];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(vertexShader, kinfo_log_size, nullptr, infoLog); // shader, max size we can store, size they return, place they put it.
            cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
        }

        // FRAGMENT SHADER
        const char* fragmentShaderSource =
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main() {\n"
                "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
            "}\0";
        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(fragmentShader, kinfo_log_size, nullptr, infoLog); // shader, max size we can store, size they return, place they put it.
            cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
        }

        /*
        Both the shaders are now compiled and the only thing left to do is link
        both shader objects into a shader program that we can use for rendering.
        */

        // Create shader program object. The final linked version, all shaders in one.
        unsigned int shaderProgram;
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // make sure it linked properly.
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success){
            glGetProgramInfoLog(shaderProgram, kinfo_log_size, nullptr, infoLog);
            cout << "ERROR::SHADER::LINK_FAILED\n" << infoLog << endl;
        }

        // use this shader program for drawing commands.
        // every shader and rendering call after glUseProgram will now use this program object.
        glUseProgram(shaderProgram);

        // After linking, these are no longer needed.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);


        /*
        Right now we sent the input vertex data to the GPU and instructed the GPU
        how it should process the vertex data within a vertex and fragment shader.
        OpenGL does not yet know how it should interpret the vertex data in memory or
        how it should connect the vertex data to the vertex shader's attributes.
        */


        // Linking vertex attributes.
        const int num_floats_in_attribute = 3;
        glVertexAttribPointer(
            0, num_floats_in_attribute, GL_FLOAT, GL_FALSE,
            num_floats_in_attribute * sizeof(float), (void*)0
        ); // 0 is the "location" as seen in the vertex shader.
        glEnableVertexAttribArray(0);

        // Up to now we dealt with vertex BUFFER objects.

        // Now lets make vertex ARRAY objects to make switching between objects and attributes simpler.
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);

        // bind vao
        glBindVertexArray(VAO);
        // copy vertices array in a buffer for open gl to use.
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // set vertex attribute pointers.
        // (attribute location, num items in attribute, attribute type, normalize, stride, offset to this attribute for the first vertex.)
        glVertexAttribPointer(0, num_floats_in_attribute, GL_FLOAT, GL_FALSE, num_floats_in_attribute * sizeof(float), (void*)0);

        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // probably a different shaders for different objects, so set it again.

        // drawing an object...
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3); //(, starting idx of the vertex array, how many vertices to draw).


        // ========================


        // CHECK AND CALL EVENTS AND SWAP BUFFERS
        // swap front and back buffers.
        glfwSwapBuffers(window);
        // Check for inputs, updates window state, calls corresponding functions for callbacks.
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}