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
		format("%d, '%s', %f, %6.2f\n") % i % str % a % a;
	}

}
