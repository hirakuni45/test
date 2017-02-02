#include <iostream>

#include "input.hpp"

int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	using namespace utils;
#if 0
	int a;
	if((input("%d") % a).status()) {
		std::cout << "Input: " << a << std::endl;
	} else {
		std::cout << "Input error" << std::endl;
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

#if 1
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
