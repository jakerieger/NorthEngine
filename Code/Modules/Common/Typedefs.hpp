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
}  // namespace North