#include "manual_meshes.h"

using std::vector;

vector<GeomTestVertex> test_geo_vertices{
	{glm::vec2(-0.5f,  0.5f)}, // top-left
    {glm::vec2(0.5f,  0.5f)}, // top-right
    {glm::vec2(0.5f, -0.5f)}, // bottom-right
	{glm::vec2(-0.5f, -0.5f)}  // bottom-left
};

vector<unsigned int> test_geo_indices{};
