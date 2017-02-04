#include <iostream>

#include "format.hpp"

int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	using namespace utils;

	int a = 1000;
	format("%d\n") % a;

}
