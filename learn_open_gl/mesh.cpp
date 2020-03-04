#include "mesh.h"

#include <glad/glad.h>
#include <glfw3.h>

#include <string>

using std::string;
using std::vector;

Mesh::Mesh(
        const std::vector<Vertex>& vertices_in,
        const std::vector<unsigned int>& indices_in,
        const std::vector<Texture>& textures_in)

    : vertices{vertices_in}, indices{indices_in}, textures{textures_in} {
    setup_mesh();
}

void Mesh::draw(Shader shader) {

    unsigned int diffuse_num = 1;
    unsigned int specular_num = 1;
    for(size_t tex_idx = 0; tex_idx < textures.size(); ++tex_idx){

        glActiveTexture(GL_TEXTURE0 + tex_idx);

        string number;
        string name = textures[tex_idx].type;
        if(name == "diffuse"){
            number = std::to_string(diffuse_num++);
        }
        else if(name == "specular"){
            number = std::to_string(specular_num++);
        }
        shader.set_float("material." + name + number, tex_idx);
        glBindTexture(GL_TEXTURE_2D, textures[tex_idx].texture_id);
    }
    glActiveTexture(GL_TEXTURE0);

    // Draw
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


// Mesh::~Mesh() {
//     // TODO: DON"T DO THIS HERE!
//     // glDeleteVertexArrays(1, &vao);
//     // glDeleteBuffers(1, &vbo);
//     // glDeleteBuffers(1, &ebo);
// }

void Mesh::setup_mesh(){

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
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
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int),
        indices.data(), GL_STATIC_DRAW
    );

    // About glVertexAttribPointer:
    // (location, number things in this attribute, type, normalize?, stride (distance between vertices), offset to get to this attribute for first vertex.)

    // Position attribute.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
    // NOTE TODO: These are now flipped.
    // Normal attribute.
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
    // Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tex_coord)));

    glBindVertexArray(0);
}
