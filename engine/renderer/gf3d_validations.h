#ifndef __GF3D_VALIDATIONS_H__
#define __GF3D_VALIDATIONS_H__

#include "containers/vector.hpp"
#include <vulkan/vulkan.h>

/**
* Checks if validation layer is on
* @return true if debug is on
*/
bool isValidationLayersEnabled();

/**
* Checks if the validation layer requested is supported
* @return true if all the layers are supported
*/
bool checkValidationLayerSupport();

/**
* Gets all the validation layers
* @return vector of all validation layers defined
*/
gf3d::vector<const char*> getValidationLayers();

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

VKAPI_ATTR VkBool32 VKAPI_CALL gf3dVkValidationDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

#endif // !__GF3D_VALIDATIONS_H__
