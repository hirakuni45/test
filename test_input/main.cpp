#include <iostream>

#include "input.hpp"

extern "C" {

	char sci_getch(void)
	{
		return 0;		
	}

};


int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{

	using namespace utils;


	int a;

	input("%d", "99") % a;


	std::cout << a << std::endl;

}
