#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <glfw3.h>

#include <vector>
#include <utility>
#include <string>

// Done
class Texture {
public:

    enum class Type {
        DIFFUSE, SPECULAR
    };

    Texture(const std::string& path_in, Type type_in);

    unsigned int texture_id;

    // Relative to dir main is in. Includes the file name.
    std::string path;

    // One of "diffuse", "specular"
    Type type;
};

#endif