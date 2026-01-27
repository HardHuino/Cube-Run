#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D diffuse_map;
uniform vec3 emissiveColor; // Couleur d'émission de la pyramide

void main()
{
    vec4 texColor = texture(diffuse_map, TexCoord);
    
    // La pyramide émet sa propre lumière - très brillante
    vec3 emissive = emissiveColor * 1.5; // Multiplié pour un effet plus intense
    
    // Mélange la texture avec la couleur émissive
    vec3 finalColor = texColor.rgb * emissive;
    
    FragColor = vec4(finalColor, 1.0);
}
