// Author: Jake Rieger
// Created: 11/18/25.
//

#include <Platform/GameApplication.hpp>

namespace North {
    class SandboxApp : public GameApplication {
    public:
        SandboxApp() : GameApplication("Sandbox", 800, 600) {}
    };
}  // namespace North

int main() {
    North::SandboxApp app;
    return app.Run();
}