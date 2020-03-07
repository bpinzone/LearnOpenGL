#ifndef COMPONENT_H
#define COMPONENT_H

class Gameobject;

#include <memory>

class Component {

public:

    virtual void start();
    virtual void update();

    // Called by Gameobject when this component is added.
    void set_game_object(std::shared_ptr<Gameobject> game_object_in);

    virtual ~Component() = default;

protected:

    std::shared_ptr<Gameobject> game_object;

private:

};

#endif