#version 330 core

in vec3 Normal;
in vec3 WorldPos;

uniform vec3 object_color;

uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 view_pos;

out vec4 FragColor;

void main() {

    float ambient_strength = 0.1f;
    vec3 ambient = ambient_strength * light_color;

    vec3 norm = normalize(Normal);
    vec3 frag_to_light = normalize(light_pos - WorldPos);
    float diffuse_strength = max(dot(norm, frag_to_light), 0.0);
    vec3 diffuse = diffuse_strength * light_color;

    float specular_strength = 0.5f;
    vec3 frag_to_camera = normalize(view_pos - WorldPos);
    vec3 light_to_frag = (WorldPos - light_pos);
    vec3 light_ref = normalize(reflect(light_to_frag, norm));

    float spec = pow(max(dot(light_ref, frag_to_camera), 0.0), 32);
    vec3 specular = specular_strength * spec * light_color;

    vec3 total_light = ambient + diffuse + specular;

    vec3 result = total_light * object_color;
    FragColor = vec4(result, 1.0);

}