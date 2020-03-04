#include "texture.h"
#include "stb_image.h"

#include <stdexcept>
#include <sstream>

using std::string;
using std::ostringstream;
using std::runtime_error;

Texture::Texture(
    const string& image_path,
    const string& type_in,
    const string& file_name_in)
    : type{type_in}, file_name{file_name_in} {

    glGenTextures(1, &texture_id);

    // Upside down image? This happens because OpenGL expects the 0.0 coordinate on the y-axis to be on the bottom side of the image, but images usually have 0.0 at the top of the y-axis. Luckily for us, stb_image.h can flip the y-axis during image loading by adding the following statment before loading any image:
    // stbi_set_flip_vertically_on_load(true);

    int width, height, num_color_channels;
    unsigned char* data = stbi_load(image_path.c_str(), &width, &height, &num_color_channels, 0);
    if(!data){
        stbi_image_free(data);
        throw runtime_error{"Could not load texture at: " + image_path};
    }

    GLenum format;
    switch(num_color_channels){
        case 1 : format = GL_RED; break;
        case 3 : format = GL_RGB; break;
        default : format = GL_RGBA; break;
    }

    // Just like other objects we need to bind it so any
    // subsequent texture commands will configure the currently bound texture:
    glBindTexture(GL_TEXTURE_2D, texture_id);

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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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
}