// Author: Jake Rieger
// Created: 11/19/25.
//

#include "Buffer.hpp"
#include <iostream>
#include <cstring>

namespace North::Graphics {
    Buffer::~Buffer() {
        Destroy();
    }

    Buffer::Buffer(Buffer&& other) noexcept
        : mAllocator(other.mAllocator), mBuffer(other.mBuffer), mAllocation(other.mAllocation), mSize(other.mSize),
          mType(other.mType), mMemoryUsage(other.mMemoryUsage), mMappedData(other.mMappedData) {
        // Reset the source object so it doesn't destroy our resources
        other.mBuffer     = VK_NULL_HANDLE;
        other.mAllocation = VK_NULL_HANDLE;
        other.mMappedData = nullptr;
    }

    Buffer& Buffer::operator=(Buffer&& other) noexcept {
        if (this != &other) {
            Destroy();

            mAllocator   = other.mAllocator;
            mBuffer      = other.mBuffer;
            mAllocation  = other.mAllocation;
            mSize        = other.mSize;
            mType        = other.mType;
            mMemoryUsage = other.mMemoryUsage;
            mMappedData  = other.mMappedData;

            other.mBuffer     = VK_NULL_HANDLE;
            other.mAllocation = VK_NULL_HANDLE;
            other.mMappedData = nullptr;
        }
        return *this;
    }

    void Buffer::Create(VmaAllocator allocator, VkDeviceSize size, Type type, MemoryUsage usage) {
        // Clean up any existing buffer
        Destroy();

        mAllocator   = allocator;
        mSize        = size;
        mType        = type;
        mMemoryUsage = usage;

        // Step 1: Define what the buffer will be used for
        // VkBufferCreateInfo describes the buffer we want to create
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType              = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size               = size;  // Size in bytes

        // Usage flags tell Vulkan how we'll use this buffer
        // Multiple flags can be combined with | operator
        bufferInfo.usage = GetVulkanUsageFlags(type);

        // For staging buffers, we need TRANSFER_SRC (source for copies)
        // For GPU buffers, we need TRANSFER_DST (destination for copies)
        if (type == Type::Staging) {
            bufferInfo.usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        } else if (usage == MemoryUsage::GPU_Only) {
            bufferInfo.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        }

        // EXCLUSIVE means only one queue family will use this buffer
        // If you need multiple queues (graphics + transfer), use CONCURRENT
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        // Step 2: Define how memory should be allocated
        // VmaAllocationCreateInfo tells VMA how to allocate memory for this buffer
        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.usage                   = GetVmaMemoryUsage(usage);

        // For CPU-accessible buffers, we want persistent mapping
        // This means we can map once and keep it mapped
        if (usage == MemoryUsage::CPU_To_GPU || usage == MemoryUsage::CPU_Only) {
            allocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
        }

        // Step 3: Create the buffer and allocate memory in one call
        // VMA handles all the complex memory allocation for us
        VmaAllocationInfo allocationInfo;
        VkResult result = vmaCreateBuffer(mAllocator,
                                          &bufferInfo,     // Buffer description
                                          &allocInfo,      // Memory allocation description
                                          &mBuffer,        // Output: VkBuffer handle
                                          &mAllocation,    // Output: VMA allocation handle
                                          &allocationInfo  // Output: Info about the allocation
        );

        if (result != VK_SUCCESS) {
            std::cerr << "Failed to create buffer! VkResult: " << result << std::endl;
            mBuffer     = VK_NULL_HANDLE;
            mAllocation = VK_NULL_HANDLE;
            return;
        }

        // If we requested persistent mapping, store the pointer
        if (allocationInfo.pMappedData != nullptr) { mMappedData = allocationInfo.pMappedData; }
    }

    void Buffer::Upload(const void* data, VkDeviceSize size, VkDeviceSize offset) {
        if (!IsValid()) {
            std::cerr << "Cannot upload to invalid buffer!" << std::endl;
            return;
        }

        if (mMemoryUsage == MemoryUsage::GPU_Only) {
            std::cerr << "Cannot directly upload to GPU_Only buffer! Use staging buffer and CopyFrom()." << std::endl;
            return;
        }

        if (offset + size > mSize) {
            std::cerr << "Upload size exceeds buffer bounds!" << std::endl;
            return;
        }

        // If buffer is already mapped (persistent mapping), use that pointer
        if (mMappedData != nullptr) {
            // Simple memcpy to mapped memory
            memcpy(CAST<u8*>(mMappedData) + offset, data, size);

            // Flush the memory range to make it visible to GPU
            // This is needed for non-coherent memory (most cases)
            vmaFlushAllocation(mAllocator, mAllocation, offset, size);
        } else {
            // Temporarily map, copy, unmap
            void* mappedData = Map();
            if (mappedData) {
                memcpy(CAST<u8*>(mappedData) + offset, data, size);
                Unmap();
            }
        }
    }

