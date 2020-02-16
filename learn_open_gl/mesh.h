#ifndef MESH_H
#define MESH_H

#include <vector>

// Mesh data. Vertices have Position and UV.
class Mesh {
public:

    enum class Primitive {
        Cube
    };

    Mesh(Primitive p);
    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);

    // Bind the vertex array.
    void use() const;

    int get_indices_size() const;

    ~Mesh();

private:

    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;

    int indices_size;

    void construct_cube();
    void generate_vao(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
};

#endif