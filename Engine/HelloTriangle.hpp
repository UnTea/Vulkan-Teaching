#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


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
	void VulkanInitialization();
	void WindowInitialization();

	GLFWwindow* m_window;
	VkInstance 	m_instance;
};