#pragma once

#include <glm/glm.hpp>

namespace ZenEngine
{
    namespace Math
    {
        bool DecomposeMatrix(const glm::mat4 &inMatrix, glm::vec3 &outTranslation, glm::vec3 &outRotation, glm::vec3 &outScale);
        bool DecomposeMatrix(const glm::mat4 &inMatrix, glm::vec3 &outTranslation, glm::quat &outRotation, glm::vec3 &outScale);
    }
}