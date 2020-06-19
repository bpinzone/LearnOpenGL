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

    // Texture units in shaders should be NAMED: <lowercase type name>#
    // Example: "diffuse1", "specular2"
    enum class Type {
        DIFFUSE, SPECULAR, CUBE
    };

    // This class is NOT responsible for managing this. (Would require shared_from_this in ctor.)
    // Although, our ctor will assert that a texture that is about to constructed is not already here.
    // NOTE: Could make a static factory function, and manage this ourselves. Then client would call
    // factory instead of Ctor, which we could make private.
    static std::set<std::shared_ptr<Texture>, Texture_comp> loaded_textures;


    // Make a diffuse or specular texture. Type must be DIFFUSE or SPECULAR.
    // TODO: Poor design. Switching variable
    Texture(const std::string& path_in, Type type_in, bool flip_vertically);

    // Make a cube map texture. Type must be CUBE.
    // Paths must be in this open gl defined ordered:
    // POSITIVE_X Right, NEGATIVE_X Left, POSITIVE_Y Top, NEGATIVE_Y Bottom, POSITIVE_Z Back, NEGATIVE_Z Front
    Texture(const std::vector<std::string> paths, Type type_in);

    Texture(unsigned int completed_texture_id, Type type_in);

    unsigned int texture_id;

    // Relative to dir main is in. Includes the file name.
    // empty for cube maps.
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