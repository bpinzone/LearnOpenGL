#include "manual_meshes.h"

using std::vector;

vector<GeomTestVertex> test_geo_vertices{
	{glm::vec2(-0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f)}, // top-left
    {glm::vec2(0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f)}, // top-right
    {glm::vec2(0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 1.0f)}, // bottom-right
	{glm::vec2(-0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 0.0f)}  // bottom-left
};

vector<unsigned int> test_geo_indices{};
