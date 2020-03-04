#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <glfw3.h>

#include <vector>
#include <utility>
#include <string>

// basic texture class.
// Has lots of default settings chosen for you.
class Texture {
public:

    Texture(const std::string& image_path, const std::string& type_in, const std::string& file_name_in);

    unsigned int texture_id;

    // One of "diffuse", "specular"
    std::string type;

    // Not a full path????
    // This might actually be a path?
    std::string file_name;
};

#endif