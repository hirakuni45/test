#include <iostream>

#include "input.hpp"

#include <cstdio>
#include <cstring>

int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	{
		float a;
		int n = std::sscanf("    100.54  ", "%f", &a);
		std::cout << "sscanf N: " << n << ", " << a << std::endl;
	}

	using namespace utils;
#if 1
	int a;
	auto err = (input("%d") % a).get_error();
	if(err == input::error::none) {
		std::cout << "Input: " << a << std::endl;
	} else {
		std::cout << "Input error: " << static_cast<int>(err) << std::endl;
	}
#endif

#if 0
	int a = 0;
	uint32_t b = 0;

	static const char* inp = { "-99 100" };
	auto n = (input("%d[, ]%x", inp) % a % b).num();
	std::cout << "Num: " << n << std::endl;
	std::cout << "1ST: " << a << ", 2ND: " << b << std::endl;
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