    void Buffer::CopyFrom(
      VkCommandBuffer cmd, const Buffer& srcBuffer, VkDeviceSize size, VkDeviceSize srcOffset, VkDeviceSize dstOffset) {
        if (!IsValid() || !srcBuffer.IsValid()) {
            std::cerr << "Cannot copy between invalid buffers!" << std::endl;
            return;
        }

        // VkBufferCopy describes the region to copy
        VkBufferCopy copyRegion = {};
        copyRegion.srcOffset    = srcOffset;  // Start position in source buffer
        copyRegion.dstOffset    = dstOffset;  // Start position in destination buffer
        copyRegion.size         = size;       // How many bytes to copy

        // Record the copy command into the command buffer
        // This doesn't execute immediately - it's recorded for later submission
        vkCmdCopyBuffer(cmd,
                        srcBuffer.GetHandle(),  // Source buffer
                        mBuffer,                // Destination buffer (this buffer)
                        1,                      // Number of regions to copy
                        &copyRegion             // Array of copy regions
        );
    }

    void* Buffer::Map() {
        if (!IsValid()) {
            std::cerr << "Cannot map invalid buffer!" << std::endl;
            return nullptr;
        }

        if (mMemoryUsage == MemoryUsage::GPU_Only) {
            std::cerr << "Cannot map GPU_Only buffer!" << std::endl;
            return nullptr;
        }

        // If already mapped (persistent mapping), return existing pointer
        if (mMappedData != nullptr) { return mMappedData; }

        // Map the memory temporarily
        void* data      = nullptr;
        VkResult result = vmaMapMemory(mAllocator, mAllocation, &data);

        if (result != VK_SUCCESS) {
            std::cerr << "Failed to map buffer memory! VkResult: " << result << std::endl;
            return nullptr;
        }

        return data;
    }

    void Buffer::Unmap() {
        if (!IsValid()) { return; }

        // Only unmap if it's not a persistent mapping
        if (mMappedData == nullptr) {
            // Flush memory before unmapping to ensure GPU sees our writes
            vmaFlushAllocation(mAllocator, mAllocation, 0, mSize);
            vmaUnmapMemory(mAllocator, mAllocation);
        }
    }

    void Buffer::Destroy() {
        if (mBuffer != VK_NULL_HANDLE && mAllocator != nullptr) {
            // If we have a non-persistent mapping active, unmap it first
            if (mMappedData == nullptr) {
                // Don't need to unmap - VMA handles it
            }

            // VMA destroys both the buffer and frees the memory in one call
            vmaDestroyBuffer(mAllocator, mBuffer, mAllocation);

            mBuffer     = VK_NULL_HANDLE;
            mAllocation = VK_NULL_HANDLE;
            mMappedData = nullptr;
        }
    }

    VkBufferUsageFlags Buffer::GetVulkanUsageFlags(Type type) {
        // These flags tell Vulkan how the buffer will be used
        switch (type) {
            case Type::Vertex:
                // Buffer will be bound as vertex buffer (source of vertex data)
                return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

            case Type::Index:
                // Buffer will be bound as index buffer (source of indices)
                return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

            case Type::Uniform:
                // Buffer will be bound as uniform buffer (shader constants)
                return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

            case Type::Storage:
                // Buffer will be bound as storage buffer (read/write in shaders)
                return VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;

            case Type::Staging:
                // Staging buffer is just a transfer source (we'll copy FROM it)
                return VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

            default:
                return 0;
        }
    }

    VmaMemoryUsage Buffer::GetVmaMemoryUsage(MemoryUsage usage) {
        // These tell VMA what kind of memory to allocate
        switch (usage) {
            case MemoryUsage::GPU_Only:
                // Memory only accessible by GPU (fastest, but no CPU access)
                // Used for static vertex/index buffers
                return VMA_MEMORY_USAGE_GPU_ONLY;

            case MemoryUsage::CPU_To_GPU:
                // Memory accessible by both CPU (write) and GPU (read)
                // Used for dynamic data that changes frequently (uniform buffers)
                return VMA_MEMORY_USAGE_CPU_TO_GPU;

            case MemoryUsage::GPU_To_CPU:
                // Memory accessible by GPU (write) and CPU (read)
                // Used for reading back results from GPU
                return VMA_MEMORY_USAGE_GPU_TO_CPU;

            case MemoryUsage::CPU_Only:
                // Memory only accessible by CPU
                // Rarely used in practice
                return VMA_MEMORY_USAGE_CPU_ONLY;

            default:
                return VMA_MEMORY_USAGE_GPU_ONLY;
        }
    }

}  // namespace North::Graphics