#include "types.h"
#include <glm/gtc/matrix_transform.hpp>

namespace camera
{
    inline glm::mat4 view(const Camera& _camera)
    {
        auto rotation = glm::mat4_cast(_camera.rotation);
        auto forward = glm::vec4(0.f, 0.f, -1.f, 0.f) * rotation;
        auto up = glm::vec3(glm::vec4(0.f, 1.f, 0.f, 0.f) * rotation);
        auto target = _camera.position + glm::vec3(forward);
        return glm::lookAt(_camera.position, target, up);
    }

    inline glm::mat4 projection(const Camera& _camera)
    {
        switch (_camera.projectionType) {
            default:
            case ProjectionType::cPerspective:
                return glm::perspective(glm::radians(_camera.fov), _camera.aspectRatio, _camera.nearZ, _camera.farZ);

            case ProjectionType::cOrtho:
                return glm::ortho(-_camera.orthoSize * _camera.aspectRatio, _camera.orthoSize * _camera.aspectRatio, _camera.orthoSize, -_camera.orthoSize, _camera.nearZ, _camera.farZ);
        }
    }

    inline glm::mat4 view_projection(const Camera& _camera)
    {
        return projection(_camera) * view(_camera);
    }

    inline void rotate_angle_axis(Camera& _camera, float32 _angle, glm::vec3 _axis)
    {
        _camera.rotation *= glm::rotate(glm::quat(), glm::radians(_angle), _axis);
    }

    inline void move(Camera& _camera, float32 _amount, glm::vec3 _direction)
    {
        _camera.position += _direction * _amount;
    }
}