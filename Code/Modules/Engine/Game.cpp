// Author: Jake Rieger
// Created: 11/18/25.
//

#include "Game.hpp"

namespace North::Engine {
    void Game::Initialize(GLFWwindow* window, u32 width, u32 height) {
        mRenderContext.Initialize(window, width, height);
    }

    void Game::Shutdown() {
        mRenderContext.Shutdown();
    }

    void Game::RequestFrame() {
        mRenderContext.DrawFrame();
    }

    void Game::Resize(u32 width, u32 height) {
        mRenderContext.Resize(width, height);
    }

    bool Game::Initialized() const {
        return mRenderContext.Initialized();
    }

    void Game::Awake() {
        // TODO: Call Awake() on script(s) of scene entities
    }

    void Game::Update(f32 dT) {
        // TODO: Call Update() on script(s) of scene entities
    }

    void Game::LateUpdate() {
        // TODO: Call LateUpdate() on script(s) of scene entities
    }

    void Game::Destroyed() {
        // TODO: Call Destroyed() on script(s) of scene entities
    }
}  // namespace North