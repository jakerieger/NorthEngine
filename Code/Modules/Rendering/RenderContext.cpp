// Author: Jake Rieger
// Created: 11/18/25.
//

// VMA Implementation
// Must be defined in exactly one .cpp file before including vk_mem_alloc.h
#define VMA_IMPLEMENTATION

#include "RenderContext.hpp"
#include <iostream>
#include <cstring>

namespace North {
    void RenderContext::Initialize(GLFWwindow* window, u32 width, u32 height) {
        mWidth  = width;
        mHeight = height;

        if (!CreateInstance()) { throw std::runtime_error("Failed to create Vulkan instance"); }

        // Create surface
        if (glfwCreateWindowSurface(mInstance, window, nullptr, &mSurface) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create window surface");
        }

        if (!SelectPhysicalDevice()) { throw std::runtime_error("Failed to select physical device"); }
        if (!CreateDevice()) { throw std::runtime_error("Failed to create Vulkan device"); }
        if (!CreateAllocator()) { throw std::runtime_error("Failed to create Vulkan allocator"); }
        if (!CreateSwapchain()) { throw std::runtime_error("Failed to create Vulkan swapchain"); }
        if (!CreateRenderPass()) { throw std::runtime_error("Failed to create Vulkan render pass"); }
        if (!CreateFramebuffers()) { throw std::runtime_error("Failed to create Vulkan frame buffers"); }
        if (!CreateCommandPool()) { throw std::runtime_error("Failed to create Vulkan command pool"); }
        if (!CreateCommandBuffers()) { throw std::runtime_error("Failed to create Vulkan command buffers"); }
        if (!CreateSyncObjects()) { throw std::runtime_error("Failed to create Vulkan sync objects"); }

        mInitialized = true;
    }

    void RenderContext::Shutdown() {
        if (!mInitialized) return;

        vkDeviceWaitIdle(mDevice);

        // Cleanup sync objects
        for (size_t i = 0; i < kMaxFramesInFlight; i++) {
            vkDestroySemaphore(mDevice, mImageAvailableSemaphores[i], nullptr);
            vkDestroySemaphore(mDevice, mRenderFinishedSemaphores[i], nullptr);
            vkDestroyFence(mDevice, mInFlightFences[i], nullptr);
        }

        // Cleanup command pool
        vkDestroyCommandPool(mDevice, mCommandPool, nullptr);

        // Cleanup swapchain
        CleanupSwapchain();

        // Cleanup render pass
        vkDestroyRenderPass(mDevice, mRenderPass, nullptr);

        // Cleanup allocator
        if (mAllocator != VK_NULL_HANDLE) { vmaDestroyAllocator(mAllocator); }

        // Cleanup device and instance (vk-bootstrap handles this)
        vkb::destroy_device(mVkbDevice);
        vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
        vkb::destroy_instance(mVkbInstance);

        mInitialized = false;
    }

