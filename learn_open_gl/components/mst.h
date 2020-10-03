#ifndef MST_H
#define MST_H

#include "../game_object.h"
#include "connector.h"

#include <glm/glm.hpp>

#include <utility>
#include <vector>
#include <memory>

class MST_coordinator : public Component {
public:

    using Game_objects_t = std::vector<std::shared_ptr<Gameobject>>;

    MST_coordinator(
        Game_objects_t& spheres_in,
        Game_objects_t& cubes_in
    );

    void start() override;
    void update() override;

    // Calculate the MST and make assignments for the
    // cubes' Connector components.
    void coordinate();

private:

    struct Insertion{
        size_t connector;
        double cost;
        bool completed;
    };

    Game_objects_t spheres;
    std::vector<std::shared_ptr<Connector>> connectors;

    // Scratch data
    std::vector<Insertion> insertions;
    std::vector<glm::vec3> locations;
    std::vector<std::pair<size_t, size_t>> edges;

    void update_neighbors(size_t newly_inserted);
};


#endif
