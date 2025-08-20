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
    // Mirror matrix across z axis
    mat4 mirrorMat = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0,-1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    gl_Position = projectionMat * viewMat * mirrorMat * worldMat * modelMat * vec4(aPos, 1);

    vertexColor = (transpose(inverse(modelMat)) * vec4(aNorm, 0));
}