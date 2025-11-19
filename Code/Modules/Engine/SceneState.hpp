// Author: Jake Rieger
// Created: 11/18/25.
//

#pragma once

#include "Common/Common.hpp"
#include "../Vendor/entt.hpp"  // TODO: Fix include path so I don't have to do relative paths, bug with CMake ?

namespace North::Engine {
    using Entity = entt::entity;

    class SceneState {
        friend class Scene;

    public:
        SceneState() = default;

        static SceneState FromFile(const fs::path& filename);
        void ToFile(const fs::path& filename);

        Entity CreateEntity();
        void DestroyEntity(Entity entity);

        template<typename Component, typename... Args>
        Component& AddComponent(Entity entity, Args&&... args) {
            return mRegistry.emplace<Component>(entity, std::forward<Args>(args)...);
        }

        template<typename Component>
        Component& GetComponent(Entity entity) {
            return mRegistry.get<Component>(entity);
        }

        template<typename... Components>
        auto View() {
            return mRegistry.view<Components...>();
        }

    private:
        entt::registry mRegistry {};
    };
}  // namespace North::Engine
