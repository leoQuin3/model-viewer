#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

out vec4 vertexColor;

// Transformations
uniform mat4 modelMat;
uniform mat4 worldMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

void main()
{
    gl_Position = projectionMat * viewMat * worldMat * modelMat * vec4(aPos, 1.0);
    vertexColor = vec4(aNorm, 1.0);
}