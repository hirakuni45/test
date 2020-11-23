#include <iostream>
#include <boost/format.hpp>
#include <limits>

#include <mpfr.h>

#include "format.hpp"

namespace {

	void test_mpfr_()
	{
		mpfr_t a;
		mpfr_t b;
		mpfr_t c;
		mpfr_t x;
		mpfr_t y;

		mpfr_init2 (c, 50);
		mpfr_set_d (c, 2.0, MPFR_RNDD);

		mpfr_init2 (y, 50);
		mpfr_set_d (y, -0, MPFR_RNDD);
		mpfr_init2 (x, 50);
		mpfr_set_d (x, +0, MPFR_RNDD);

		mpfr_init2 (a, 50);
		mpfr_init2 (b, 50);

		mpfr_sqrt(a, c, MPFR_RNDD);

		mpfr_const_pi(b, MPFR_RNDD);

		mpfr_printf("%.50RNf\n", a);
		mpfr_printf("%.50RNf\n", b);

		mpfr_clear (c);
		mpfr_clear (x);
		mpfr_clear (y);
		mpfr_clear (a);
		mpfr_clear (b);
	}

}

int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	using namespace utils;

	test_mpfr_();
}
