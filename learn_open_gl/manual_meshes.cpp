#include "manual_meshes.h"

using std::vector;

vector<GeomTestVertex> test_geo_vertices{
	{glm::vec2(-0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f)}, // top-left
    {glm::vec2(0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f)}, // top-right
    {glm::vec2(0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 1.0f)}, // bottom-right
	{glm::vec2(-0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 0.0f)}  // bottom-left
};

vector<unsigned int> test_geo_indices{};

const vector<Vertex> cube_vertices {
    // TODO: Learn how to use blender lol
    // pos                          //normals                    // tex chords
    // Top. Start at top left, going clockwise.
    {glm::vec3{-0.5f, 0.5f, 0.5f,}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec2{0.0f, 0.0f}},
    {glm::vec3{0.5f, 0.5f, 0.5f,}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec2{1.0f, 0.0f}},
    {glm::vec3{0.5f, 0.5f, -0.5f,}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec2{1.0f, 1.0f}},
    {glm::vec3{-0.5f, 0.5f, -0.5f,}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec2{0.0f, 1.0f}},
    // Bottom
    {glm::vec3{-0.5f, -0.5f, -0.5f,}, glm::vec3{0.0f, -1.0f, 0.0f}, glm::vec2{0.0f, 1.0f}},
    {glm::vec3{0.5f, -0.5f, -0.5f,}, glm::vec3{0.0f, -1.0f, 0.0f}, glm::vec2{1.0f, 1.0f}},
    {glm::vec3{0.5f, -0.5f, 0.5f,}, glm::vec3{0.0f, -1.0f, 0.0f}, glm::vec2{1.0f, 0.0f}},
    {glm::vec3{-0.5f, -0.5f, 0.5f,}, glm::vec3{0.0f, -1.0f, 0.0f}, glm::vec2{0.0f, 0.0f}},
    // Right
    {glm::vec3{0.5f, -0.5f, 0.5f,}, glm::vec3{1.0f, 0.0f, 0.0f}, glm::vec2{0.0f, 0.0f}},
    {glm::vec3{0.5f, -0.5f, -0.5f,}, glm::vec3{1.0f, 0.0f, 0.0f}, glm::vec2{1.0f, 0.0f}},
    {glm::vec3{0.5f, 0.5f, -0.5f,}, glm::vec3{1.0f, 0.0f, 0.0f}, glm::vec2{1.0f, 1.0f}},
    {glm::vec3{0.5f, 0.5f, 0.5f,}, glm::vec3{1.0f, 0.0f, 0.0f}, glm::vec2{0.0f, 1.0f}},
    // Left
    {glm::vec3{-0.5f, 0.5f, 0.5f,}, glm::vec3{-1.0f, 0.0f, 0.0f}, glm::vec2{0.0f, 1.0f}},
    {glm::vec3{-0.5f, 0.5f, -0.5f,}, glm::vec3{-1.0f, 0.0f, 0.0f}, glm::vec2{1.0f, 1.0f}},
    {glm::vec3{-0.5f, -0.5f, -0.5f}, glm::vec3{-1.0f, 0.0f, 0.0f}, glm::vec2{1.0f, 0.0f}},
    {glm::vec3{-0.5f, -0.5f, 0.5f,}, glm::vec3{-1.0f, 0.0f, 0.0f}, glm::vec2{0.0f, 0.0f}},
    // Front
    {glm::vec3{-0.5, -0.5f, 0.5f,}, glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec2{0.0f, 0.0f}},
    {glm::vec3{0.5, -0.5f, 0.5f,}, glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec2{1.0f, 0.0f}},
    {glm::vec3{0.5, 0.5f, 0.5f,}, glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec2{1.0f, 1.0f}},
    {glm::vec3{-0.5, 0.5f, 0.5f,}, glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec2{0.0f, 1.0f}},
    // Back
    {glm::vec3{-0.5, 0.5f, -0.5f,}, glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec2{0.0f, 1.0f}},
    {glm::vec3{0.5, 0.5f, -0.5f,}, glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec2{1.0f, 1.0f}},
    {glm::vec3{0.5, -0.5f, -0.5f,}, glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec2{1.0f, 0.0f}},
    {glm::vec3{-0.5, -0.5f, -0.5f,}, glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec2{0.0f, 0.0f}}
};

const vector<unsigned int>& get_cube_indices(){

    static vector<unsigned int> cube_indices;

    static bool do_init = true;
    if(do_init){
        do_init = false;

        for(int face = 0; face < 6; ++face){
            for(int triangle = 0; triangle < 2; ++triangle){
                for(int vertex = 0; vertex < 3; ++vertex){
                    cube_indices.push_back(
                        (4*face) + ((2*triangle + vertex) % 4)
                    );
                }
            }
        }
    }
    return cube_indices;
}

