#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>


class HelloTriangle
{
public:
	void Run()
	{
		WindowInitialization();
		VulkanInitialization();
		MainLoop();
		Cleanup();
	}

private:
	void Cleanup();
	void MainLoop();
	void CreateInstance();
	void SetupDebugMessenger();
	void VulkanInitialization();
	void WindowInitialization();
	bool CheckValidationLayerSupport();
	void PopulateDebugMessengerCreateInformation(VkDebugUtilsMessengerCreateInfoEXT&);
	std::vector<const char*> GetRequiredExtensions();

	GLFWwindow* 				mWindow;
	VkInstance 					mInstance;
	VkDebugUtilsMessengerEXT 	mDebugMessenger;
};