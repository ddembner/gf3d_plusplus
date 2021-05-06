#include <vulkan/vulkan.h>
#include <vector>

class Descriptor
{

};

class DescriptorManager
{
private:
	std::vector<Descriptor> pools;
};