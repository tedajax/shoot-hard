#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace camera
{
    glm::mat4 view_matrix(const Camera& _camera)
    {
        return glm::lookAt(_camera.position, _camera.lookat, _camera.up);
    }

    glm::mat4 projection_matrix(const Camera& _camera)
    {
        switch (_camera.projectionType) {
            default:
            case ProjectionType::cPerspective:
                return glm::perspective(glm::radians(_camera.fov), _camera.aspectRatio, _camera.nearZ, _camera.farZ);

            case ProjectionType::cOrtho:
                return glm::ortho(-_camera.orthoSize * _camera.aspectRatio, _camera.orthoSize * _camera.aspectRatio, _camera.orthoSize, -_camera.orthoSize);
        }
    }
}