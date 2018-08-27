#include <iostream>

#include "../test_format/format.hpp"
#include "fixed_string.hpp"

namespace {

	typedef utils::fixed_string<1024> fxstring;
	typedef utils::fixed_string<16> fxstring16;

}

int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	{  // 標準コンストラクタ検査
		fxstring s;
		utils::format("%d, '%s'\n") % s.size() % s.c_str();
	}

	{  // コンストラクタ検査
		fxstring s("asdfgh");
		utils::format("%d, '%s'\n") % s.size() % s.c_str();
	}

	{  // コンストラクタ検査（オーバーフロー時）
		fxstring16 s("0123456789ABCDEFGHIJKLMN");
		utils::format("%d, '%s'\n") % s.size() % s.c_str();
	}

	{  // 代入検査
		fxstring s;
		s = "zxcvb";
		utils::format("%d, '%s'\n") % s.size() % s.c_str();
	}

	{  // 代入検査（オーバーフロー時）
		fxstring16 s;
		s = "0123456789ABCDEFGHIJKLMN";
		utils::format("%d, '%s'\n") % s.size() % s.c_str();
	}

	{  // 追加検査
		fxstring s("poiuyt_");
		utils::format("%d, '%s'\n") % s.size() % s.c_str();
		s += "lkjhgfghiJKLM";
		utils::format("%d, '%s'\n") % s.size() % s.c_str();
	}
	{  // 追加検査（オーバーフロー時）
		fxstring16 s("poiuyt_");
		utils::format("%d, '%s'\n") % s.size() % s.c_str();
		s += "lkjhgfghiJKLM";
		utils::format("%d, '%s'\n") % s.size() % s.c_str();
	}

	{  // = オペレーター検査 EQ
		static const char* p = { "poiuyt" };
		fxstring s("poiuyt");
		if(s == p) {
			utils::format("%d, '%s' : '%s' EQ\n") % s.size() % s.c_str() % p;
		} else {
			utils::format("%d, '%s' : '%s' NE\n") % s.size() % s.c_str() % p;
		}
	}
	{  // = オペレーター検査 NE
		static const char* p = { "poIuyt" };
		fxstring s("poiuyt");
		if(s == p) {
			utils::format("%d, '%s' : '%s' EQ\n") % s.size() % s.c_str() % p;
		} else {
			utils::format("%d, '%s' : '%s' NE\n") % s.size() % s.c_str() % p;
		}
	}

	{  // for
		fxstring s("0123456789");
		for(auto it = s.begin(); it != s.end(); ++it) {
			utils::format("%c") % *it;
		}
		utils::format("\n");
	}
}
