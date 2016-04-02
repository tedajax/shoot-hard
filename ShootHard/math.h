#pragma once

#include "types.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace math
{
    inline float32 min(float32 _a, float32 _b)
    {
        return (_a < _b) ? _a : _b;
    }

    inline float32 max(float32 _a, float32 _b)
    {
        return (_a > _b) ? _a : _b;
    }

    inline float32 clamp(float32 _value, float32 _min, float32 _max)
    {
        return max(min(_value, _max), _min);
    }

    inline float32 lerp(float32 _a, float32 _b, float32 _t)
    {
        return (_b - _a) * _t + _a;
    }

    namespace matrix
    {
        inline void trs(const glm::vec2& _translation, const float32& _rotation, const glm::vec2& _scale, glm::mat4& _matrixOut)
        {
            auto t = glm::translate(glm::mat4(), glm::vec3(_translation, 0.f));
            auto r = glm::rotate(glm::mat4(), _rotation, glm::vec3(0.f, 0.f, -1.f));
            auto s = glm::scale(glm::mat4(), glm::vec3(_scale, 1.f));
            _matrixOut = t * r * s;
        }

        inline glm::mat4 trs(const glm::vec2& _translation, const float32& _rotation, const glm::vec2& _scale)
        {
            glm::mat4 result;
            trs(_translation, _rotation, _scale, result);
            return result;
        }

        inline void trs(const glm::vec3& _translation, const glm::quat& _rotation, const glm::vec3& _scale, glm::mat4& _matrixOut)
        {
            auto t = glm::translate(glm::mat4(), _translation);
            auto r = glm::mat4_cast(_rotation);
            auto s = glm::scale(glm::mat4(), _scale);
            _matrixOut = t * r * s;
        }

        inline glm::mat4 trs(const glm::vec3& _translation, const glm::quat& _rotation, const glm::vec3& _scale)
        {
            auto t = glm::translate(glm::mat4(), _translation);
            auto r = glm::mat4_cast(_rotation);
            auto s = glm::scale(glm::mat4(), _scale);
            return t * r * s;
        }
    }
}