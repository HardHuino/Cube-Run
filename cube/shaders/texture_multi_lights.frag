#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;
in vec2 TexCoord;

uniform sampler2D diffuse_map; 
uniform vec3 lightColor;

// Lumières des pyramides (maximum 10 pyramides)
#define MAX_PYRAMID_LIGHTS 10
uniform int numPyramidLights;
uniform vec3 pyramidLightPositions[MAX_PYRAMID_LIGHTS];
uniform vec3 pyramidLightColors[MAX_PYRAMID_LIGHTS];

void main()
{
    // On calcule la distance de l'objet par rapport à la caméra
    float distance = length(FragPos);

    // GESTION SPÉCIALE DU CIEL (Pour ne pas illuminer le ciel)
    if(distance > 300.0) {
        FragColor = texture(diffuse_map, TexCoord);
        return;
    }

    // LUMIÈRE PRINCIPALE (soleil/lumière globale) 
    
    // Ambient
    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * lightColor;
  
    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(-FragPos); 
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
    
    vec4 texColor = texture(diffuse_map, TexCoord);
    vec3 mainLighting = (ambient + diffuse + specular) * texColor.rgb;

    // LUMIÈRES DES PYRAMIDES (Point Lights) 
    vec3 pyramidLighting = vec3(0.0);
    
    for(int i = 0; i < numPyramidLights; i++) {
        vec3 pyramidPos = pyramidLightPositions[i];
        vec3 pyramidColor = pyramidLightColors[i];
        
        // Distance entre le fragment et la pyramide
        float dist = length(pyramidPos - FragPos);
        
        // Atténuation: lumière diminue avec distance
        float attenuation = 1.0 / (1.0 + 0.09 * dist + 0.032 * dist * dist);
        
        // Direction de la lumière de la pyramide
        vec3 pyramidLightDir = normalize(pyramidPos - FragPos);
        
        // Diffuse pyramide
        float pyramidDiff = max(dot(norm, pyramidLightDir), 0.0);
        vec3 pyramidDiffuse = pyramidDiff * pyramidColor * attenuation;
        
        // On ajoute cette lumière au total
        pyramidLighting += pyramidDiffuse * 50; // Augmente l'intensité de 500%
    }
    
    // Combinaison de toutes les lumières
    vec3 lightingResult = mainLighting + (pyramidLighting * texColor.rgb);

    // BROUILLARD (Pour ne pas afficher les lumière si elles sont trop loin, peut-etre pas nécéssaire)
    float density = 0.015; 
    vec3 fogColor = vec3(0.05, 0.05, 0.1); 

    float fogFactor = 1.0 / exp(pow(distance * density, 2));
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    vec3 finalColor = mix(fogColor, lightingResult, fogFactor);
    
    FragColor = vec4(finalColor, 1.0);
}
