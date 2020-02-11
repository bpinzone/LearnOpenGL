#version 330 core
// in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform float intensity;

// This is not a uniform we set in the shader.
// This will be whatever texture is bound to GL_TEXTURE_2D at draw time.
// When you "set" these with glUniform, you are setting the "texture UNIT" to retrieve this data from!
uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    // mix = lerp.
    FragColor = mix(
        texture(texture1, TexCoord),
        texture(texture2, TexCoord), 0.3)
        // * vec4(ourColor, 1.0)
        * intensity;
}