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

// struct Insertion{
//     size_t connector;
//     double cost;
//     bool completed;
// };

// vector<pair<size_t, size_t>> mst(vector<Gameobject>& game_objects){

//     vector<pair<size_t, size_t>> edges;

//     vector<glm::vec3> locations;
//     locations.reserve(game_objects.size());
//     transform(game_objects.begin(), game_objects.end(), back_inserter(locations),
//         [](auto& game_object){
//         return game_object.get_position();
//     });

//     vector<Insertion> insertions(
//         locations.size(),
//         Insertion{0, numeric_limits<double>::infinity(), false}
//     );

//     insertions[0].completed = true;
//     update_neighbors(locations, insertions, 0);

//     size_t objects_remaining = locations.size() - 1;
//     while(objects_remaining > 0){

//         // i1 < i2 ?
//         auto min_it = min_element(insertions.begin(), insertions.end(), [](auto& i1, auto& i2){

//             if(i1.completed != i2.completed){
//                 return !i1.completed;
//             }
//             return i1.cost < i2.cost;
//         });
//         size_t min_idx = distance(insertions.begin(), min_it);
//         edges.push_back(min_idx, insertions[min_idx].connector);
//         insertions[min_idx].completed = true;
//         update_neighbors(min_idx);
//         --objects_remaining;
//     }
//     return edges;
// }

// void update_neighbors(vector<glm::vec3>& locations, vector<Insertion>& insertions, size_t newly_inserted){

//     for(size_t neighbor_idx = 0; neighbor_idx < insertions.size(); ++neighbor_idx){
//         if(neighbor_idx != newly_inserted && !insertions[neighbor_idx].completed){
//             double cost = calc_sq_distance(locations[newly_inserted], locations[neighbor_idx]);
//             if(cost < insertions[neighbor_idx].cost){
//                 insertions[neighbor_idx].cost = cost;
//                 insertions[neighbor_idx].connector = newly_inserted;
//             }
//         }
//     }
// }

// double calc_sq_distance(const glm::vec3& v1, const glm::vec3& v2){
//     float x_diff = v1.x - v2.x;
//     float y_diff = v1.y - v2.y;
//     float z_diff = v1.z - v2.z;
//     return (x_diff * x_diff) + (y_diff * y_diff) + (z_diff + z_diff);
// }
