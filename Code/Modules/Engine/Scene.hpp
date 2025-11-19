// Author: Jake Rieger
// Created: 11/18/25.
//

#pragma once

#include "SceneState.hpp"
#include "SceneParser.hpp"
#include "Graphics/RenderContext.hpp"

namespace North::Engine {
    class Scene {
    public:
        Scene() = default;

        bool LoadFromFile(const fs::path& filrname);
        // bool LoadFromDescriptor(struct SceneDescriptor& descriptor);

        void Draw(Graphics::RenderContext& renderContext);

        void Awake();
        void Update(f32 dT);
        void LateUpdate();
        void Destroyed();

    private:
        SceneState mState;
    };
}  // namespace North::Engine
