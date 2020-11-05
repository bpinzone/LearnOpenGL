#version 330 core
layout (location = 0) in vec3 aPos;
// Other vertex inputs will be populated here, but its fine that we don't use them.

uniform mat4 model;
uniform mat3 normal;

// Its fine that we don't have the std140 uniform block.

uniform mat4 world_space_to_light_clip;

void main(){
    gl_Position = world_space_to_light_clip * model * vec4(aPos, 1.0);
}