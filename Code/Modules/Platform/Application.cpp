// Author: Jake Rieger
// Created: 11/18/25.
//

#include "Application.hpp"

#include <GLFW/glfw3.h>

namespace North::Platform {
    i32 IApplication::Run() {
        Initialize();
        OnAwake();
        {
            mRunning       = true;
            mLastFrameTime = glfwGetTime();

            // Main loop
            while (mRunning && !glfwWindowShouldClose(mWindow)) {
                const f64 currentTime = glfwGetTime();
                const auto dT         = CAST<f32>(currentTime - mLastFrameTime);
                mLastFrameTime        = currentTime;

                OnUpdate(dT);
                OnRender();
                OnLateUpdate();

                glfwPollEvents();
            }

            mRunning = false;
        }
        OnDestroy();
        Shutdown();

        return EXIT_SUCCESS;
    }

    void IApplication::Quit() {
        mRunning = false;
    }

    GLFWwindow* IApplication::GetWindow() const {
        return mWindow;
    }

    void* IApplication::GetWindowHandle() const {
        return (void*)mWindow;  // TODO: Actually implement the platform-specific handles
    }

    void IApplication::GetWindowDimensions(u32& width, u32& height) const {
        width  = mWidth;
        height = mHeight;
    }

    const string& IApplication::GetTitle() const {
        return mTitle;
    }

    void IApplication::Initialize() {
        if (!glfwInit()) { throw std::runtime_error("Failed to initialize GLFW"); }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        mWindow = glfwCreateWindow((i32)mWidth, (i32)mHeight, mTitle.c_str(), nullptr, nullptr);
        if (!mWindow) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwSetWindowUserPointer(mWindow, this);

        glfwSetKeyCallback(mWindow, KeyCallback);
        glfwSetMouseButtonCallback(mWindow, MouseButtonCallback);
        glfwSetCursorPosCallback(mWindow, CursorPosCallback);
        glfwSetFramebufferSizeCallback(mWindow, FramebufferSizeCallback);
    }

    void IApplication::Shutdown() const {
        if (mWindow) {
            glfwDestroyWindow(mWindow);
            glfwTerminate();
        }
    }

    void IApplication::KeyCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods) {
        auto* app = CAST<IApplication*>(glfwGetWindowUserPointer(window));
        if (app) {
            app->OnKey(key);
            if (action == GLFW_PRESS) {
                app->OnKeyPress(key);
            } else if (action == GLFW_RELEASE) {
                app->OnKeyRelease(key);
            }
        }
    }

    void IApplication::MouseButtonCallback(GLFWwindow* window, i32 button, i32 action, i32 mods) {
        auto* app = CAST<IApplication*>(glfwGetWindowUserPointer(window));
        if (app) {
            app->OnMouseButton(button);
            if (action == GLFW_PRESS) {
                app->OnMouseButtonPress(button);
            } else if (action == GLFW_RELEASE) {
                app->OnMouseButtonRelease(button);
            }
        }
    }

    void IApplication::CursorPosCallback(GLFWwindow* window, f64 xpos, f64 ypos) {
        auto* app = CAST<IApplication*>(glfwGetWindowUserPointer(window));
        if (app) { app->OnMouseMove((f32)xpos, (f32)ypos); }
    }

    void IApplication::FramebufferSizeCallback(GLFWwindow* window, i32 width, i32 height) {
        auto* app = CAST<IApplication*>(glfwGetWindowUserPointer(window));
        if (app) {
            app->mWidth  = width;
            app->mHeight = height;
            app->OnResize(width, height);
        }
    }
}  // namespace North::Platform