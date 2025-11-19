// Author: Jake Rieger
// Created: 11/18/25.
//

#pragma once

#include "Common/Common.hpp"
#include "Rendering/RenderContext.hpp"

namespace North {
    class Game {
    public:
        Game() = default;

        void Initialize(GLFWwindow* window, u32 width, u32 height) {
            mRenderContext.Initialize(window, width, height);
        }

        void Shutdown() {
            mRenderContext.Shutdown();
        }

        void RequestFrame() {
            mRenderContext.DrawFrame();
        }

        void Resize(u32 width, u32 height) {
            mRenderContext.Resize(width, height);
        }

    private:
        RenderContext mRenderContext;
    };
}  // namespace North
