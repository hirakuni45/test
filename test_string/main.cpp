#include <iostream>

#include "../test_format/format.hpp"
#include "fixed_string.hpp"

namespace {

	typedef utils::fixed_string<1024> fxstring;

}

int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	{
		fxstring s;
		utils::format("%d, '%s'\n") % s.size() % s.c_str();
	}

	{
		fxstring s("asdfgh");
		utils::format("%s\n") % s.c_str();
	}

	{
		fxstring s;
		s = "zxcvb";
		utils::format("%d, '%s'\n") % s.size() % s.c_str();
	}

	{
		fxstring s("poiuyt_");
		utils::format("%d, '%s'\n") % s.size() % s.c_str();
		s += "lkjhgf";
		utils::format("%d, '%s'\n") % s.size() % s.c_str();
	}

	{
		static const char* p = { "poiuyt" };
		fxstring s("poiuyt");
		if(s == p) {
			utils::format("%d, '%s' : '%s' EQ\n") % s.size() % s.c_str() % p;
		} else {
			utils::format("%d, '%s' : '%s' NE\n") % s.size() % s.c_str() % p;
		}
	}

	{
		static const char* p = { "poiuy" };
		fxstring s("poiuyt");
		if(s != p) {
			utils::format("%d, '%s' : '%s' NE\n") % s.size() % s.c_str() % p;
		} else {
			utils::format("%d, '%s' : '%s' EQ\n") % s.size() % s.c_str() % p;
		}
	}
}
