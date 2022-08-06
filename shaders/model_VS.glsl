#version 410
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

// uniform mat4 model;
// uniform mat4 view;
// uniform mat4 projection;
uniform mat4 projection_view_model;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = projection_view_model * vec4(aPos, 1.0);
    // gl_Position = projection * view * model * vec4(aPos, 1.0);
}
