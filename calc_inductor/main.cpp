#include <iostream>

#include "format.hpp"

int main(int argc, char* argv[]);

namespace {

	double calc_indu_(double vin, double vout, double iout, double r, double fsw)
	{
		return ((vin - vout) * vout) / (vin * fsw * r * iout); 
	}
}

int main(int argc, char* argv[])
{
	double vinp = 100.0 * 1.41421356;
	double vout = 140.0;
	double iout = 5.0;
	double r = 0.3;
	double fsw = 96e6 / 512.0;

	{
		auto a = calc_indu_(vinp, vout, iout, r, fsw);
		utils::format("Vinp: %6.3f, ") % vinp;
		utils::format("Vout: %6.3f, ") % vout;
		utils::format("Iout: %6.3f, ") % iout;
		utils::format("%6.3f [uH]\n") % (a * 1e6);
	}

	{
		vout = 70.0;
		iout = 2.5;
		auto a = calc_indu_(vinp, vout, iout, r, fsw);
		utils::format("Vinp: %6.3f, ") % vinp;
		utils::format("Vout: %6.3f, ") % vout;
		utils::format("Iout: %6.3f, ") % iout;
		utils::format("%6.3f [uH]\n") % (a * 1e6);
	}

	{
		vout = 5.0;
		iout = 0.1;
		auto a = calc_indu_(vinp, vout, iout, r, fsw);
		utils::format("Vinp: %6.3f, ") % vinp;
		utils::format("Vout: %6.3f, ") % vout;
		utils::format("Iout: %6.3f, ") % iout;
		utils::format("%6.3f [uH]\n") % (a * 1e6);
	}
}
