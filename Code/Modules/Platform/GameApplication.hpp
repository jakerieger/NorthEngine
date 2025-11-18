// Author: Jake Rieger
// Created: 11/18/25.
//

#pragma once

#include "Application.hpp"
#include "Engine/Game.hpp"

namespace North {
    class GameApplication : public IApplication {
    public:
        explicit GameApplication(const string& title, u32 width = kDefaultWidth, u32 height = kDefaultHeight)
            : IApplication(title, width, height) {}

        ~GameApplication() override = default;

        void OnAwake() override {
            u32 width, height;
            GetWindowDimensions(width, height);
            mRenderContext.Initialize(GetWindow(), width, height);
        }

        void OnDestroy() override {
            mRenderContext.Shutdown();
        }

        void OnRender() override {
            mRenderContext.DrawFrame();
        }

        void OnResize(u32 width, u32 height) override {
            mRenderContext.Resize(width, height);
        }

    private:
        Game mGame;
        RenderContext mRenderContext;
    };
}  // namespace North
