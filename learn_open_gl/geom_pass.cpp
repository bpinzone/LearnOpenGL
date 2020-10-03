#include "geom_pass.h"

#include <glad/glad.h>

#include <stdexcept>

#include "hierarchy.h"
#include "texture.h"

using std::shared_ptr;

Geom_pass::Geom_pass(){

    glGenFramebuffers(1, &g_framebuffer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, g_framebuffer_id);

    // Position
    unsigned int g_position_tex_id;
    glGenTextures(1, &g_position_tex_id);
    glBindTexture(GL_TEXTURE_2D, g_position_tex_id);
    // RGBA instead of RGB bc of alignment considerations.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window_width, window_height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_position_tex_id, 0);
    g_position_tex = Texture::construct_texture_from_completed_id(
        g_position_tex_id, "position");

    // Normal
    unsigned int g_normal_tex_id;
    glGenTextures(1, &g_normal_tex_id);
    glBindTexture(GL_TEXTURE_2D, g_normal_tex_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window_width, window_height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, g_normal_tex_id, 0);
    g_normal_tex = Texture::construct_texture_from_completed_id(
        g_normal_tex_id, "normal");

    // Albedo and specular in one texture.
    unsigned int g_albedo_spec_tex_id;
    glGenTextures(1, &g_albedo_spec_tex_id);
    glBindTexture(GL_TEXTURE_2D, g_albedo_spec_tex_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window_width, window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, g_albedo_spec_tex_id, 0);
    g_albedo_spec_tex = Texture::construct_texture_from_completed_id(
        g_albedo_spec_tex_id, "albedo_spec");

    unsigned int attachments[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachments);

    // Renderbuffer for depth and stencil.
    glGenRenderbuffers(1, &g_renderbuffer_id);
    glBindRenderbuffer(GL_RENDERBUFFER, g_renderbuffer_id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window_width, window_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, g_renderbuffer_id);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        throw std::runtime_error{"Geometry pass frame buffer is incomplete!"};
    }
}

void Geom_pass::bind_framebuffer_and_render_start(Hierarchy* hier){
    bind_framebuffer_and_clear();
    hier->render_start();
}

void Geom_pass::bind_framebuffer_and_render_update(Hierarchy* hier){
    bind_framebuffer_and_clear();
    hier->render_update();
}

void Geom_pass::bind_framebuffer_and_clear(){
    // TODO: overall design:
    // TODO: general change here and other places in the code. We don't need to do ClearColor all the time.
    // Make setters for some member variables private, and only do work when they change!
    // Another example: settings shader uniforms. (like texture locations, don't need to do that unless shader changes!)
    glBindFramebuffer(GL_FRAMEBUFFER, g_framebuffer_id);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

shared_ptr<Texture> Geom_pass::get_position_tex() const{
    return g_position_tex;
}
shared_ptr<Texture> Geom_pass::get_normal_tex() const{
    return g_normal_tex;
}
shared_ptr<Texture> Geom_pass::get_albedo_spec_tex() const{
    return g_albedo_spec_tex;
}