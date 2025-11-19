// Author: Jake Rieger
// Created: 11/18/25.
//

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <optional>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace North {
    using u8   = uint8_t;
    using u16  = uint16_t;
    using u32  = uint32_t;
    using u64  = uint64_t;
    using uptr = uintptr_t;

    using i8   = int8_t;
    using i16  = int16_t;
    using i32  = int32_t;
    using i64  = int64_t;
    using iptr = intptr_t;

#if defined(__GNUC__) || defined(__clang__)
    using u128 = __uint128_t;
    using i128 = __int128_t;
#endif

    using f32 = float;
    using f64 = double;

    using std::make_shared;
    using std::make_unique;
    using std::optional;
    using std::shared_ptr;
    using std::string;
    using std::unique_ptr;
    using std::vector;
    using std::weak_ptr;
    namespace fs = std::filesystem;

    using Vec2   = glm::vec2;
    using Vec3   = glm::vec3;
    using Vec4   = glm::vec4;
    using Mat4x4 = glm::mat4x4;
}  // namespace North