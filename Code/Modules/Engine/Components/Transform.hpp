// Author: Jake Rieger
// Created: 11/16/25.
//

#pragma once

#include "Common/Common.hpp"
#include "Math/Constants.hpp"

namespace North::Engine::Components {
    class Transform {
    public:
        Transform() = default;

        NE_ND Vec3 const& GetPosition() const;
        NE_ND Vec3 const& GetRotation() const;
        NE_ND Vec3 const& GetScale() const;
        NE_ND Mat4x4 const& GetModelMatrix();

        void SetPosition(const Vec3& position);
        void SetPosition(f32 x, f32 y, f32 z);
        void SetRotation(const Vec3& rotation);
        void SetRotation(f32 x, f32 y, f32 z);
        void SetScale(const Vec3& scale);
        void SetScale(f32 x, f32 y, f32 z);

        void Translate(const Vec3& translation);
        void RotateEuler(const Vec3& eulerAngles);
        void RotateAxisEuler(f32 degrees, const Vec3& axis);
        void Scale(const Vec3& scale);

    private:
        Vec3 mPosition {0, 0, 0};
        Vec3 mRotation {0, 0, 0};
        Vec3 mScale {1, 1, 1};
        Mat4x4 mModelMatrix {Math::Constants::kIdentity4x4};
        bool mDirty {false};
    };
}  // namespace North::Engine::Components
