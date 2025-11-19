// Author: Jake Rieger
// Created: 11/18/25.
//

#pragma once

#include <Common/Typedefs.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace North::Math::Constants {
    static constexpr Mat4x4 kIdentity4x4 = glm::identity<Mat4x4>();
    static constexpr Vec3 kAxis_X        = {1, 0, 0};
    static constexpr Vec3 kAxis_Y        = {0, 1, 0};
    static constexpr Vec3 kAxis_Z        = {0, 0, 1};
    static constexpr Vec3 kAxis_All      = {1, 1, 1};
}  // namespace North::Math::Constants