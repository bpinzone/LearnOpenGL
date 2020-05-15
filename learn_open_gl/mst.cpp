#include "mst.h"

#include <glm.hpp>

#include <algorithm>
#include <iterator>
#include <limits>

using std::min_element;
using std::numeric_limits;
using std::back_inserter;
using std::transform;
using std::vector;
using std::pair;

static double calc_sq_distance(const glm::vec3& v1, const glm::vec3& v2);

MST_coordinator::MST_coordinator(
        Game_objects_t& spheres_in,
        Game_objects_t& cubes_in)

    : spheres{spheres_in} {

    transform(
        cubes_in.begin(), cubes_in.end(), back_inserter(connectors),
        [](auto& cube){
           return cube->template get_component<Connector>();
        }
    );

    insertions.resize(spheres.size());
    locations.resize(spheres.size());
    edges.resize(spheres.size() - 1);
}

void MST_coordinator::start() {
    coordinate();
}

void MST_coordinator::update() {
    coordinate();
}

void MST_coordinator::coordinate(){

    // Reset scratch data.
    for(size_t sphere_i = 0; sphere_i < spheres.size(); ++sphere_i){
        insertions[sphere_i].cost = numeric_limits<double>::infinity();
        insertions[sphere_i].completed = false;
        locations[sphere_i] = spheres[sphere_i]->get_transform().get_position();
    }

    // Find MST edges
    insertions[0].completed = true;
    update_neighbors(0);
    for(size_t spheres_added = 1; spheres_added < spheres.size(); ++spheres_added){

        const auto& min_it = min_element(
            insertions.begin(), insertions.end(),
            [](auto& i1, auto& i2){

                if(i1.completed != i2.completed){
                    return !i1.completed;
                }
                return i1.cost < i2.cost;
            }
        );

        size_t min_idx = distance(insertions.begin(), min_it);
        edges[spheres_added - 1].first = min_idx;
        edges[spheres_added - 1].second = insertions[min_idx].connector;
        insertions[min_idx].completed = true;
        update_neighbors(min_idx);
    }

    // Assign connector components.
    for(size_t edge_i = 0; edge_i < edges.size(); ++edge_i){
        connectors[edge_i]->set_connected_objects(
            spheres[edges[edge_i].first],
            spheres[edges[edge_i].second]
        );
    }

}

void MST_coordinator::update_neighbors(size_t newly_inserted){

    for(size_t neighbor_idx = 0; neighbor_idx < insertions.size(); ++neighbor_idx){
        if(neighbor_idx != newly_inserted && !insertions[neighbor_idx].completed){
            double cost = calc_sq_distance(locations[newly_inserted], locations[neighbor_idx]);
            if(cost < insertions[neighbor_idx].cost){
                insertions[neighbor_idx].cost = cost;
                insertions[neighbor_idx].connector = newly_inserted;
            }
        }
    }
}

double calc_sq_distance(const glm::vec3& v1, const glm::vec3& v2){
    float x_diff = v1.x - v2.x;
    float y_diff = v1.y - v2.y;
    float z_diff = v1.z - v2.z;
    return (x_diff * x_diff) + (y_diff * y_diff) + (z_diff * z_diff);
}
