// Author: Jake Rieger
// Created: 11/16/25.
//

#include "Transform.hpp"

namespace North::Engine::Components {
    Vec3 const& Transform::GetPosition() const {
        return mPosition;
    }

    Vec3 const& Transform::GetRotation() const {
        return mRotation;
    }

    Vec3 const& Transform::GetScale() const {
        return mScale;
    }

    Mat4x4 const& Transform::GetModelMatrix() {
        if (mDirty) {
            mModelMatrix = glm::translate(mModelMatrix, mPosition);
            mModelMatrix = glm::rotate(mModelMatrix, glm::radians(mRotation.x), Math::Constants::kAxis_X);
            mModelMatrix = glm::rotate(mModelMatrix, glm::radians(mRotation.y), Math::Constants::kAxis_Y);
            mModelMatrix = glm::rotate(mModelMatrix, glm::radians(mRotation.z), Math::Constants::kAxis_Z);
            mModelMatrix = glm::scale(mModelMatrix, mScale);
            mDirty       = false;
        }

        return mModelMatrix;
    }

    void Transform::SetPosition(const Vec3& position) {
        mPosition = position;
        mDirty    = true;
    }

    void Transform::SetPosition(f32 x, f32 y, f32 z) {
        SetPosition({x, y, z});
    }

    void Transform::SetRotation(const Vec3& rotation) {
        mRotation = rotation;
        mDirty    = true;
    }

    void Transform::SetRotation(f32 x, f32 y, f32 z) {
        SetRotation({x, y, z});
    }

    void Transform::SetScale(const Vec3& scale) {
        mScale = scale;
        mDirty = true;
    }

    void Transform::SetScale(f32 x, f32 y, f32 z) {
        SetScale({x, y, z});
    }

    void Transform::Translate(const Vec3& translation) {
        mPosition += translation;
        mDirty = true;
    }

    void Transform::RotateEuler(const Vec3& eulerAngles) {
        mRotation += eulerAngles;
        mDirty = true;
    }

    void Transform::RotateAxisEuler(f32 degrees, const Vec3& axis) {
        const Vec3 angles = axis * degrees;
        RotateEuler(angles);
    }

    void Transform::Scale(const Vec3& scale) {
        mScale += scale;
        mDirty = true;
    }
}  // namespace North::Engine::Components