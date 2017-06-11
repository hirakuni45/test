#include <iostream>

#include "../test_format/format.hpp"

int main(int argc, char* argv[]);

namespace {

	double calc_indu_(double vin, double vout, double iout, double r, double fsw)
	{
		return ((vin - vout) * vout) / (vin * fsw * r * iout); 
	}

	double ipeak_(double iout, double r) {
		return iout + (r * iout) / 2.0;
	}
}

int main(int argc, char* argv[])
{
#if 1
	double vinp = 100.0 * 1.41421356;
	double vout = 110.0;
	double iout = 3.0;
	double r = 0.3;
	double fsw = 96e6 / 512.0;

	{
		auto a = calc_indu_(vinp, vout, iout, r, fsw);
		auto b = ipeak_(iout, r);
		utils::format("Vinp: %6.3f, ") % vinp;
		utils::format("Vout: %6.3f, ") % vout;
		utils::format("Iout: %6.3f, ") % iout;
		utils::format("Peak: %6.3f, ") % b;
		utils::format("%6.3f [uH]\n") % (a * 1e6);
	}

	{
		vout = 55.0;
		iout = 1.5;
		auto a = calc_indu_(vinp, vout, iout, r, fsw);
		utils::format("Vinp: %6.3f, ") % vinp;
		utils::format("Vout: %6.3f, ") % vout;
		utils::format("Iout: %6.3f, ") % iout;
		utils::format("%6.3f [uH]\n") % (a * 1e6);
	}

	{
		vout = 5.0;
		iout = 0.1;
		r = 0.5;
		auto a = calc_indu_(vinp, vout, iout, r, fsw);
		utils::format("Vinp: %6.3f, ") % vinp;
		utils::format("Vout: %6.3f, ") % vout;
		utils::format("Iout: %6.3f, ") % iout;
		utils::format("%6.3f [uH]\n") % (a * 1e6);
	}
#endif

#if 0
	double vinp = 20.0;
	double vout = 18.0;
	double iout = 10.0;
	double r = 0.3;
	double fsw = 80e6 / 512.0;

	{
		auto a = calc_indu_(vinp, vout, iout, r, fsw);
		auto b = ipeak_(iout, r);
		utils::format("Vinp: %6.3f, ") % vinp;
		utils::format("Vout: %6.3f, ") % vout;
		utils::format("Iout: %6.3f, ") % iout;
		utils::format("Peak: %6.3f, ") % b;
		utils::format("%6.3f [uH]\n") % (a * 1e6);
	}

	{
		vout = 0.5;
		iout = 0.1;
		auto a = calc_indu_(vinp, vout, iout, r, fsw);
		auto b = ipeak_(iout, r);
		utils::format("Vinp: %6.3f, ") % vinp;
		utils::format("Vout: %6.3f, ") % vout;
		utils::format("Iout: %6.3f, ") % iout;
		utils::format("Peak: %6.3f, ") % b;
		utils::format("%6.3f [uH]\n") % (a * 1e6);
	}
#endif
}
