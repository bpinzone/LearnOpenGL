#include "manual_meshes.h"

using std::vector;

vector<GeomTestVertex> test_geo_vertices{
	{glm::vec2(-0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f)}, // top-left
    {glm::vec2(0.5f,  0.5f),  glm::vec3(0.0f, 1.0f, 0.0f)}, // top-right
    {glm::vec2(0.5f, -0.5f),  glm::vec3(0.0f, 0.0f, 1.0f)}, // bottom-right
	{glm::vec2(-0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 0.0f)}  // bottom-left
};

vector<unsigned int> test_geo_indices{};

vector<PostProcessQuadVertex> quad_vertices {

    // clockwise, starting from top left.
    // pos, tex coord
    // For tex coords, (0, 0) is bottom left, (1, 0) is bottom right.
    {glm::vec3(-1, 1, 0), glm::vec2(0, 1)},
    {glm::vec3(1, 1, 0),  glm::vec2(1, 1)},
    {glm::vec3(1, -1, 0), glm::vec2(1, 0)},
    {glm::vec3(-1, -1, 0),   glm::vec2(0, 0)}
};

vector<unsigned int> quad_indices {

    2, 1, 0,
    0, 3, 2
};