#version 460

#define MAX_TEXTURES 32

layout(location = 0) in vec2 i_uv;
layout(location = 1) in vec4 i_col;
layout(location = 2) in float inIsTex;
layout(location = 3) in flat int inIndex;

layout(location = 0) out vec4 fragmentColor;

struct fMatrix
{
    int texIndex;
};

layout(std140, binding = 1) uniform fUniformMatrix
{
    fMatrix f_matrix[MAX_TEXTURES];
};

// uniform sampler2D tex[MAX_TEXTURES];

void main()
{
    // vec4 texColor = mix(i_col, texture(tex[f_matrix[inIndex].texIndex], i_uv), inIsTex);
    vec4 texColor = i_col;

    if (texColor.a < 0.5) discard;
    fragmentColor = i_col * texColor;

    // if(inIsTex == 1.0)
    // {
    //     fragmentColor = i_col * texture(tex[inTexIndex], i_uv).rgba;
    // }
    // else
    // {
    //     fragmentColor = i_col;
    // }
}