#include <iostream>
#include <cstdio>
#include <cstring>

#include "input.hpp"

int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	using namespace utils;

	int pass = 0;

#if 0
	{ // test 0
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
			std::cout << "Test1, integer input as '1234' : " << a;
			if(a == b) {
				std::cout << "  Pass." << std::endl;
				++pass;
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
			std::cout << "Test2, floating point input as '2.5e3' : " << a;
			if(a == b) {
				std::cout << "  Pass." << std::endl;
				++pass;
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
		std::cout << "Test3, state true check: " << static_cast<int>(err) << " (" << a << ")";
		if(err == input::error::none) {
			std::cout << "  Pass." << std::endl;
			++pass;
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
		std::cout << "Test4, state false check: " << static_cast<int>(err) << " (" << a << ")";
		if(err != input::error::none && a == 999) {
			std::cout << "  Pass." << std::endl;
			++pass;
		} else {
			std::cout << "  State NG!" << std::endl;
			return -1;
		}
	}
#endif

#if 1
	{  // test 5 複数の整数変換（１）
		int a = 0;
		uint32_t b = 0;
		int c = 0;
		static const char* inp = { "-99 100,200" };
		auto n = (input("%d[, ]%x,%d", inp) % a % b % c).num();
		std::cout << "Test5, multi scan for integer: " << a << ", " << b << ", " << c;
		if(n == 3 && a == -99 && b == 0x100 && c == 200) {
			std::cout << "  Pass." << std::endl;
			++pass;
		} else {
			std::cout << "  Scan NG!" << std::endl;
			return -1;
		}
	}
#endif

#if 1
	{  // test 6 複数の整数変換（２）
		int a = 0;
		uint32_t b = 0;
		int c = 0;
		static const char* inp = { "-95,1a4,236" };
		auto n = (input("%d[, ]%x,%d", inp) % a % b % c).num();
		std::cout << "Test6, multi scan for integer: "
			<< a << ", " << b << ", " << c << " (" << n << ")";
		if(n == 3 && a == -95 && b == 0x1a4 && c == 236) {
			std::cout << "  Pass." << std::endl;
			++pass;
		} else {
			std::cout << "  Scan NG!" << std::endl;
			return -1;
		}
	}
#endif

#if 1
	{  // test 7 整数、浮動小数点、文字 の複数変換
		int a = 0;
		float b = 0;
		char c = 0;
		static const char* inp = { "-17,-12.75,Z" };
		auto n = (input("%d,%f,%c", inp) % a % b % c).num();
		std::cout << "Test7, multi scan for 'int,float,char': "
			<< a << ", " << b << ", " << c << " (" << n << ")";
		if(n == 3 && a == -17 && b == -12.75f && c == 'Z') {
			std::cout << "  Pass." << std::endl;
			++pass;
		} else {
			std::cout << "  Scan NG!" << std::endl;
			return -1;
		}
	}
#endif

#if 1
	{  // test 8 浮動小数点、整数、１６進、８進の複数変換
		float a;
		int b;
		uint32_t c;
		uint32_t d;
		static const char* inp = { "101.945,-76,7ff,745" };
		auto n = (input("%f,%d,%x,%o", inp) % a % b % c % d).num();
		std::cout << "Test8, multi scan for 'float,int,hex,float': "
			<< a << ", " << b << ", " << c << ", " << d << " (" << n << ")";
		if(n == 4 && a == 101.945f && b == -76 && c == 0x7ff && d == 0745) {
			std::cout << "  Pass." << std::endl;
			++pass;
		} else {
			std::cout << "  Scan NG!" << std::endl;
			return -1;
		}
	}
#endif

#if 1
	{  // test 9 セパレーター文字の一致検査
		static const char* inp = { " 100 250 300" };
		int a;
		int b;
		int c;
		auto n = (input(" %3d %3d %3d", inp) % a % b % c).num();
		std::cout << "Test9, multi scan for 'int' at spacing: "
			<< a << ", " << b << ", " << c << " (" << n << ")";
		if(n == 3 && a == 100 && b == 250 && c == 300) {
			std::cout << "  Pass." << std::endl;
			++pass;
		} else {
			std::cout << "  Scan NG!" << std::endl;
			return -1;
		}
	}
#endif

#if 1
	{  // test10 (auto 機能確認)
		static const char* inp = { "100 0x9a 0b1101 0o775" };
		int a[4] = { -1 };
		auto n = (input("%a %a %a %a", inp) % a[0] % a[1] % a[2] % a[3]).num();
		std::cout << "Test10, multi scan for 'auto': "
			<< a[0] << ", " << a[1] << ", " << a[2] << ", " << a[3] << " (" << n << ")";
		if(n == 4 && a[0] == 100 && a[1] == 0x9a && a[2] == 0b1101 && a[3] == 0775) {
			std::cout << "  Pass." << std::endl;
			++pass;
		} else {
			std::cout << "  Scan NG!" << std::endl;
			return -1;
		}
	}
#endif


	if(pass == 10) {
		std::cout << std::endl;
		std::cout << "All Pass: " << pass << std::endl;
	}
}
