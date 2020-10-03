#include "texture.h"
#include "stb_image.h"

#include <stdexcept>
#include <sstream>
#include <cassert>

using std::string;
using std::runtime_error;
using std::set;
using std::shared_ptr;
using std::vector;

struct Texture_comp {

    using is_transparent = std::true_type;

    bool operator()(
            const shared_ptr<const Texture> t1,
            const shared_ptr<const Texture> t2) const {
        return t1->get_paths() < t2->get_paths();
    }

    bool operator()(
            const shared_ptr<const Texture> t,
            const vector<string>& paths) const {
        return t->get_paths() < paths;
    }

    bool operator()(
            const vector<string>& paths,
            const shared_ptr<const Texture> t) const {
        return paths < t->get_paths();
    }
};


// Avoid loading the same textures over and over again.
// Identified by paths
static set<shared_ptr<Texture>, Texture_comp> loaded_textures;


shared_ptr<Texture> Texture::construct_texture_from_img_file(
        const string& _path,
        const string& _sampler_base_identifier,
        bool flip_vertically){

    vector<string> _paths{_path};

    // Make sure we haven't already loaded it.
    auto already_loaded_it = loaded_textures.find(_paths);
    if(already_loaded_it != loaded_textures.end()){
        return *already_loaded_it;
    }

    unsigned int new_texture_id;
    glGenTextures(1, &new_texture_id);

    // Upside down image? This happens because OpenGL expects the 0.0 coordinate on the y-axis to be on the bottom side of the image, but images usually have 0.0 at the top of the y-axis. Luckily for us, stb_image.h can flip the y-axis during image loading by adding the following statement before loading any image:
    // potential problem:
    stbi_set_flip_vertically_on_load(flip_vertically);

    int width, height, num_color_channels;
    unsigned char* data = stbi_load(_path.c_str(), &width, &height, &num_color_channels, 0);
    if(!data){
        stbi_image_free(data);
        throw runtime_error{"Could not load texture at: " + _path};
    }

    GLenum format;
    switch(num_color_channels){
        case 1 : format = GL_RED; break;
        case 3 : format = GL_RGB; break;
        default : format = GL_RGBA; break;
    }

    // Just like other objects we need to bind it so any
    // subsequent texture commands will configure the currently bound texture:
    glBindTexture(GL_TEXTURE_2D, new_texture_id);

    /*
    Actually generate the texture data
    (
        target, level of mipmap youre creating,
        format of how to store, width, height, legacy 0,
        format we're feeding, data type of src image (how we stored them, unsigned char), actual data.
    )
    */
    // This is operating on texture bound to target GL_TEXTURE_2D, which is "texture_id".
    glTexImage2D(
        GL_TEXTURE_2D, 0, format, width, height,
        0, format, GL_UNSIGNED_BYTE, data
    );

    // generate mip maps automatically. Alternatively, do the above a bunch, incrementing 2nd arg.
    glGenerateMipmap(GL_TEXTURE_2D);

    // WRAPPING OPTIONS for the currently bound texture object:
    // (texture target, specify axis, mode for how that axis should wrap)
    // Defines how UV coordinates behave when out of range.
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    /*
    // specify the border color in case "clamp to border" wrap mode is used.
    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    */

    // Filtering options for the cur bound texture object.
    // (texture 2D, when to use this mode <upscaling vs downscaling>, what filter mode to use.)
    // Has to do with scaling the texture onto an object.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Bound texture object now has the data in it.
    // free data given by stbi_load
    stbi_image_free(data);

    // bc ctor private;
    return shared_ptr<Texture>{new Texture(
        new_texture_id, _sampler_base_identifier, _paths)};

}


shared_ptr<Texture> Texture::construct_cube_texture_from_img_file(
        const vector<string>& _paths,
        const string& _sampler_base_identifier){

    unsigned int new_texture_id;

    glGenTextures(1, &new_texture_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, new_texture_id);

    // Upside down image? This happens because OpenGL expects the 0.0 coordinate on the y-axis to be on the bottom side of the image, but images usually have 0.0 at the top of the y-axis. Luckily for us, stb_image.h can flip the y-axis during image loading by adding the following statment before loading any image:
    // potential problem:
    stbi_set_flip_vertically_on_load(false);

    for(GLuint i = 0; i < _paths.size(); ++i) {
        int width, height, num_color_channels;
        unsigned char* data;
        data = stbi_load(_paths[i].c_str(), &width, &height, &num_color_channels, 0);
        if(!data){
            stbi_image_free(data);
            throw std::runtime_error{"Failed to load texture: " + _paths[i]};
        }
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
        );
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // ctor private
    return shared_ptr<Texture>{new Texture(
        new_texture_id, _sampler_base_identifier, _paths)};
}

shared_ptr<Texture> Texture::construct_texture_from_completed_id(
        unsigned int completed_texture_id,
        const string& _sampler_base_identifier){

    // ctor private
    return shared_ptr<Texture>{new Texture(
        completed_texture_id, _sampler_base_identifier, vector<string>{})};
}


const string& Texture::get_sampler_base_identifier() const {
    return sampler_base_identifier;
}

unsigned int Texture::get_id() const {
    return texture_id;
}

const vector<string>& Texture::get_paths() const{
    return paths;
}


// Private constructor
Texture::Texture(
        unsigned int _texture_id,
        const string& _sampler_base_identifier,
        const vector<string>& _paths)

    : texture_id{_texture_id},
    sampler_base_identifier{_sampler_base_identifier},
    paths{_paths} {
}
