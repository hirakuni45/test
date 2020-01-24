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
	{  // 標準入力からの整数入力
		int a = 0;
		auto f = (input("%d\n") % a).status();
		std::cout << "Test00, stdin to integer: " << a;
		if(f) {
			std::cout << "  Pass." << std::endl;
		} else {
			std::cout << "  NG." << std::endl;
		}
	}
#endif

#if 1
	{  // test 1 整数の正常変換
		int a;
		int b = 12345;
		static const char* inp = { "12345" };
		if((input("%d", inp) % a).status()) {
			std::cout << "Test01, integer input as '" << inp << "' : " << a;
			if(a == b) {
				std::cout << "  Pass." << std::endl;
				++pass;
			} else {
				std::cout << "  Value NG!" << std::endl;
			}
		} else {
			std::cout << "Test01, State NG!" << std::endl;
		}
	}
#endif

#if 1
	{  // test 2 浮動小数点の正常変換
		float a;
		float b = 2.5e3;
		if((input("%f", "2.5e3") % a).status()) {
			std::cout << "Test02, floating point input as '2.5e3' : " << a;
			if(a == b) {
				std::cout << "  Pass." << std::endl;
				++pass;
			} else {
				std::cout << "  Value NG!" << std::endl;
			}
		} else {
			std::cout << "Test02, State NG!" << std::endl;
		}
	}
#endif

#if 1
	{  // test 3 整数の正常変換、ステータス確認
		int a;
		int b = 8756;
		static const char* inp = { "8756" };
		auto err = (input("%d", inp) % a).get_error();
		std::cout << "Test03, state true check: " << static_cast<int>(err) << " (" << a << ")";
		if(err == input::error::none) {
			if(a == b) {
				std::cout << "  Pass." << std::endl;
				++pass;
			} else {
				std::cout << "  Value NG!" << std::endl;
			}
		} else {
			std::cout << "  State NG!" << std::endl;
		}
	}
#endif

