#ifndef MANUAL_MODELS_H
#define MANUAL_MODELS_H

#include <vector>

#include <glm/glm.hpp>

#include "vertex.h"

extern std::vector<GeomTestVertex> test_geo_vertices;
extern std::vector<unsigned int> test_geo_indices;

extern const std::vector<Vertex> cube_vertices;
const std::vector<unsigned int>& get_cube_indices();

#endif