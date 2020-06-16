#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal;

out vec3 TexCoords; // vec3 instead of vec2

void main(){

    // Use cube's position as texture coords. Interesting.
    TexCoords = aPos;

    // Remove translation.
    mat4 no_translation_view = mat4(mat3(view));

    // NOTE: No model in computation.
    vec4 max_depth_pos = projection * no_translation_view * vec4(aPos, 1.0);

    // Since cube is just 1x1x1 in scale, we need to trick the depth buffer into believing that the skybox has the maximum depth value of 1.0 so that it fails the depth test wherever there's a different object in front of it.
    // Perspective div about to happen, so the resulting normalized device coordinates will then always have a z value equal to 1.0: the maximum depth value
    max_depth_pos = max_depth_pos.xyww;

    gl_Position = max_depth_pos;
}