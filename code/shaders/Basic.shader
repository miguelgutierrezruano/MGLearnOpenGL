
// Distributed under MIT License
// @miguelgutierrezruano
// 2023

#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 modelViewProjection;

void main()
{
    v_TexCoord = texCoord;
    gl_Position = modelViewProjection * position;
}

#shader fragment
#version 330 core

out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = texColor * u_Color;
}
        