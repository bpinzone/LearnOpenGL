#include "texture.h"
#include "stb_image.h"

#include <stdexcept>
#include <sstream>

using std::string;
using std::ostringstream;
using std::runtime_error;

Texture::Texture(const string& image_path) {

    glGenTextures(1, &texture_id);

    // Just like other objects we need to bind it so any
    // subsequent texture commands will configure the currently bound texture:
    glBindTexture(GL_TEXTURE_2D, texture_id);

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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upside down image? This happens because OpenGL expects the 0.0 coordinate on the y-axis to be on the bottom side of the image, but images usually have 0.0 at the top of the y-axis. Luckily for us, stb_image.h can flip the y-axis during image loading by adding the following statment before loading any image:
    stbi_set_flip_vertically_on_load(true);

    // Load and generate the texture
    int width, height, nrChannels; // number of color channels.
    // read texture data from disk using library.
    unsigned char* data = stbi_load(image_path.c_str(), &width, &height, &nrChannels, 0);
    if(!data){
        ostringstream oss;
        oss << "Could not load texture at: " << image_path << "\n";
        throw runtime_error{oss.str()};
    }

    // OK for now.
    bool has_alpha = image_path.substr(image_path.rfind("."), 4) == ".png";

    /*
    Actually generate the texture data
    (
        target, level of mipmap youre creating,
        format of how to store, width, height, legacy 0,
        format we're feeding, data type of src image (how we stored them, unsigned char), actual data.
    )
    */

    // This is operating on texture bound to target GL_TEXTURE_2D, which is "texture_id".
    // Why isn't third parameter "GL_RBGA" ? Looks like we're not ACTUALLY utilizing transparency right now, seeing as the shader uses "mix", aka lerp.
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
        has_alpha? GL_RGBA : GL_RGB,
        GL_UNSIGNED_BYTE, data
    );

    // generate mip maps automatically. Alternatively, do the above a bunch, incrementing 2nd arg.
    glGenerateMipmap(GL_TEXTURE_2D);

    // Bound texture object now has the data in it.
    // free data given by stbi_load
    stbi_image_free(data);
}