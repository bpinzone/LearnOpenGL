#include "mesh.h"

#include <glad/glad.h>
#include <glfw3.h>

using std::vector;

Mesh::Mesh(Primitive p) {
    switch(p){
        case Primitive::Cube: construct_cube(); return;
    }
}

Mesh::Mesh(const vector<float>& vertices, const vector<unsigned int>& indices) {
    generate_vao(vertices, indices);
}

void Mesh::use() const {
    glBindVertexArray(vao);
    // No need to unbind, really.
}

int Mesh::get_indices_size() const {
    return indices_size;
}

Mesh::~Mesh() {
    // TODO: DON"T DO THIS HERE!
    // glDeleteVertexArrays(1, &vao);
    // glDeleteBuffers(1, &vbo);
    // glDeleteBuffers(1, &ebo);
}
void Mesh::construct_cube(){

    vector<float> vertices {
        // pos                // tex chords
        // Top. Start at top left, going clockwise.
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f,  // 0
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        // Bottom
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // 4
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
        // Right
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
        // Left
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
        // Front
        -0.5, 0.5f, 0.5f, 0.0f, 0.0f,
        0.5, 0.5f, 0.5f, 0.0f, 1.0f,
        0.5, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5, -0.5f, 0.5f, 1.0f, 1.0f,
        // Back
        -0.5, 0.5f, -0.5f, 0.0f, 0.0f,
        0.5, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5, -0.5f, -0.5f, 1.0f, 0.0f,
        -0.5, -0.5f, -0.5f, 1.0f, 1.0f
    };

    vector<unsigned int> indices;
    for(int face = 0; face < 6; ++face){
        for(int triangle = 0; triangle < 2; ++triangle){
            for(int vertex = 0; vertex < 3; ++vertex){
                indices.push_back(face*4 + triangle + vertex);
            }
        }
    }
    indices_size = indices.size();
    generate_vao(vertices, indices);

}

void Mesh::generate_vao(const vector<float>& vertices, const vector<unsigned int>& indices) {

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
        vertices.data(), GL_STATIC_DRAW
    );

    /*
    A VAO stores the glBindBuffer calls when the target is GL_ELEMENT_ARRAY_BUFFER.
    This also means it stores its unbind calls so make sure you don't unbind the
    element array buffer before unbinding your VAO, otherwise it doesn't have an EBO configured.
    */
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        indices.data(), GL_STATIC_DRAW
    );

    // About glVertexAttribPointer:
    // (location, number things in this attribute, type, normalize?, stride (distance between vertices), offset to get to this attribute for first vertex.)

    // Stride is sizeof(float) * 5.
    const GLsizei stride = sizeof(float) * 5;
    // Position attribute.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, static_cast<void*>(0));
    glEnableVertexAttribArray(0);
    // UV attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3*sizeof(float)));
    glEnableVertexAttribArray(1);


}
