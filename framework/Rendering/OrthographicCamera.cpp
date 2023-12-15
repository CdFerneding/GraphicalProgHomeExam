#include "OrthographicCamera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

OrthographicCamera::OrthographicCamera(const OrthographicCamera::Frustrum &frustrum, const glm::vec3 &position,
                                       float rotation) : Camera() {
    this->CameraFrustrum = frustrum;
    this->Position = position;
    this->Rotation = rotation;
    OrthographicCamera::RecalculateMatrix();
}

void OrthographicCamera::RecalculateMatrix() {
    this->ProjectionMatrix = glm::ortho(this->CameraFrustrum.left, this->CameraFrustrum.right,
                                        this->CameraFrustrum.bottom, this->CameraFrustrum.top,
                                        this->CameraFrustrum.near, this->CameraFrustrum.far);
    this->ViewMatrix = glm::translate(glm::mat4(1.0f), this->Position) *
                       glm::rotate(glm::mat4(1.0f), glm::radians(this->Rotation), glm::vec3(0.0f, 0.0f, 1.0f));

    this->ViewProjectionMatrix = glm::mat4(1.0f);
}