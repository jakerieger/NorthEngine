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
            mGame.Initialize(GetWindow(), width, height);
        }

        void OnDestroy() override {
            mGame.Shutdown();
        }

        void OnRender() override {
            mGame.RequestFrame();
        }

        void OnResize(u32 width, u32 height) override {
            mGame.Resize(width, height);
        }

    private:
        Game mGame;
    };
}  // namespace North
