// Author: Jake Rieger
// Created: 11/18/25.
//

#include "SceneState.hpp"

namespace North::Engine {
    SceneState SceneState::FromFile(const fs::path& filename) {
        return {};
    }

    void SceneState::ToFile(const fs::path& filename) {}

    Entity SceneState::CreateEntity() {
        return mRegistry.create();
    }

    void SceneState::DestroyEntity(Entity entity) {
        mRegistry.destroy(entity);
    }
}  // namespace North::Engine