#pragma once
#include <vk_mem_alloc.h>
#include <glm/mat4x4.hpp>
struct AllocatedBuffer
{
	VkBuffer buffer;
	VmaAllocation allocation;
};

struct AllocatedImage
{
	VkImage image;
	VmaAllocation allocation;
};