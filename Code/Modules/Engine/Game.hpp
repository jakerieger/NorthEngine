// Author: Jake Rieger
// Created: 11/18/25.
//

#pragma once

#include "Common/Common.hpp"
#include "Graphics/RenderContext.hpp"

namespace North::Engine {
    class Game {
    public:
        Game() = default;

        void Initialize(GLFWwindow* window, u32 width, u32 height);
        void Shutdown();
        void RequestFrame();
        void Resize(u32 width, u32 height);

        NE_ND bool Initialized() const;

        void Awake();
        void Update(f32 dT);
        void LateUpdate();
        void Destroyed();

    private:
        Graphics::RenderContext mRenderContext;
    };
}  // namespace North::Engine
