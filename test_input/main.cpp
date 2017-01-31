#include <iostream>

#include "input.hpp"

extern "C" {

	char sci_getch(void)
	{
		return 0;		
	}

};


int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{

	using namespace utils;


	int a;
	uint32_t b;

	input("%d[, ]%x", "-99 100") % a % b;

	std::cout << a << ", " << b << std::endl;

	auto n = input::get_conversion();
	std::cout << "N: " << n << std::endl;
}
