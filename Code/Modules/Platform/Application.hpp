// Author: Jake Rieger
// Created: 11/18/25.
//

#pragma once

#include "Common/Common.hpp"

#include <atomic>
#include <utility>

struct GLFWwindow;

namespace North::Platform {
    class IApplication {
    public:
        static constexpr u32 kDefaultWidth  = 1280;
        static constexpr u32 kDefaultHeight = 720;

        IApplication() = default;
        explicit IApplication(string title, u32 width = kDefaultWidth, u32 height = kDefaultHeight)
            : mTitle(std::move(title)) {}
        virtual ~IApplication() = default;

        NE_CLASS_PREVENT_MOVES_COPIES(IApplication)

        i32 Run();
        void Quit();

        virtual void OnAwake() {}
        virtual void OnUpdate(f32 dT) {}
        virtual void OnLateUpdate() {}
        virtual void OnRender() {}
        virtual void OnDestroy() {}

        virtual void OnKeyPress(u32 keyCode) {}
        virtual void OnKeyRelease(u32 keyCode) {}
        virtual void OnKey(u32 keyCode) {}

        virtual void OnMouseButtonPress(u32 button) {}
        virtual void OnMouseButtonRelease(u32 button) {}
        virtual void OnMouseButton(u32 button) {}

        virtual void OnMouseMove(f32 x, f32 y) {}
        virtual void OnMouseScroll(f32 dY) {}

        virtual void OnResize(u32 width, u32 height) {}

        NE_ND GLFWwindow* GetWindow() const;
        NE_ND void* GetWindowHandle() const;

        void GetWindowDimensions(u32& width, u32& height) const;
        NE_ND const string& GetTitle() const;

    private:
        GLFWwindow* mWindow {nullptr};
        u32 mWidth {kDefaultWidth};
        u32 mHeight {kDefaultHeight};
        string mTitle;
        f64 mLastFrameTime {0};
        std::atomic<bool> mRunning {false};

        void Initialize();
        void Shutdown() const;

        static void KeyCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods);
        static void MouseButtonCallback(GLFWwindow* window, i32 button, i32 action, i32 mods);
        static void CursorPosCallback(GLFWwindow* window, f64 xpos, f64 ypos);
        static void FramebufferSizeCallback(GLFWwindow* window, i32 width, i32 height);
    };
}  // namespace North::Platform
