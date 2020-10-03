#version 330 core

// material constructed from this shader will be used to render a post processing quad.

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out VS_OUT {
    vec2 TexCoord;
} vs_out;

void main() {
    gl_Position = vec4(aPos, 1);
    vs_out.TexCoord = aTexCoord;
}