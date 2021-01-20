#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "HelloTriangle.hpp"


int main()
{
	HelloTriangle application{};

	try
	{
		application.Run();
	}
	catch (const std::exception& exception)
	{
		std::cerr << exception.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
