#include <vulkan/vulkan.h>
#include <vector>

class Descriptor
{

};

class DescriptorSetAllocator
{
private:
	std::vector<Descriptor> pools;
};