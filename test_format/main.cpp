#include <iostream>

#include "format.hpp"

int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	using namespace utils;

	{
		int a = 1000;
		format("%d\n") % a;
	}

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
		format("%d, '%s', %7.3f, %6.2f\n") % i % str % a % b;
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
}
