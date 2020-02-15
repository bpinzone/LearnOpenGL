#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <glfw3.h>

#include <vector>
#include <utility>
#include <string>

// Really basic texture class.
// Has lots of default settings chosen for you.
class Texture {
public:

    Texture(const std::string& image_path);

    unsigned int texture_id;
};

#endif