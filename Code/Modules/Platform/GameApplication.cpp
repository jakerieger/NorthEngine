// Author: Jake Rieger
// Created: 11/18/25.
//

#include "GameApplication.hpp"

namespace North::Platform {
    void GameApplication::OnAwake() {
        u32 width, height;
        GetWindowDimensions(width, height);
        mGame.Initialize(GetWindow(), width, height);
        if (!mGame.Initialized()) { throw std::runtime_error("Failed to initialize game"); }
        mGame.Awake();
    }

    void GameApplication::OnDestroy() {
        mGame.Destroyed();
        mGame.Shutdown();
    }

    void GameApplication::OnUpdate(f32 dT) {
        mGame.Update(dT);
    }

    void GameApplication::OnRender() {
        mGame.RequestFrame();
    }

    void GameApplication::OnLateUpdate() {
        mGame.LateUpdate();
    }

    void GameApplication::OnResize(u32 width, u32 height) {
        mGame.Resize(width, height);
    }
}  // namespace North::Platform