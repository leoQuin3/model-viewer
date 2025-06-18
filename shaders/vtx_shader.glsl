#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec4 vertexColor;

uniform mat4 rotateMat;

void main()
{
    gl_Position = rotateMat * vec4(aPos, 1.0);
    vertexColor = vec4(aColor, 1.0);
}