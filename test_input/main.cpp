#include <iostream>

#include "input.hpp"

#include <cstdio>
#include <cstring>

int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	using namespace utils;

#if 0
	{
		float a;
		int n = std::sscanf("    100.54  ", "%f", &a);
		std::cout << "sscanf N: " << n << ", " << a << std::endl;
	}
#endif

#if 1
	{
		float a;
		if((input("%f", "2.5e3") % a).status()) {
			std::cout << a << std::endl;
		} else {
			std::cout << "Error exp(float)" << std::endl;
		}
	}
#endif

#if 0
	int a;
	auto err = (input("%d") % a).get_error();
	if(err == input::error::none) {
		std::cout << "Input: " << a << std::endl;
	} else {
		std::cout << "Input error: " << static_cast<int>(err) << std::endl;
	}
#endif

#if 1
	int a = 0;
	uint32_t b = 0;

	static const char* inp = { "-99 100" };
	auto n = (input("%d[, ]%x", inp) % a % b).num();
	std::cout << "Num: " << n << std::endl;
	std::cout << "1ST: " << a << ", 2ND: " << b << std::endl;
#endif

#if 1
	{
		int x = 0;
		int y = 0;
		int z = 0;
//		static const char* inp = { "235,-21,10\n" };
		static const char* inp = { "17,-10,2" };
		auto n = (input("%d,%d,%d", inp) % x % y % z).num();
		std::cout << std::endl;
		std::cout << "Src: " << inp << std::endl;
		std::cout << "Num: " << n << std::endl;
		std::cout << "X: " << x << ", Y: " << y << ", Z:" << z << std::endl;
	}
#endif


#if 0
	float a;
	int b;
	uint32_t c;
	double d;

	static const char* inp = { "101.945,-76,7ff,1.567" };
	auto n = (input("%f,%d,%x,%f", inp) % a % b % c % d).num();
	std::cout << "Num: " << n << std::endl;
	std::cout << "1ST: " << a << std::endl;
	std::cout << "2ND: " << b << std::endl;
	std::cout << "3RD: " << c << std::endl;
	std::cout << "4TH: " << d << std::endl;
#endif
}
