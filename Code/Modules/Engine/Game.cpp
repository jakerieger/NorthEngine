// Author: Jake Rieger
// Created: 11/18/25.
//

#include "Game.hpp"

namespace North::Engine {
    void Game::Initialize(GLFWwindow* window, u32 width, u32 height) {
        mRenderContext.Initialize(window, width, height);
        mActiveScene = make_unique<Scene>();
    }

    void Game::Shutdown() {
        mActiveScene.reset();
        mRenderContext.Shutdown();
    }

    void Game::RequestFrame() {
        // auto frameData = mRenderContext.BeginFrame();
        mActiveScene->Draw(mRenderContext);
        // mRenderContext.EndFrame(frameData);

        mRenderContext.DrawFrame();
    }

    void Game::Resize(u32 width, u32 height) {
        mRenderContext.Resize(width, height);
    }

    bool Game::Initialized() const {
        return mRenderContext.Initialized();
    }

    void Game::Awake() {
        mActiveScene->Awake();
    }

    void Game::Update(f32 dT) {
        mActiveScene->Update(dT);
    }

    void Game::LateUpdate() {
        mActiveScene->LateUpdate();
    }

    void Game::Destroyed() {
        mActiveScene->Destroyed();
    }
}  // namespace North::Engine