    void RenderContext::DrawFrame() {
        // Wait for the previous frame to finish
        vkWaitForFences(mDevice, 1, &mInFlightFences[mCurrentFrame], VK_TRUE, UINT64_MAX);

        // Acquire an image from the swapchain
        u32 imageIndex;
        VkResult result = vkAcquireNextImageKHR(mDevice,
                                                mSwapchain,
                                                UINT64_MAX,
                                                mImageAvailableSemaphores[mCurrentFrame],
                                                VK_NULL_HANDLE,
                                                &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            Resize(mWidth, mHeight);
            return;
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            std::cerr << "Failed to acquire swapchain image!" << std::endl;
            return;
        }

        // Reset fence only if we're submitting work
        vkResetFences(mDevice, 1, &mInFlightFences[mCurrentFrame]);

        // Record command buffer
        VkCommandBuffer cmd = mCommandBuffers[mCurrentFrame];
        vkResetCommandBuffer(cmd, 0);

        VkCommandBufferBeginInfo beginInfo {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(cmd, &beginInfo) != VK_SUCCESS) {
            std::cerr << "Failed to begin recording command buffer!" << std::endl;
            return;
        }

        // Begin render pass with clear color
        VkRenderPassBeginInfo renderPassInfo {};
        renderPassInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass        = mRenderPass;
        renderPassInfo.framebuffer       = mFramebuffers[imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = mSwapchainExtent;

        VkClearValue clearColor        = {{{0.1f, 0.2f, 0.3f, 1.0f}}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues    = &clearColor;

        vkCmdBeginRenderPass(cmd, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        // Nothing to draw, just clearing

        vkCmdEndRenderPass(cmd);

        if (vkEndCommandBuffer(cmd) != VK_SUCCESS) {
            std::cerr << "Failed to record command buffer!" << std::endl;
            return;
        }

        // Submit command buffer
        VkSubmitInfo submitInfo {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[]      = {mImageAvailableSemaphores[mCurrentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount     = 1;
        submitInfo.pWaitSemaphores        = waitSemaphores;
        submitInfo.pWaitDstStageMask      = waitStages;
        submitInfo.commandBufferCount     = 1;
        submitInfo.pCommandBuffers        = &cmd;

        VkSemaphore signalSemaphores[]  = {mRenderFinishedSemaphores[mCurrentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores    = signalSemaphores;

        if (vkQueueSubmit(mGraphicsQueue, 1, &submitInfo, mInFlightFences[mCurrentFrame]) != VK_SUCCESS) {
            std::cerr << "Failed to submit draw command buffer!" << std::endl;
            return;
        }

        // Present
        VkPresentInfoKHR presentInfo {};
        presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores    = signalSemaphores;

        VkSwapchainKHR swapChains[] = {mSwapchain};
        presentInfo.swapchainCount  = 1;
        presentInfo.pSwapchains     = swapChains;
        presentInfo.pImageIndices   = &imageIndex;

        result = vkQueuePresentKHR(mPresentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            Resize(mWidth, mHeight);
        } else if (result != VK_SUCCESS) {
            std::cerr << "Failed to present swapchain image!" << std::endl;
        }

        mCurrentFrame = (mCurrentFrame + 1) % kMaxFramesInFlight;
    }

    void RenderContext::Resize(u32 width, u32 height) {
        if (width == 0 || height == 0) return;

        vkDeviceWaitIdle(mDevice);

        mWidth  = width;
        mHeight = height;

        // CreateSwapchain will handle cleanup of old swapchain resources
        CreateSwapchain();
        CreateFramebuffers();
    }

    bool RenderContext::CreateInstance() {
        vkb::InstanceBuilder builder;

        auto instRet = builder.set_app_name("North Engine")
                         .request_validation_layers(true)
                         .use_default_debug_messenger()
                         .require_api_version(1, 1, 0)
                         .build();

        if (!instRet) {
            std::cerr << "Failed to create Vulkan instance: " << instRet.error().message() << std::endl;
            return false;
        }

        mVkbInstance    = instRet.value();
        mInstance       = mVkbInstance.instance;
        mDebugMessenger = mVkbInstance.debug_messenger;

        return true;
    }

    bool RenderContext::SelectPhysicalDevice() {
        vkb::PhysicalDeviceSelector selector {mVkbInstance};

        auto physRet = selector.set_surface(mSurface).set_minimum_version(1, 1).select();

        if (!physRet) {
            std::cerr << "Failed to select physical device: " << physRet.error().message() << std::endl;
            return false;
        }

        mVkbPhysicalDevice = physRet.value();
        mPhysicalDevice    = mVkbPhysicalDevice.physical_device;

        std::cout << "Selected GPU: " << mVkbPhysicalDevice.name << std::endl;
        return true;
    }

    bool RenderContext::CreateDevice() {
        vkb::DeviceBuilder deviceBuilder {mVkbPhysicalDevice};

        auto devRet = deviceBuilder.build();

        if (!devRet) {
            std::cerr << "Failed to create logical device: " << devRet.error().message() << std::endl;
            return false;
        }

        mVkbDevice = devRet.value();
        mDevice    = mVkbDevice.device;

        // Get queues
        auto graphicsQueueRet = mVkbDevice.get_queue(vkb::QueueType::graphics);
        if (!graphicsQueueRet) {
            std::cerr << "Failed to get graphics queue: " << graphicsQueueRet.error().message() << std::endl;
            return false;
        }
        mGraphicsQueue = graphicsQueueRet.value();

        auto presentQueueRet = mVkbDevice.get_queue(vkb::QueueType::present);
        if (!presentQueueRet) {
            std::cerr << "Failed to get present queue: " << presentQueueRet.error().message() << std::endl;
            return false;
        }
        mPresentQueue = presentQueueRet.value();

        return true;
    }

    bool RenderContext::CreateAllocator() {
        VmaAllocatorCreateInfo allocatorInfo {};
        allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_1;
        allocatorInfo.physicalDevice   = mPhysicalDevice;
        allocatorInfo.device           = mDevice;
        allocatorInfo.instance         = mInstance;

        if (vmaCreateAllocator(&allocatorInfo, &mAllocator) != VK_SUCCESS) {
            std::cerr << "Failed to create VMA allocator!" << std::endl;
            return false;
        }

        return true;
    }

    bool RenderContext::CreateSwapchain() {
        // Store the old swapchain handle for retirement
        VkSwapchainKHR oldSwapchain = mSwapchain;

        // Cleanup old swapchain resources if recreating (but keep the swapchain handle for now)
        if (oldSwapchain != VK_NULL_HANDLE) {
            // Destroy framebuffers and image views, but not the swapchain itself yet
            for (const auto framebuffer : mFramebuffers) {
                vkDestroyFramebuffer(mDevice, framebuffer, nullptr);
            }
            mFramebuffers.clear();

            for (const auto imageView : mSwapchainImageViews) {
                vkDestroyImageView(mDevice, imageView, nullptr);
            }
            mSwapchainImageViews.clear();
        }

        // Create new swapchain (passing old swapchain for efficient recreation)
        vkb::SwapchainBuilder swapchainBuilder {mVkbDevice};

        auto swapRet = swapchainBuilder.set_old_swapchain(oldSwapchain).set_desired_extent(mWidth, mHeight).build();

        if (!swapRet) {
            std::cerr << "Failed to create swapchain: " << swapRet.error().message() << std::endl;
            return false;
        }

        // Now destroy the old swapchain (the new one is created)
        if (oldSwapchain != VK_NULL_HANDLE) { vkDestroySwapchainKHR(mDevice, oldSwapchain, nullptr); }

        mVkbSwapchain         = swapRet.value();
        mSwapchain            = mVkbSwapchain.swapchain;
        mSwapchainImages      = mVkbSwapchain.get_images().value();
        mSwapchainImageViews  = mVkbSwapchain.get_image_views().value();
        mSwapchainImageFormat = mVkbSwapchain.image_format;
        mSwapchainExtent      = mVkbSwapchain.extent;

        return true;
    }

    bool RenderContext::CreateRenderPass() {
        VkAttachmentDescription colorAttachment {};
        colorAttachment.format         = mSwapchainImageFormat;
        colorAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef {};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass {};
        subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments    = &colorAttachmentRef;

        VkSubpassDependency dependency {};
        dependency.srcSubpass    = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass    = 0;
        dependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassInfo {};
        renderPassInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments    = &colorAttachment;
        renderPassInfo.subpassCount    = 1;
        renderPassInfo.pSubpasses      = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies   = &dependency;

        if (vkCreateRenderPass(mDevice, &renderPassInfo, nullptr, &mRenderPass) != VK_SUCCESS) {
            std::cerr << "Failed to create render pass!" << std::endl;
            return false;
        }

        return true;
    }

    bool RenderContext::CreateFramebuffers() {
        mFramebuffers.resize(mSwapchainImageViews.size());

        for (size_t i = 0; i < mSwapchainImageViews.size(); i++) {
            VkImageView attachments[] = {mSwapchainImageViews[i]};

            VkFramebufferCreateInfo framebufferInfo {};
            framebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass      = mRenderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments    = attachments;
            framebufferInfo.width           = mSwapchainExtent.width;
            framebufferInfo.height          = mSwapchainExtent.height;
            framebufferInfo.layers          = 1;

            if (vkCreateFramebuffer(mDevice, &framebufferInfo, nullptr, &mFramebuffers[i]) != VK_SUCCESS) {
                std::cerr << "Failed to create framebuffer!" << std::endl;
                return false;
            }
        }

        return true;
    }

    bool RenderContext::CreateCommandPool() {
        auto queueFamilyIndex = mVkbDevice.get_queue_index(vkb::QueueType::graphics);
        if (!queueFamilyIndex) {
            std::cerr << "Failed to get graphics queue family index!" << std::endl;
            return false;
        }

        VkCommandPoolCreateInfo poolInfo {};
        poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndex.value();

        if (vkCreateCommandPool(mDevice, &poolInfo, nullptr, &mCommandPool) != VK_SUCCESS) {
            std::cerr << "Failed to create command pool!" << std::endl;
            return false;
        }

        return true;
    }

    bool RenderContext::CreateCommandBuffers() {
        mCommandBuffers.resize(kMaxFramesInFlight);

        VkCommandBufferAllocateInfo allocInfo {};
        allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool        = mCommandPool;
        allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = static_cast<u32>(mCommandBuffers.size());

        if (vkAllocateCommandBuffers(mDevice, &allocInfo, mCommandBuffers.data()) != VK_SUCCESS) {
            std::cerr << "Failed to allocate command buffers!" << std::endl;
            return false;
        }

        return true;
    }

    bool RenderContext::CreateSyncObjects() {
        mImageAvailableSemaphores.resize(kMaxFramesInFlight);
        mRenderFinishedSemaphores.resize(kMaxFramesInFlight);
        mInFlightFences.resize(kMaxFramesInFlight);

        VkSemaphoreCreateInfo semaphoreInfo {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < kMaxFramesInFlight; i++) {
            if (vkCreateSemaphore(mDevice, &semaphoreInfo, nullptr, &mImageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(mDevice, &semaphoreInfo, nullptr, &mRenderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(mDevice, &fenceInfo, nullptr, &mInFlightFences[i]) != VK_SUCCESS) {
                std::cerr << "Failed to create synchronization objects!" << std::endl;
                return false;
            }
        }

        return true;
    }

    void RenderContext::CleanupSwapchain() {
        // Destroy framebuffers first (they reference image views)
        for (const auto framebuffer : mFramebuffers) {
            vkDestroyFramebuffer(mDevice, framebuffer, nullptr);
        }
        mFramebuffers.clear();

        // Destroy image views manually before destroying swapchain
        for (const auto imageView : mSwapchainImageViews) {
            vkDestroyImageView(mDevice, imageView, nullptr);
        }
        mSwapchainImageViews.clear();

        // Now destroy the swapchain (but not the image views, we already did that)
        // Note: vkb::destroy_swapchain also destroys image views, but we cleared them first
        if (mSwapchain != VK_NULL_HANDLE) {
            vkDestroySwapchainKHR(mDevice, mSwapchain, nullptr);
            mSwapchain = VK_NULL_HANDLE;
        }
    }
}  // namespace North