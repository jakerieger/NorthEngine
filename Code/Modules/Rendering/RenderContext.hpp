// Author: Jake Rieger
// Created: 11/18/25.
//

#pragma once

#include "Common/Common.hpp"

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include <VkBootstrap.h>
#include <GLFW/glfw3.h>

namespace North {
    class RenderContext {
    public:
        RenderContext() = default;

        void Initialize(GLFWwindow* window, u32 width, u32 height);
        void Shutdown();

        void DrawFrame();
        void Resize(u32 width, u32 height);

        NE_ND bool IsInitialized() const {
            return mInitialized;
        }

    private:
        // Initialization helpers
        bool CreateInstance();
        bool SelectPhysicalDevice();
        bool CreateDevice();
        bool CreateAllocator();
        bool CreateSwapchain();
        bool CreateRenderPass();
        bool CreateFramebuffers();
        bool CreateCommandPool();
        bool CreateCommandBuffers();
        bool CreateSyncObjects();

        // Cleanup helpers
        void CleanupSwapchain();

        u32 mWidth        = 0;
        u32 mHeight       = 0;
        bool mInitialized = false;

        // Vulkan core objects
        vkb::Instance mVkbInstance;
        vkb::PhysicalDevice mVkbPhysicalDevice;
        vkb::Device mVkbDevice;
        vkb::Swapchain mVkbSwapchain;

        VkInstance mInstance             = VK_NULL_HANDLE;
        VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
        VkDevice mDevice                 = VK_NULL_HANDLE;
        VkSurfaceKHR mSurface            = VK_NULL_HANDLE;
        VkQueue mGraphicsQueue           = VK_NULL_HANDLE;
        VkQueue mPresentQueue            = VK_NULL_HANDLE;

        // Swapchain
        VkSwapchainKHR mSwapchain = VK_NULL_HANDLE;
        vector<VkImage> mSwapchainImages;
        vector<VkImageView> mSwapchainImageViews;
        VkFormat mSwapchainImageFormat {};
        VkExtent2D mSwapchainExtent {};

        // Render pass and framebuffers
        VkRenderPass mRenderPass = VK_NULL_HANDLE;
        vector<VkFramebuffer> mFramebuffers;

        // Command buffers
        VkCommandPool mCommandPool = VK_NULL_HANDLE;
        vector<VkCommandBuffer> mCommandBuffers;

        // Synchronization
        static constexpr i32 kMaxFramesInFlight = 2;
        vector<VkSemaphore> mImageAvailableSemaphores;
        vector<VkSemaphore> mRenderFinishedSemaphores;
        vector<VkFence> mInFlightFences;
        u32 mCurrentFrame = 0;

        // Memory allocator
        VmaAllocator mAllocator = VK_NULL_HANDLE;

        // Debug messenger
        VkDebugUtilsMessengerEXT mDebugMessenger = VK_NULL_HANDLE;
    };
}  // namespace North
