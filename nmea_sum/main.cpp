// NMEA check sum

#include <cstdint>
#include "format.hpp"

// static const char* str = "$PMTK251,9600*17";
// static const char* str = "$PMTK251,38400*27";
static const char* str = "PMTK251,38400";

int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{

	char ch;
	const char* p = str;
	uint32_t sum = 0;
	while((ch = *p) != 0) {
		sum ^= static_cast<uint32_t>(ch);
		++p;
	}

	utils::format("%02X\n") % (sum & 0xff);
}
