#include <iostream>
#include <boost/format.hpp>

#include "format.hpp"

namespace {

	class sqri {
	public:
		int operator () (int in) {
			return in * in;
		}
	}; 

	void list_error_(utils::format::error errc) {
		switch(errc) {
		case utils::format::error::null:
			utils::format("Error: null ptr.\n");
			break;
		case utils::format::error::unknown:
			utils::format("Error: unknown type.\n");
			break;
		case utils::format::error::different:
			utils::format("Error: different type.\n");
			break;
		case utils::format::error::over:
			utils::format("Error: over range.\n");
			break;
		default:
			utils::format("Error: no error.\n");
			break;
		}
	}
}

// 速度検査
// #define SPEED_TEST

int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
#ifdef SPEED_TEST
	std::string s;
	if(argc >= 2) s = argv[1];
//	const long maxIter = 2000000L;
//	const long maxIter = 50000L;
	const long maxIter = 1;
	if(s == "printf") {
        // libc version
        for(long i = 0; i < maxIter; ++i)
            printf("%0.10f:%04d:%+g:%s:%p:%c:%%\n",
                1.234, 42, 3.13, "str", (void*)1000, (int)'X');
	}
	else if(s == "boost") {
		for(long i = 0; i < maxIter; ++i) {
			std::cout << boost::format("%0.10f:%04d:%+g:%s:%p:%c:%%\n")
			% 1.234 % 42 % 3.13 % "str" % (void*)1000 % (int)'X';
		}
	}
	else if(s == "format") {
        for(long i = 0; i < maxIter; ++i) {
            utils::format("%0.10f:%04d:%+g:%s:%x:%c:%%\n")
                % 1.234 % 42 % 3.13 % "str" % (uint32_t)1000 % (int)'X';
		}
	}
	return 0;
#endif

	{  // %nd
		int v = 1234;
		utils::format("--------(8)\n");
		utils::format("%8d\n") % v;
		utils::format("--------(2)\n");
		utils::format("%2d\n") % v;
	}

	{  // nullptr
		utils::format("test for 'nullptr form':\n  ");
		auto errcode = (utils::format(nullptr)).get_error();
		list_error_(errcode);
	}

	{  // %s nullptr
		utils::format("test for 'nullptr const char pointer':\n  ");
		const char* str = nullptr;
		auto errcode = (utils::format("STR: '%s'\n") % str).get_error();
		list_error_(errcode);
	}

	{
		utils::format("test for 'different type (int to float)':\n  ");
		float a = 1.0f;
		auto errcode = (utils::format("int: %d\n") % a).get_error();
		list_error_(errcode);
	}

	{
		utils::format("test for 'floating point auto(%%g) format':\n");
		float a = 1210.05f;
		printf("  printf: %%g(1210.05f): %g\n", a);
		fflush(stdout);
		utils::format("  format: %%g(1210.05f): %g\n") % a;
	}

	{
		float a = 1000000.0f;
		printf("  printf: %%e(1000000.0f): %e\n", a);
		fflush(stdout);
		utils::format("  format: %%e(1000000.0f): %e\n") % a;
	}

	{
		sqri sq;
		int a = sq(100);
		utils::format("%d\n") % a;
	}

	using namespace utils;
	{
		static const char* t = { "Asdfg" };
		format("'%s'\n") % t;
	}

	{
		char tmp[100];
		strcpy(tmp, "Qwert");
		format("'%s'\n") % tmp;
	}

	{
		char str[] = { "Asdfg" };
		int i = 123;
		float a = 1000.105f;
		float b = 0.0f;
		format("%d, %d, '%s', %7.3f, %6.2f\n") % i % -i % str % a % b;
	}

	{  // 固定小数点
		uint16_t val = (1 << 10) + 500; 
		format("Fixed point: %4.3:10y\n") % val;
	}

	{  // sformat
		format("sformat pass 1: in\n");
		int a = 9817;
		sformat("%d\n") % a;

		char tmp[4];
		sformat("%d", tmp, sizeof(tmp)) % a;
		format("'%s'\n") % tmp;
		format("sformat pass 1: out\n");
	}

	{
		float x = 1.0f;
		float y = 2.0f;
		float z = 3.0f;
		char tmp[512];
		sformat("Value: %f, %f, %f\n", tmp, sizeof(tmp)) % x % y % z;
		sformat("Value: %f, %f, %f\n", tmp, sizeof(tmp), true) % y % z % x;
		sformat("Value: %f, %f, %f\n", tmp, sizeof(tmp), true) % z % x % y;
		int size = sformat::chaout().size();
		format("(%d)\n%s") % size % tmp;
	}


	{  // 10進、16進、8進、2進
		int16_t n = -1;
		format("%d, 0x%04x, 0o%03o, 0b%04b\n") % n % n % n % n;
		n = 1;
		format("%d, 0x%04x, 0o%03o, 0b%04b\n") % n % n % n % n;
	}

	{  // 異なったプロトタイプ
		static const char* str = { "Poiuytrewq" };
		int val = 1921;
		format("%s, %d\n") % val % str;
	}
	format("\n");

	{  // 0.01 * 10000
		float a = 0.0f;
		for(int i = 0; i < 10000; ++i) {
			a += 0.01f;
		}
		format("0.01 * 10000 (float): %6.3f\n") % a;
	}
	{  // 0.01 * 10000
		double a = 0.0f;
		for(int i = 0; i < 10000; ++i) {
			a += 0.01;
		}
		std::cout << boost::format("0.01 * 10000 (double): %6.3f\n") % a;
	}
}
