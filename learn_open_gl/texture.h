#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <glfw3.h>

#include <vector>
#include <utility>
#include <string>
#include <set>
#include <memory>

struct Texture_comp;

class Texture {
public:

    enum class Type {
        DIFFUSE, SPECULAR
    };

    // This class is NOT responsible for managing this. (Would require shared_from_this in ctor.)
    // Although, our ctor will assert that a texture that is about to constructed is not already here.
    static std::set<std::shared_ptr<Texture>, Texture_comp> loaded_textures;


    Texture(const std::string& path_in, Type type_in);

    unsigned int texture_id;

    // Relative to dir main is in. Includes the file name.
    const std::string path;

    // One of "diffuse", "specular"
    Type type;
};

struct Texture_comp {

    using is_transparent = std::true_type;

    bool operator()(
            const std::shared_ptr<const Texture> t1,
            const std::shared_ptr<const Texture> t2) const {
        return t1->path < t2->path;
    }

    bool operator()(
            const std::shared_ptr<const Texture> t,
            const std::string& path) const {
        return t->path < path;
    }

    bool operator()(
            const std::string& path,
            const std::shared_ptr<const Texture> t) const {
        return path < t->path;
    }
};

#endif