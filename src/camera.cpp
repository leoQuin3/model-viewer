#include <iostream>
#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up) : position(position), front(front), up(up), worldUp(up)
{
    updateCameraVectors(front);
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = getSpeed() * deltaTime;

    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;
    if (direction == UP)
        position += up * velocity;
    if (direction == DOWN)
        position -= up * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constraintPitch)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (constraintPitch) {
        if (pitch > 89.f)
            pitch = 89.f;
        if (pitch < -89.f)
            pitch = -89.f;
    }

    updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
    fov -= (float)yoffset;
    if (fov < MIN_FOV)
        fov = MIN_FOV;
    if (fov > MAX_FOV)
        fov = MAX_FOV;
}

void Camera::lookAtPosition(glm::vec3 position)
{
    glm::vec3 lookDir = glm::normalize(position - this->position);
    updateCameraVectors(lookDir);
}

glm::mat4 Camera::getViewMatrix() const
{
    glm::mat4 translation = glm::mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        -position.x, -position.y, -position.z, 1
    );
    glm::mat4 rotation = glm::mat4(
        right.x, up.x, -front.x, 0,
        right.y, up.y, -front.y, 0,
        right.z, up.z, -front.z, 0,
        0, 0, 0, 1
    );
    return rotation * translation;
}

glm::vec3 Camera::getFront() const
{
    return front;
}

glm::vec3 Camera::getRight() const
{
    return this->right;
}

glm::vec3 Camera::getUp() const
{
    return this->up;
}

float Camera::getSpeed() const
{
    if (isSprinting)
        return maxSpeed;
    else
        return movementSpeed;
}

void Camera::updateCameraVectors()
{
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::updateCameraVectors(glm::vec3 front)
{
    this->front = glm::normalize(front);
    right = glm::normalize(glm::cross(this->front, worldUp));
    up = glm::normalize(glm::cross(right, this->front));

    pitch = asin(this->front.y);
    yaw = atan2f(this->front.x, this->front.z);
}