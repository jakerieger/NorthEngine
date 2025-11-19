// Author: Jake Rieger
// Created: 11/19/25.
//

#pragma once

#include "Common/Common.hpp"
#include "Buffer.hpp"
#include "Math/Constants.hpp"

#include <vk_mem_alloc.h>

namespace North::Graphics {
    /// @brief Represents a single draw call with all necessary state
    struct DrawCommand {
        // const Pipeline* pipeline = nullptr;
        // const Material* material = nullptr;
        // const Mesh* mesh         = nullptr;
        Mat4x4 modelMatrix = Math::Constants::kIdentity4x4;
        //
        // // Additional per-draw data
        u32 instanceCount = 1;
        u32 firstInstance = 0;
    };

    /// @brief High-level render commands that can be submitted to the renderer
    enum class RenderCommandType {
        Draw,
        DrawIndexed,
        DrawInstanced,
        BeginRenderPass,
        EndRenderPass,
        SetPipeline,
        BindDescriptorSet,
        PushConstants
    };

    /// @brief Base class for render commands
    class RenderCommand {
    public:
        virtual ~RenderCommand()                        = default;
        NE_ND virtual RenderCommandType GetType() const = 0;
        virtual void Execute(VkCommandBuffer cmd)       = 0;
    };

    /// @brief Command buffer for collecting render commands
    class RenderCommandBuffer {
    public:
        RenderCommandBuffer() = default;

        void Reset();
        void Submit(unique_ptr<RenderCommand> command);

        NE_ND const vector<unique_ptr<RenderCommand>>& GetCommands() const {
            return mCommands;
        }

    private:
        vector<unique_ptr<RenderCommand>> mCommands;
    };

    /// @brief Represents a frame's worth of rendering work
    struct FrameData {
        VkCommandBuffer commandBuffer       = VK_NULL_HANDLE;
        VkSemaphore imageAvailableSemaphore = VK_NULL_HANDLE;
        VkSemaphore renderFinishedSemaphore = VK_NULL_HANDLE;
        VkFence inFlightFence               = VK_NULL_HANDLE;

        // Per-frame resources
        VkDescriptorSet globalDescriptorSet = VK_NULL_HANDLE;
        Buffer* uniformBuffer               = nullptr;

        // Frame-specific command collection
        RenderCommandBuffer renderCommandBuffer;
        vector<DrawCommand> drawCommands;
    };

    /// @brief Global frame constants (updated per frame)
    struct FrameConstants {
        Mat4x4 viewMatrix;
        Mat4x4 projectionMatrix;
        Mat4x4 viewProjectionMatrix;
        Vec4 cameraPosition;
        f32 time;
        f32 deltaTime;
        u32 frameIndex;
        u32 _padding;
    };
}  // namespace North::Graphics
