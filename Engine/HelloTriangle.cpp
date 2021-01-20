#include "HelloTriangle.hpp"

#include <cstdint>
#include <stdexcept>


const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;


void HelloTriangle::WindowInitialization ()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

void HelloTriangle::VulkanInitialization ()
{
	CreateInstance();
}

void HelloTriangle::MainLoop ()
{
	while (!glfwWindowShouldClose(m_window))
		glfwPollEvents();
}

void HelloTriangle::Cleanup ()
{
	vkDestroyInstance(m_instance, nullptr);

	//Can be removed
	glfwDestroyWindow(m_window);

	glfwTerminate();
}

void HelloTriangle::CreateInstance ()
{
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

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInformation.enabledExtensionCount = glfwExtensionCount;
	createInformation.ppEnabledExtensionNames = glfwExtensions;

	createInformation.enabledLayerCount = 0;

	if (vkCreateInstance(&createInformation, nullptr, &m_instance) != VK_SUCCESS)
		throw std::runtime_error("failed to create instance!");
}