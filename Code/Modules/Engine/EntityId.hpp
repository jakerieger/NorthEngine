// Author: Jake Rieger
// Created: 11/18/25.
//

#pragma once

#include <Common/Common.hpp>

namespace North::Engine {
    class EntityId {
    public:
        explicit constexpr EntityId(u64 value) : mValue(value) {}

        NE_ND constexpr u64 Value() const {
            return mValue;
        }

        constexpr bool operator==(const EntityId& other) const {
            return mValue == other.mValue;
        }

        constexpr bool operator!=(const EntityId& other) const {
            return mValue != other.mValue;
        }

        constexpr bool operator<(const EntityId& other) const {
            return mValue < other.mValue;
        }

        constexpr bool operator>(const EntityId& other) const {
            return mValue > other.mValue;
        }

        constexpr bool operator<=(const EntityId& other) const {
            return mValue <= other.mValue;
        }

        constexpr bool operator>=(const EntityId& other) const {
            return mValue >= other.mValue;
        }

        constexpr u64 operator*() const {
            return mValue;
        }

        NE_ND constexpr bool Valid() const {
            return mValue != kInvalidEntityId;
        }

        static constexpr EntityId Invalid() {
            return EntityId {kInvalidEntityId};
        }

    private:
        u64 mValue;
        static constexpr u64 kInvalidEntityId = std::numeric_limits<u64>::max();
    };
}  // namespace North::Engine

#ifndef NE_ENTITY_ID_HASH_SPECIALIZATION
    #define NE_ENTITY_ID_HASH_SPECIALIZATION

// Allow EntityId to be used as a key with STL maps/sets
template<>
struct std::hash<North::EntityId> {
    std::size_t operator()(const North::EntityId& id) const noexcept {
        return std::hash<North::u64> {}(id.Value());
    }
};  // namespace std
#endif