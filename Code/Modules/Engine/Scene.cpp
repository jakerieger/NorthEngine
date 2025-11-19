// Author: Jake Rieger
// Created: 11/18/25.
//

#include "Scene.hpp"

namespace North::Engine {
    bool Scene::LoadFromFile(const fs::path& filrname) {
        return false;
    }

    void Scene::Draw(Graphics::RenderContext& renderContext) {}

    void Scene::Awake() {}

    void Scene::Update(f32 dT) {}

    void Scene::LateUpdate() {}

    void Scene::Destroyed() {}
}  // namespace North::Engine