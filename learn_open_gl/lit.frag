#version 330 core

in vec3 Normal;
in vec3 WorldPos;
in vec2 TexCoords;

uniform vec3 view_pos;

struct Material {
    // colors reflected for each light component
    sampler2D diffuse;  // serves as diffuse AND ambient color.
    sampler2D specular;
    sampler2D emission;
    float shininess;
};
uniform Material material;

struct Light {
    vec3 position;
    vec3 direction;
    float inner_cutoff;
    float outer_cutoff;

    // colors shone for each light component.
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform Light light;

out vec4 FragColor;

void main() {

    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 frag_to_light = normalize(light.position - WorldPos);
    float diff = max(dot(norm, frag_to_light), 0.0);
    // texture() returns a vec4, cut off last value.
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // specular
    vec3 frag_to_camera = normalize(view_pos - WorldPos);
    vec3 light_to_frag = (WorldPos - light.position);
    vec3 light_ref = normalize(reflect(light_to_frag, norm));
    float spec = pow(max(dot(light_ref, frag_to_camera), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    // emission
    vec3 specular_pixel = vec3(texture(material.specular, TexCoords));
    float specular_distance = specular_pixel.x - 0.5f;
    specular_distance *= -1;
    vec3 emission = 0.6f *
        specular_distance *
        vec3(texture(material.emission, TexCoords));

    // Attenuation
    float distance = length(light_to_frag);
    float attenuation = 1.0 / (
        light.constant +
        light.linear * distance +
        light.quadratic * (distance * distance)
    );

    // spotlight
    // Represents theta in the picture.
    float spot_alignment = dot(normalize(light_to_frag), normalize(light.direction));
    // Represents the "size" of the area between the cutoff points.
    float epsilon = light.inner_cutoff - light.outer_cutoff;
    // Fraction represents how much of episilon you have.
    // Fragments inside the inner cutoff have values above one.
    // Fragments outside the outer cutoff have values below zero.
    // Subtraction removes the portion outside the outer_cutoff, as it doesn't count towards you getting
    // into the inner ring.
    float intensity = clamp((spot_alignment - light.outer_cutoff) / epsilon, 0.0, 1.0);

    // put together
    vec3 total_light = ambient + (intensity * diffuse) + (intensity * specular);
    total_light *= attenuation;

    FragColor = vec4(total_light, 1.0);

}