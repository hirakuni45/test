#include <iostream>
#include <cstdio>
#include <cstring>

#include "input.hpp"

int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	using namespace utils;

	int error = 0;

#if 0
	{ // test
		float a;
		int n = std::sscanf("    100.54  ", "%f", &a);
		std::cout << "sscanf N: " << n << ", " << a << std::endl;
	}
#endif

#if 1
	{  // test 1 整数の正常変換
		int a;
		int b = 1234;
		if((input("%d", "1234") % a).status()) {
			std::cout << "Test1, int input as '1234' : " << a;
			if(a == b) {
				std::cout << "  Pass." << std::endl;
			} else {
				std::cout << "  Value NG!" << std::endl;
				return -1;
			}
		} else {
			std::cout << "  State NG!" << std::endl;
			return -1;
		}
	}
#endif

#if 1
	{  // test 2 浮動小数点の正常変換
		float a;
		float b = 2.5e3;
		if((input("%f", "2.5e3") % a).status()) {
			std::cout << "Test2, float input as '2.5e3' : " << a;
			if(a == b) {
				std::cout << "  Pass." << std::endl;
			} else {
				std::cout << "  Value NG!" << std::endl;
				return -1;
			}
		} else {
			std::cout << "  State NG!" << std::endl;
			return -1;
		}
	}
#endif

#if 1
	{  // test 3 整数の正常変換、ステータス確認
		int a;
		auto err = (input("%d", "876") % a).get_error();
		std::cout << "Test3, state ok check: " << static_cast<int>(err) << " (" << a << ")";
		if(err == input::error::none) {
			std::cout << "  Pass." << std::endl;
		} else {
			std::cout << "  State NG!" << std::endl;
			return -1;
		}
	}
#endif

#if 1
	{  // test 4 整数の異常変換、ステータス確認、引数の初期値が変化しない事
		int a = 999;
		auto err = (input("%d", "8a6") % a).get_error();
		std::cout << "Test4, state ng check: " << static_cast<int>(err) << " (" << a << ")";
		if(err != input::error::none && a == 999) {
			std::cout << "  Pass." << std::endl;
		} else {
			std::cout << "  State NG!" << std::endl;
			return -1;
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
		auto n = (input(" %3d %3d %3d", inp) % a % b % c).num();
		std::cout << std::endl;
		if(n == 3 && a == 100 && b == 250 && c == 300) {
			std::cout << "Test case 7: OK!\n";
		} else {
			std::cout << "Test case 7: NG!\n";
			++error;
		}
		std::cout << "Num: " << n << std::endl;
		std::cout << "1ST: " << a << std::endl;
		std::cout << "2ND: " << b << std::endl;
		std::cout << "3RD: " << c << std::endl;
	}
#endif

#if 1
	{  // test8 (auto 機能確認)
		static const char* inp = { "100 0x9a 0b1101 0o775" };
		int a[4] = { -1 };
		auto n = (input("%a %a %a %a", inp) % a[0] % a[1] % a[2] % a[3]).num();
		std::cout << std::endl;
		if(n == 4 && a[0] == 100 && a[1] == 0x9a && a[2] == 0b1101 && a[3] == 0775) {
			std::cout << "Test case 8: OK!\n";
		} else {
			std::cout << "Test case 8: NG!\n";
			++error;
		}
		std::cout << "Num: " << n << std::endl;
		std::cout << "1ST: " << a[0] << std::endl;
		std::cout << "2ND: " << a[1] << std::endl;
		std::cout << "3RD: " << a[2] << std::endl;
		std::cout << "4TH: " << a[3] << std::endl;
	}
#endif

	return error;
}