#if 1
	{  // test 4 整数の異常変換、ステータス確認、引数の初期値が変化しない事
		int a = 999;
		auto err = (input("%d", "8a6") % a).get_error();
		std::cout << "Test04, state false check: " << static_cast<int>(err) << " (" << a << ")";
		if(err != input::error::none && a == 999) {
			std::cout << "  Pass." << std::endl;
			++pass;
		} else {
			std::cout << "Test04, State NG!" << std::endl;
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
		std::cout << "Test05, multi scan for integer: " << a << ", " << b << ", " << c;
		if(n == 3 && a == -99 && b == 0x100 && c == 200) {
			std::cout << "  Pass." << std::endl;
			++pass;
		} else {
			std::cout << "Test05, Scan NG!" << std::endl;
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
		std::cout << "Test06, multi scan for integer: "
			<< a << ", " << b << ", " << c << " (" << n << ")";
		if(n == 3 && a == -95 && b == 0x1a4 && c == 236) {
			std::cout << "  Pass." << std::endl;
			++pass;
		} else {
			std::cout << "Test06, Scan NG!" << std::endl;
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
		std::cout << "Test07, multi scan for 'int,float,char': "
			<< a << ", " << b << ", " << c << " (" << n << ")";
		if(n == 3 && a == -17 && b == -12.75f && c == 'Z') {
			std::cout << "  Pass." << std::endl;
			++pass;
		} else {
			std::cout << "Test07, Scan NG!" << std::endl;
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
		std::cout << "Test08, multi scan for 'float,int,hex,float': "
			<< a << ", " << b << ", " << c << ", " << d << " (" << n << ")";
		if(n == 4 && a == 101.945f && b == -76 && c == 0x7ff && d == 0745) {
			std::cout << "  Pass." << std::endl;
			++pass;
		} else {
			std::cout << "Test08, Scan NG!" << std::endl;
		}
	}
#endif

#if 1
	{  // test 9 セパレーター文字の一致検査
		static const char* inp = { " 100 250 300" };
		int a = 9999;
		int b = 9999;
		int c = 9999;
		auto n = (input(" %d %d %d", inp) % a % b % c).num();
		std::cout << "Test09, multi scan for 'int' at spacing: "
			<< a << ", " << b << ", " << c << " (" << n << ")";
		if(n == 3 && a == 100 && b == 250 && c == 300) {
			std::cout << "  Pass." << std::endl;
			++pass;
		} else {
			std::cout << "Test09, Scan NG!" << std::endl;
		}
	}
#endif

#if 1
	{  // test10 (auto 機能確認)
///		static const char* inp = { "100 x9a b1101 o775" };
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
		}
	}
#endif

#if 1
	{  // test11 ２進、８進、１０進、１６進、浮動小数、文字、変換
		static const char* inp = { "11011 0625 -985 4a7Bc 1.025 Y" };
		uint32_t a = 9999;
		uint32_t b = 9999;
		int32_t c = 9999;
		uint32_t d = 9999;
		float e = 0.0f;
		char f;
		auto n = (input("%b %o %d %x %f %c", inp) % a % b % c % d % e % f).num();
		std::cout << "Test11, multi scan for 'bin, oct, dec, hex, float, cha': ";
		std::cout << a << ", " << b << ", " << c << ", " << d << ", ";
		std::cout << e << ", " << f << " (" << n << ")";
		if(n == 6 && a == 0b11011 && b == 0625 && c == -985 && d == 0x4a7bc && e == 1.025f && f == 'Y') {
			std::cout << "  Pass." << std::endl;
			++pass;
		} else {
			std::cout << "Test11, Scan NG!" << std::endl;
		}
	}
#endif

#if 1
	{  // test12 (整数の符号無しエラー確認)
		static const char* inp = { "-896" };
		int a = 0;
		auto err = (input("%u", inp) % a).get_error();
		std::cout << "Test12, Check signed integer error: " << a << ", " << static_cast<int>(err);
		if(err == input::error::different_sign) {
			std::cout << "  Pass." << std::endl;
			++pass;
		} else {
			std::cout << "Test12, Check error NG!" << std::endl;
		}
	}
#endif

#if 1
	{  // test13 (整数の符号無しエラー確認)
		static const char* inp = { "-896" };
		uint32_t a = 0;
		auto err = (input("%d", inp) % a).get_error();
		std::cout << "Test13, Check unsigned integer error: " << a << ", " << static_cast<int>(err);
		if(err == input::error::different_sign) {
			std::cout << "  Pass." << std::endl;
			++pass;
		} else {
			std::cout << "Test13, Check error NG!" << std::endl;
		}
	}
#endif

#if 1
	{  // test14 (符号無しにマイナス記号)
		static const char* inp = { "-896" };
		uint32_t a = 0;
		auto err = (input("%u", inp) % a).get_error();
		std::cout << "Test14, Check signed type error: " << a << ", " << static_cast<int>(err);
		if(err == input::error::sign_type) {
			std::cout << "  Pass." << std::endl;
			++pass;
		} else {
			std::cout << "Test14, Check error NG!" << std::endl;
		}
	}
#endif

#if 1
	{  // test15 (std::string 型の検査）
		int a;
		int b = 56789;
		static const char* t = { "56789" };
		std::string form = "%d";
		std::string inp = t;
		auto eno = (input(form, inp) % a).get_error();
		if(eno == input::error::none) {
			std::cout << "Test15, integer input as '" << inp << "' (std::string) : " << a;
			if(a == b) {
				std::cout << "  Pass." << std::endl;
				++pass;
			} else {
				std::cout << "  Value NG!" << std::endl;
			}
		} else {
			std::cout << "Test15, State NG! (" << static_cast<int>(eno) << ")" << std::endl;
		}
	}
#endif

#if 1
	{  // test16 (多桁の少数入力）
		float a;
		float b = 0.333333333333f;
		std::string form = "%f";
		std::string inp = "0.333333333333";
		if((input(form, inp) % a).status()) {
			std::cout << "Test16, Overflowed floting point input as '" << inp << "': " << a;
			if(a == b) {
				std::cout << "  Pass." << std::endl;
				++pass;
			} else {
				std::cout << "  Value NG!" << std::endl;
			}
		} else {
			std::cout << "Test16, State NG!" << std::endl;
		}
	}
#endif

#if 1
	{  // test17 (整数のオーバーフロー変換エラー確認）
		int a = 999999;
		static const char* inp = { "956478965123" };
		auto err = (input("%d", inp) % a).get_error();
		std::cout << "Test17, Overflowed integer(DEC) error check input as '" << inp << "': " << a;
		if(err == input::error::overflow) {
			std::cout << "  Pass." << std::endl;
			++pass;
		} else {
			std::cout << "  Error NG! (" << static_cast<int>(err) << ")" << std::endl;
		}
	}
#endif


	if(pass >= 17) {
		std::cout << std::endl;
		std::cout << "All Pass: " << pass << std::endl;
	} else {
		return -1;
	}
}
