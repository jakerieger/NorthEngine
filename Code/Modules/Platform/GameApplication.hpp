// Author: Jake Rieger
// Created: 11/18/25.
//

#pragma once

#include "Application.hpp"
#include "Engine/Game.hpp"

namespace North::Platform {
    class GameApplication : public IApplication {
    public:
        explicit GameApplication(const string& title, u32 width = kDefaultWidth, u32 height = kDefaultHeight)
            : IApplication(title, width, height) {}

        ~GameApplication() override = default;

        void OnAwake() override;
        void OnDestroy() override;
        void OnUpdate(f32 dT) override;
        void OnRender() override;
        void OnLateUpdate() override;
        void OnResize(u32 width, u32 height) override;

    private:
        Engine::Game mGame;
    };
}  // namespace North::Platform
