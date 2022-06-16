#include "gf3d_memory.h"
#include "gf3d_logger.h"
#include "memory/FreeListAllocator.h"

namespace gf3d
{
	constexpr u64 MAX_MEMORY_TYPES = static_cast<u64>(memory_type::eMaxTypes);

	struct MemoryManager
	{
		FreeListAllocator dynamicAllocator;
		u64 memoryTracker[MAX_MEMORY_TYPES];
	};

	static MemoryManager* gMemoryManager;

	const char* memoryTypeAsString(const memory_type& type)
	{
		switch (type)
		{
		case memory_type::eUnknown:
			return "Unknown";
		case memory_type::eAllocator:
			return "Allocator";
		case memory_type::eDynamic:
			return "Dynamic";
		case memory_type::eNew:
			return "New";
		case memory_type::eVector:
			return "Vector";
		case memory_type::eTexture:
			return "Texture";
		default:
			return "Not a memory type";
		}
	}

	void initDynamicMemorySystem(u64 size)
	{
		if (!gMemoryManager) {
			gMemoryManager = static_cast<MemoryManager*>(std::malloc(sizeof(MemoryManager)));
			if (gMemoryManager) {
				std::memset(gMemoryManager, 0, sizeof(MemoryManager));
				gMemoryManager->dynamicAllocator.init(size);
			}
		}
	}

	void shutdownDynamicMemorySystem()
	{
		if (gMemoryManager) {
			gMemoryManager->dynamicAllocator.destroy();
			std::free(gMemoryManager);
			gMemoryManager = nullptr;
		}
	}

	void* malloc(const u64& size, const memory_type& type)
	{
		void* block = nullptr;

		if (gMemoryManager) {
			block = gMemoryManager->dynamicAllocator.allocate(size);
			const u64 i = static_cast<u64>(type);
			gMemoryManager->memoryTracker[i] += size;
		}
		else {
			block = std::malloc(size);
		}
		
		return block;
	}

	void free(void* data, const u64& size, const memory_type& type)
	{
		if (gMemoryManager) {
			gMemoryManager->dynamicAllocator.free(data);
			const u64 i = static_cast<u64>(type);
			gMemoryManager->memoryTracker[i] -= size;
			return;
		}
		std::free(data);
	}

	void log_memory_usage()
	{
		char buffer[2048] = "\n---Memory Usage---\n";
		u64 offset = std::strlen(buffer);

		for (u64 i = 0; i < MAX_MEMORY_TYPES; i++) {

			const char* typeAsString = memoryTypeAsString(static_cast<memory_type>(i));
			f64 amount = 0.0;
			char unit[3] = "xb";

			if (gMemoryManager->memoryTracker[i] >= TO_GB(1)) {
				amount = gMemoryManager->memoryTracker[i] / static_cast<f64>(TO_GB(1));
				unit[0] = 'g';
			}
			else if (gMemoryManager->memoryTracker[i] >= TO_MB(1)) {
				amount = gMemoryManager->memoryTracker[i] / static_cast<f64>(TO_MB(1));
				unit[0] = 'm';
			}
			else if (gMemoryManager->memoryTracker[i] >= TO_KB(1)) {
				amount = gMemoryManager->memoryTracker[i] / static_cast<f64>(TO_KB(1));
				unit[0] = 'k';
			}
			else {
				amount = gMemoryManager->memoryTracker[i];
				unit[0] = 'b';
				unit[1] = 0;
			}

			int length = snprintf(buffer + offset, 2048, "%s: %.2f%s\n", typeAsString, amount, unit);
			offset += length;
		}

		LOGGER_INFO(buffer);
	}
}

void* operator new(std::size_t size)
{
	void* data = nullptr;

	if (size == 0)
		size = 1;

	data = gf3d::malloc(size, gf3d::memory_type::eNew);

	return data;
}

void* operator new(std::size_t size, std::align_val_t al)
{
	void* data = nullptr;

	if (size == 0)
		size = 1;

	data = gf3d::malloc(size, gf3d::memory_type::eNew);

	return data;
}

void operator delete(void* data, u64 size) noexcept
{
	gf3d::free(data, size, gf3d::memory_type::eNew);
}

void operator delete(void* data) noexcept
{
	u64* size = static_cast<u64*>(data) - 2;
	gf3d::free(data, *size, gf3d::memory_type::eNew);
}

void* operator new[](std::size_t size)
{
	void* data = gf3d::malloc(size, gf3d::memory_type::eNew);

	return data;
}

void operator delete[](void* data) noexcept
{
	u64* size = static_cast<u64*>(data) - 2;
	gf3d::free(data, *size, gf3d::memory_type::eNew);
}