#version 330 core
in vec2 TexCoord;

out vec4 FragColor;

// This is just set to a slot number, and that slot is populated with the texture elsewhere.
// When you "set" these with glUniform, you are setting the "texture UNIT" to retrieve this data from!
uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    // mix = lerp.
    FragColor = mix(
        texture(texture1, TexCoord),
        texture(texture2, TexCoord), 0.3
    );
}