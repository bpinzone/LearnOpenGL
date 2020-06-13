int main(){

    // ======================================
    // Generate a FRAME BUFFER
    // You have a frame buffer. You can attach textures or renderbuffers to it.
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    // It is also possible to bind a framebuffer to a read or write target specifically by binding to GL_READ_FRAMEBUFFER or GL_DRAW_FRAMEBUFFER respectively.
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Unfortunately, we can't use our framebuffer yet because it is not complete. For a framebuffer to be complete the following requirements have to be satisfied: ...
    // An attachment is a memory location that can act as a buffer for the frame buffer.
    // Two attachment options: Texture or renderbuffer.

    // A COLOR TEXTURE ATTACHMENT: (more specifically, a color attachment)
    unsigned int color_texture;
    glGenTextures(1, &color_texture);
    glBindTexture(GL_TEXTURE_2D, color_texture);
    // Only allocating data, not initializing.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window_width, window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    // Dont' care about wrapping settings.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    /*
    If you want to render your whole screen to a texture of a smaller or larger size you need to call glViewport again (before rendering to your framebuffer) with the new dimensions of your texture, otherwise render commands will only fill part of the texture.
    */

    // Attach texture to frame buffer.
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture, 0);

    unsigned int depth_stencil_texture;
    glGenTextures(1, &depth_stencil_texture);
    glBindTexture(GL_TEXTURE_2D, depth_stencil_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, window_width, window_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth_stencil_texture, 0);

    // If you want all rendering operations to have a visual impact again on the main window we need to MAKE THE DEFAULT FRAMEBUFFER ACTIVE BY BINDING TO 0:
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE){

    }
    glDeleteFramebuffers(1, &fbo);


    // Generate a RENDER BUFFER ========================
    // Render buffers typically used for depth and stencil buffers, since we write to them and don't sample them.
    // Render buffers are another thing you can attach. They are specifically designed to be an attachment, unlike textures which are a fairly general purpose.
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    // Make this buffer into a depth and stencil buffer.
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window_width, window_height);

    // Attach the render buffer object
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    return 0;

}