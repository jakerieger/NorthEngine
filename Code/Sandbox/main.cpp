// Author: Jake Rieger
// Created: 11/18/25.
//

#include <Platform/GameApplication.hpp>

namespace North {
    class SandboxApp final : public GameApplication {
    public:
        SandboxApp() : GameApplication("Sandbox") {}

        void OnKeyPress(u32 keyCode) override {
            if (keyCode == GLFW_KEY_ESCAPE) { Quit(); }
        }
    };
}  // namespace North

int main() {
    North::SandboxApp app;
    return app.Run();
}