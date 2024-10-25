#shader vertex
#version 330 core
layout(location = 0) in vec3 position;
out vec2 TexCoord;
void main()
{
    TexCoord = position.xy * 0.5 + 0.5;
    gl_Position = vec4(position, 1.0);
}

#shader pixel
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform float time; // Додаємо час як uniform

void main()
{
    // Зміна кольору за допомогою синусоїди часу
    vec3 color1 = vec3(sin(time * 0.5), 1.0, 0.5);
    vec3 color2 = vec3(1.0, sin(time), 1.0);
    
    // Градієнт між двома кольорами залежить від часу
    vec3 gradient_color = mix(color1, color2, TexCoord.y);
    
    FragColor = vec4(gradient_color, 1.0);
}