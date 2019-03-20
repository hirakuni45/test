#include <iostream>

#include "input.hpp"

#include <cstdio>
#include <cstring>

int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	using namespace utils;

#if 0
	{ // test 1
		float a;
		int n = std::sscanf("    100.54  ", "%f", &a);
		std::cout << "sscanf N: " << n << ", " << a << std::endl;
	}
#endif

#if 1
	{  // test 2
		float a;
		if((input("%f", "2.5e3") % a).status()) {
			std::cout << a << std::endl;
		} else {
			std::cout << "Error exp(float)" << std::endl;
		}
	}
#endif

#if 0
	{  // test 3
		int a;
		auto err = (input("%d") % a).get_error();
		if(err == input::error::none) {
			std::cout << "Input: " << a << std::endl;
		} else {
			std::cout << "Input error: " << static_cast<int>(err) << std::endl;
		}
	}
#endif

#if 1
	{  // test 4
		int a = 0;
		uint32_t b = 0;
		int c = 0;

		static const char* inp = { "-99 100,200" };
		auto n = (input("%d[, ]%x,%d", inp) % a % b % c).num();
		std::cout << "Num: " << n << std::endl;
		std::cout << "1ST: " << a << ", 2ND: " << b << ", 3RD: " << c << std::endl;
	}
#endif

#if 1
	{  // test 5
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
	{  // test 6
		float a;
		int b;
		uint32_t c;
		double d;

		static const char* inp = { "101.945,-76,7ff,1.567" };
		auto n = (input("%f,%d,%x,%f", inp) % a % b % c % d).num();
		std::cout << std::endl;
		std::cout << "Num: " << n << std::endl;
		std::cout << "1ST: " << a << std::endl;
		std::cout << "2ND: " << b << std::endl;
		std::cout << "3RD: " << c << std::endl;
		std::cout << "4TH: " << d << std::endl;
	}
#endif

#if 1
	{  // test 7
		static const char* inp = { " 100 250 300" };
		int a;
		int b;
		int c;
		auto n = (input("%4d%4d%4d", inp) % a % b % c).num();
		std::cout << std::endl;
		if(n == 3 && a == 100 && b == 250 && c == 300) {
			std::cout << "Test case 7: OK!\n";
		}
		std::cout << "Num: " << n << std::endl;
		std::cout << "1ST: " << a << std::endl;
		std::cout << "2ND: " << b << std::endl;
		std::cout << "3RD: " << c << std::endl;
	}
#endif
}
