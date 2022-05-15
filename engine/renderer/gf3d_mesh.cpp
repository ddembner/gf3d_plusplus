#define VMA_IMPLEMENTATION
#include "gf3d_mesh.h"
#include "vulkan_functions.h"

gf3d::vector<VkVertexInputAttributeDescription> Mesh::Vertex::getAttributeDescription()
{
	gf3d::vector<VkVertexInputAttributeDescription> attributes(2);

	//Position
	attributes[0].binding = 0;
	attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributes[0].location = 0;
	attributes[0].offset = offsetof(Vertex, position);

	//Color
	attributes[1].binding = 0;
	attributes[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributes[1].location = 1;
	attributes[1].offset = offsetof(Vertex, color);

	return attributes;
}

gf3d::vector<VkVertexInputBindingDescription> Mesh::Vertex::getBindingDescription()
{
	gf3d::vector<VkVertexInputBindingDescription> bindings(1);
	bindings[0].binding = 0;
	bindings[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	bindings[0].stride = sizeof(Vertex);
	
	return bindings;
}

Mesh::Mesh(const std::string& path)
{

}

void Mesh::destroy(const VmaAllocator& allocator)
{
	if (!isAllocated) return;
	vmaDestroyBuffer(allocator, allocatedBuffer.buffer, allocatedBuffer.allocation);
	isAllocated = false;
}

void Mesh::allocateMesh(Gf3dDevice& gf3dDevice)
{
	auto device = gf3dDevice.GetDevice();
	auto allocator = gf3dDevice.GetAllocator();

	size_t size = vertices.size() * sizeof(Vertex);

	//Create a buffer on the cpu
	VulkanBuffer stagingBuffer = createAllocatedBuffer(
		allocator, 
		size, 
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
		VMA_MEMORY_USAGE_CPU_ONLY);

	//Put the vertex data into the buffer
	void* data;
	vmaMapMemory(allocator, stagingBuffer.allocation, &data);
	memcpy(data, vertices.data(), vertices.size() * sizeof(Vertex));
	vmaUnmapMemory(allocator, stagingBuffer.allocation);

	//Create the buffer for the mesh on the gpu
	allocatedBuffer = createAllocatedBuffer(
		allocator, 
		vertices.size() * sizeof(Vertex), 
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, 
		VMA_MEMORY_USAGE_GPU_ONLY);

	//Copy from the cpu into the gpu
	VkCommandBuffer cmd = nullptr;

	VkBufferCopy copyBuffer = {};
	copyBuffer.dstOffset = 0;
	copyBuffer.srcOffset = 0;
	copyBuffer.size = size;

	vulkanCommandBufferSingleUseBegin(&gf3dDevice, gf3dDevice.GetCommandPool(), &cmd);
	vkCmdCopyBuffer(cmd, stagingBuffer.buffer, allocatedBuffer.buffer, 1, &copyBuffer);
	vulkanCommandBufferSingleUseEnd(&gf3dDevice, gf3dDevice.GetCommandPool(), &cmd, gf3dDevice.GetGraphicsQueue());

	vmaDestroyBuffer(allocator, stagingBuffer.buffer, stagingBuffer.allocation);

	vertexCount = static_cast<u32>(vertices.size());
	isAllocated = true;
}

void Mesh::bind(VkCommandBuffer cmd)
{
	VkDeviceSize offset = 0;
	vkCmdBindVertexBuffers(cmd, 0, 1, &allocatedBuffer.buffer, &offset);
}

void Mesh::draw(VkCommandBuffer cmd)
{
	vkCmdDraw(cmd, vertexCount, 1, 0, 0);
}
