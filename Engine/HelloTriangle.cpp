#include "HelloTriangle.hpp"

#include <iostream>
#include <cstdint>
#include <stdexcept>

const std::vector<const char*> validationLayers { "VK_LAYER_KHRONOS_validation" };

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif


const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;


VkResult CreateDebugUtilsMessengerEXT
(
	VkInstance instance,
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator,
	VkDebugUtilsMessengerEXT* pDebugMessenger
)
{
	auto function = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

	if (function != nullptr)
		return function(instance, pCreateInfo, pAllocator, pDebugMessenger);
	else
		return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void DestroyDebugUtilsMessengerEXT
(
	VkInstance instance,
	VkDebugUtilsMessengerEXT debugMessenger,
	const VkAllocationCallbacks* pAllocator
)
{
	auto function = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

	if (function != nullptr)
		function(instance, debugMessenger, pAllocator);
}

void HelloTriangle::WindowInitialization()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	mWindow = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

void HelloTriangle::VulkanInitialization()
{
	CreateInstance();
	SetupDebugMessenger();
}

void HelloTriangle::MainLoop()
{
	while (!glfwWindowShouldClose(mWindow))
		glfwPollEvents();
}

void HelloTriangle::Cleanup()
{
	if (enableValidationLayers)
		DestroyDebugUtilsMessengerEXT(mInstance, mDebugMessenger, nullptr);

	vkDestroyInstance(mInstance, nullptr);

	//Can be removed
	glfwDestroyWindow(mWindow);

	glfwTerminate();
}

void HelloTriangle::CreateInstance()
{
	if (enableValidationLayers && !CheckValidationLayerSupport())
		throw std::runtime_error("validation layers requested, but not available!");

	VkApplicationInfo applicationInformation{};

	applicationInformation.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInformation.pNext = nullptr;
	applicationInformation.pApplicationName = "Hello Triangle";
	applicationInformation.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationInformation.pEngineName = "No Engine";
	applicationInformation.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationInformation.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInformation{};
	createInformation.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInformation.pApplicationInfo = &applicationInformation;

	auto extensions = GetRequiredExtensions();
	createInformation.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInformation.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;

	if (enableValidationLayers)
	{
		createInformation.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInformation.ppEnabledLayerNames = validationLayers.data();

		PopulateDebugMessengerCreateInformation(debugCreateInfo);
		createInformation.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
	}
	else
	{
		createInformation.enabledLayerCount = 0;
		createInformation.pNext = nullptr;
	}

	if (vkCreateInstance(&createInformation, nullptr, &mInstance) != VK_SUCCESS)
		throw std::runtime_error("failed to create instance!");
}

bool HelloTriangle::CheckValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers)
	{
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
			return false;
	}

	return true;
}

std::vector<const char *> HelloTriangle::GetRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers)
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	return extensions;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback
(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData
)
{
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

void HelloTriangle::PopulateDebugMessengerCreateInformation(VkDebugUtilsMessengerCreateInfoEXT& createInformation)
{
	createInformation = {};
	createInformation.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInformation.messageSeverity =
	{
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
	};
	createInformation.messageType =
	{
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT 	|
  		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT 	|
  		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
	};
	createInformation.pfnUserCallback = DebugCallback;
}

void HelloTriangle::SetupDebugMessenger()
{
	if (!enableValidationLayers)
		return;

	VkDebugUtilsMessengerCreateInfoEXT createInformation;
	PopulateDebugMessengerCreateInformation(createInformation);

	if (CreateDebugUtilsMessengerEXT(mInstance, &createInformation, nullptr, &mDebugMessenger) != VK_SUCCESS)
		throw std::runtime_error("failed to set up debug messenger!");
}