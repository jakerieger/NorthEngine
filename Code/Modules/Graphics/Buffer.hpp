// Author: Jake Rieger
// Created: 11/19/25.
//

#pragma once

#include "Common/Common.hpp"

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace North::Graphics {
    /**
     * @brief Wrapper around VkBuffer that handles memory allocation via VMA
     *
     * This class simplifies Vulkan buffer creation and management.
     *
     * - Vertex data (positions, normals, UVs, etc.)
     * - Index data (triangle indices)
     * - Uniform data (constants like matrices, light data)
     * - Storage data (large read/write buffers for compute shaders)
     *
     * VMA (Vulkan Memory Allocator) handles the complex memory allocation for us,
     * so we don't have to manually manage VkDeviceMemory.
     */
    class Buffer {
    public:
        /**
         * @brief Types of buffers we can create
         *
         * Each type has different usage flags and memory requirements:
         * - Vertex: Stores vertex data, read by vertex shader
         * - Index: Stores indices, used for indexed drawing
         * - Uniform: Stores shader constants (updated frequently from CPU)
         * - Storage: Large buffers for compute shaders (read/write)
         * - Staging: Temporary CPU-visible buffer for uploading to GPU
         */
        enum class Type { Vertex, Index, Uniform, Storage, Staging };

        /**
         * @brief How the buffer memory should be allocated
         *
         * - GPU_Only: Fastest GPU access, no CPU access (for static data)
         * - CPU_To_GPU: CPU can write, GPU can read (for dynamic data)
         * - GPU_To_CPU: GPU can write, CPU can read (for readback)
         * - CPU_Only: CPU-side buffer (rarely used)
         */
        enum class MemoryUsage {
            GPU_Only,    // VMA_MEMORY_USAGE_GPU_ONLY
            CPU_To_GPU,  // VMA_MEMORY_USAGE_CPU_TO_GPU
            GPU_To_CPU,  // VMA_MEMORY_USAGE_GPU_TO_CPU
            CPU_Only     // VMA_MEMORY_USAGE_CPU_ONLY
        };

        Buffer() = default;
        ~Buffer();

        // Prevent copying (buffers own GPU memory)
        NE_CLASS_PREVENT_COPIES(Buffer)

        // Allow moving
        Buffer(Buffer&& other) noexcept;
        Buffer& operator=(Buffer&& other) noexcept;

        /**
         * @brief Create a buffer with the specified size and usage
         *
         * @param allocator VMA allocator (get from Renderer)
         * @param size Size in bytes of the buffer
         * @param type What kind of buffer (Vertex, Index, Uniform, etc.)
         * @param usage How memory should be allocated (GPU_Only, CPU_To_GPU, etc.)
         *
         * Example:
         *   Buffer vertexBuffer;
         *   vertexBuffer.Create(allocator, sizeof(Vertex) * 1000,
         *                      Buffer::Type::Vertex, Buffer::MemoryUsage::GPU_Only);
         */
        void Create(VmaAllocator allocator, VkDeviceSize size, Type type, MemoryUsage usage);

        /**
         * @brief Upload data to the buffer
         *
         * This only works for buffers with CPU-accessible memory (CPU_To_GPU or CPU_Only).
         * For GPU_Only buffers, you need to use a staging buffer (see CopyFrom).
         *
         * @param data Pointer to data to upload
         * @param size Size of data in bytes
         * @param offset Offset into buffer to start writing
         *
         * Example:
         *   vector<Vertex> vertices = {...};
         *   buffer.Upload(vertices.data(), sizeof(Vertex) * vertices.size());
         */
        void Upload(const void* data, VkDeviceSize size, VkDeviceSize offset = 0);

        /**
         * @brief Copy data from another buffer (GPU-side copy)
         *
         * Used to transfer data from a staging buffer to a GPU_Only buffer.
         * This happens on the GPU via a command buffer.
         *
         * @param cmd Command buffer to record the copy command
         * @param srcBuffer Source buffer to copy from
         * @param size Size in bytes to copy
         * @param srcOffset Offset in source buffer
         * @param dstOffset Offset in destination buffer
         *
         * Example (typical staging buffer workflow):
         *   Buffer staging;
         *   staging.Create(allocator, dataSize, Type::Staging, MemoryUsage::CPU_To_GPU);
         *   staging.Upload(data, dataSize);
         *
         *   Buffer gpuBuffer;
         *   gpuBuffer.Create(allocator, dataSize, Type::Vertex, MemoryUsage::GPU_Only);
         *
         *   // Record copy command
         *   VkCommandBuffer cmd = ...;
         *   gpuBuffer.CopyFrom(cmd, staging, dataSize);
         *   // Submit command buffer and wait...
         */
        void CopyFrom(VkCommandBuffer cmd,
                      const Buffer& srcBuffer,
                      VkDeviceSize size,
                      VkDeviceSize srcOffset = 0,
                      VkDeviceSize dstOffset = 0);

        /**
         * @brief Map the buffer memory for CPU access
         *
         * Returns a pointer you can write to directly. Only works for CPU-accessible buffers.
         * You MUST call Unmap() when done!
         *
         * Example:
         *   void* data = buffer.Map();
         *   memcpy(data, myData, dataSize);
         *   buffer.Unmap();
         *
         * For frequently updated buffers (like uniform buffers), you can keep them
         * persistently mapped (map once, use many times, unmap on destruction).
         */
        void* Map();

        /**
         * @brief Unmap the buffer memory
         *
         * Call this after you're done writing to mapped memory.
         */
        void Unmap();

        /**
         * @brief Destroy the buffer and free its memory
         *
         * This is automatically called in the destructor, but you can call it
         * explicitly if you need to release resources early.
         */
        void Destroy();

        // Getters
        NE_ND VkBuffer GetHandle() const {
            return mBuffer;
        }
        NE_ND VkDeviceSize GetSize() const {
            return mSize;
        }
        NE_ND Type GetType() const {
            return mType;
        }
        NE_ND bool IsValid() const {
            return mBuffer != VK_NULL_HANDLE;
        }
        NE_ND bool IsMapped() const {
            return mMappedData != nullptr;
        }

    private:
        VmaAllocator mAllocator   = nullptr;
        VkBuffer mBuffer          = VK_NULL_HANDLE;
        VmaAllocation mAllocation = VK_NULL_HANDLE;
        VkDeviceSize mSize        = 0;
        Type mType                = Type::Vertex;
        MemoryUsage mMemoryUsage  = MemoryUsage::GPU_Only;
        void* mMappedData         = nullptr;

        // Helper to convert our enum to VkBufferUsageFlags
        static VkBufferUsageFlags GetVulkanUsageFlags(Type type);

        // Helper to convert our enum to VmaMemoryUsage
        static VmaMemoryUsage GetVmaMemoryUsage(MemoryUsage usage);
    };

}  // namespace North::Graphics