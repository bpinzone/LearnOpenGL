#ifndef MANUAL_MODELS_H
#define MANUAL_MODELS_H

#include <vector>

#include <glm/glm.hpp>

#include "vertex.h"

extern std::vector<GeomTestVertex> test_geo_vertices;
extern std::vector<unsigned int> test_geo_indices;

extern std::vector<PostProcessQuadVertex> quad_vertices;
extern std::vector<unsigned int> quad_indices;

#endif