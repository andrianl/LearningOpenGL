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

void main()
{
    vec3 gradient_color = vec3(1.0, 1.0, 0.5) * (1.0 - TexCoord.y) + vec3(1.0, 0.0, 1.0) * TexCoord.y;
    FragColor = vec4(gradient_color, 1.0);
};


