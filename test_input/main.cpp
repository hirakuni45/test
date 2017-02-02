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
#if 0
	int a = 0;
	uint32_t b = 0;

	auto n = (input("%d[, ]%x", "-99 100") % a % b).num();
	std::cout << "Num: " << n << std::endl;
	std::cout << "1ST: " << a << ", 2ND: " << b << std::endl;
#endif
	float a;
	int b;
	uint32_t c;
	double d;

	auto n = (input("%f,%d,%x,%f", "101.945,-76,7ff,1.567") % a % b % c % d).num();
	std::cout << "Num: " << n << std::endl;
	std::cout << "1ST: " << a << std::endl;
	std::cout << "2ND: " << b << std::endl;
	std::cout << "3RD: " << c << std::endl;
	std::cout << "4TH: " << d << std::endl;
}
