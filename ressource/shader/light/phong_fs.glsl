#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

in vec2 TexCoords;

uniform Material material;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform vec3 viewPosition;
uniform float time;

in vec3 Normal;
in vec3 FragmentPosition;

out vec4 FragColor;

vec3 calculateDiffuseColor(vec3 normal, vec3 lightDirection){
    float diffuse = max(dot(normal,lightDirection),0.0f);
    return light.diffuse * (diffuse * vec3(texture(material.diffuse, TexCoords)));
}

vec3 calculateSpecularLight(vec3 viewDirection, vec3 reflectDirection){
    float spec = pow(max(dot(viewDirection, reflectDirection),0.0f),material.shininess);
    return light.specular * (spec * vec3(texture(material.specular, TexCoords)));
}

vec3 calculateAmbientLight(){
    return light.ambient * vec3(texture(material.diffuse, TexCoords));
}

void main(){

    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(light.position - FragmentPosition);
    vec3 viewDirection = normalize(viewPosition - FragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, norm);

    vec3 ambientLight = calculateAmbientLight();
    vec3 diffuseLight = calculateDiffuseColor(norm, lightDirection);
    vec3 specularLight = calculateSpecularLight(viewDirection, reflectDirection);

    vec3 emission = vec3(0.0);
    if (texture(material.specular, TexCoords).r == 0.0){   /*rough check for blackbox inside spec texture */
        /*apply emission texture */
       // emission = texture(material.emission, TexCoords).rgb;
        
        /*some extra fun stuff with "time uniform" */
        emission = texture(material.emission, TexCoords * sin(time) + vec2(0.0,sin(time))).rgb;   /*moving */
        //emission = emission * (sin(time) * 0.5 + 0.5) * 2.0;                     /*fading */
    }

    //vec3 emission = vec3(texture(material.emission, TexCoords));       
    //emission = emission * vec3(texture(material.specular, TexCoords));

    vec3 result = ambientLight + diffuseLight + specularLight + emission;
    FragColor = vec4(result, 1.0f); 
}