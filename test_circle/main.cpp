#include <iostream>

#include "../test_format/format.hpp"
#include "circle.hpp"

int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	typedef imath::circle CIRCLE;
	CIRCLE	cir;

	uint32_t r = 100;
	auto p0 = CIRCLE::angle_to_position(0.0f, r);
	utils::format("Start: %d, %d\n") % p0.x % p0.y;

	auto p1 = CIRCLE::angle_to_position(0.5f, r);
	utils::format("Final: %d, %d\n") % p1.x % p1.y;

	cir.start(p0, vtx::ipos(0), p1);

	uint32_t i = 0;
	bool f = false;
	do {
		auto p = cir.get_position();
		utils::format("(%d) %d, %d\n") % i % p.x % p.y;
		f = cir.step();
		++i;
	} while(!f) ;
}
