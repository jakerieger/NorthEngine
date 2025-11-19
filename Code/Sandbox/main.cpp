// Author: Jake Rieger
// Created: 11/18/25.
//

#include <Platform/GameApplication.hpp>
#include <Input/InputCodes.hpp>

namespace North {
    class SandboxApp final : public Platform::GameApplication {
    public:
        SandboxApp() : GameApplication("Sandbox") {}

        void OnKeyPress(u32 keyCode) override {
            if (keyCode == Input::Keys::Escape) { Quit(); }
        }
    };
}  // namespace North

int main() {
    North::SandboxApp app;
    return app.Run();
}