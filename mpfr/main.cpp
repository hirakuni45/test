#include "format.hpp"
#include "mpfr.hpp"

#include "basic_arith.hpp"
#include "calc_symbol.hpp"
#include "calc_func.hpp"

namespace {

#if 0
	void test_mpfr_()
	{
		mpfr_t a;
		mpfr_t b;
		mpfr_t c;

		mpfr_init2 (a, 100);
		mpfr_init2 (b, 100);

		mpfr_init2 (c, 100);
		mpfr_set_d (c, 2.0, MPFR_RNDN);

		mpfr_sqrt(a, c, MPFR_RNDN);

		mpfr_const_pi(b, MPFR_RNDN);

		mpfr_printf("%.95RNf\n", a);
		mpfr_printf("%.95RNf\n", b);

		mpfr_clear (c);
		mpfr_clear (b);
		mpfr_clear (a);
	}
#endif

	typedef mpfr::value<50> NVAL;

	typedef utils::calc_def CDEF;

	typedef utils::calc_symbol<NVAL> SYMBOL;
	SYMBOL	symbol_;

	typedef utils::calc_func<NVAL> FUNC;
	FUNC	func_;

	typedef utils::basic_arith<NVAL, SYMBOL, FUNC> ARITH;
	ARITH	arith_(symbol_, func_);
}

int main(int argc, char* argv[]);

int main(int argc, char* argv[])
{
#if 0
	VAL a(2.0);
	VAL b(4.0);

	a = a / b;

	char tmp[100];
	a(tmp, sizeof(tmp));
	utils::format("%s\n") % tmp;
#endif
	{
		char tmp[100];
//		utils::sformat("%c(2)", tmp, sizeof(tmp)) % static_cast<char>(CDEF::NAME::SQRT);
		utils::sformat("%c(45)", tmp, sizeof(tmp)) % static_cast<char>(CDEF::NAME::SIN);
		arith_.analize(tmp);
	}

	{
		auto ans = arith_();
		char tmp[200];
		ans(tmp, sizeof(tmp));
		utils::format("%s\n") % tmp;
	}
}
