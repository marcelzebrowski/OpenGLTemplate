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
    vec3 direction;
    float cutOff;
    float outerCutOff;


    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
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

    float theta = dot(lightDirection, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 result;

    float distance  = length(light.position - FragmentPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
       
  //  if(theta > light.cutOff){
    
    
        vec3 viewDirection = normalize(viewPosition - FragmentPosition);
        vec3 reflectDirection = reflect(-lightDirection, norm);

        
        vec3 ambientLight = calculateAmbientLight();
        vec3 diffuseLight = calculateDiffuseColor(norm, lightDirection);
        vec3 specularLight = calculateSpecularLight(viewDirection, reflectDirection);
        
        ambientLight *= attenuation;
        diffuseLight *= attenuation;
        specularLight *= attenuation;



        vec3 emission = vec3(0.0);
        if (texture(material.specular, TexCoords).r == 0.0){
            emission = texture(material.emission, TexCoords /* sin(time) + vec2(0.0,sin(time))*/).rgb;
        }

        ambientLight *= intensity;
        diffuseLight *= intensity;
        specularLight *= intensity;

        result = ambientLight + diffuseLight + specularLight; //+ emission;
  //  }else{
  //      result = calculateAmbientLight() / 2.0f ;
  //  }
        

    
    FragColor = vec4(result, 1.0f); 
}