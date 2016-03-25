#include "types.h"

namespace camera
{
    glm::mat4 view_matrix(const Camera& _camera);
    glm::mat4 projection_matrix(const Camera& _camera);
}