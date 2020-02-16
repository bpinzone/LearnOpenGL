
    // ===== where to put?
    // texture target, specify axis, mode for how that axis should wrap.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    // specify the border color in case "clamp to border" wrap mode is used.
    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // texture filtering
    // (texture 2D, when to use this mode <upscaling vs downscaling>, what filter mode to use.)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // generating mipmaps
    // OpenGL is able to do all the work for us with a single call to glGenerateMipmaps after we've created a texture.

    // you can also filter between mipmap levels. This replaces one of the options above.
    // Here, you're specifying the same info as before, but also that you want to use a mipmap.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // A common mistake is to set one of the mipmap filtering options as the magnification filter. This doesn't have any effect since mipmaps are primarily used for when textures get downscaled


    // load a texture
    int width, height, num_color_channels;
    unsigned char *data = stbi_load("wood_container.jpg", &width, &height, &num_color_channels, 0);

    // generate texture object.
    unsigned int texture_id;
    glGenTextures(1, &texture_id);
    // Just like other objects we need to bind it so any subsequent texture commands will configure the currently bound texture:
    // SO, THE CODE ABOVE WILL NOT ACTUALLY DO ANYTHING!
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // Generate actual texture.
    // (target, level of mipmap youre creating, format of how to store, width, height, legacy 0,
    // format we're feeding, data type of src image (how we stored them, unsigned char), actual data)
    // This is operating on texture bound to target GL_TEXTURE_2D, which is "texture".
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    // generate mip maps automatically. Alternatively, do the above a bunch, incrementing 2nd arg.
    glGenerateMipmap(GL_TEXTURE_2D);
    // bound texture object now has the data in it.
    // free data given by stbi_load
    stbi_image_free(data);