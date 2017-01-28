#pragma once
//=====================================================================//
/*! @file
    @brief  input クラス @n
			数値、文字列などの入力クラス
			Copyright 2017 Kunihito Hiramatsu
    @author 平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//

extern "C" {
	char sci_getch(void);
};

namespace utils {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  標準入力ファンクタ
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	struct def_chainp {
		const char* str_;
		def_chainp(const char* str) : str_(str) { }

		char operator() () {
			if(str_ == nullptr) sci_getch();
			return *str_++;
		}
	};


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  汎用入力クラス
		@param[in]	INP	入力クラス
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class INP>
	class basic_input {

		const char*	form_;

		INP		inp_;

		char	last_;

		int32_t dec_() {
			int32_t a = 0;
			while(1) {
				char ch = inp_();
				if(ch >= '0' && ch <= '9') {
					a *= 10;
					a += ch - '0';
				} else {
					last_ = ch;
					break;
				}
			}
			return a;
		}

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief  コンストラクター
		*/
		//-----------------------------------------------------------------//
		basic_input(const char* form, const char* inp = nullptr) : form_(form), inp_(inp), last_(0)
		{
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  オペレーター「%」（int）
			@param[in]	val	整数値
			@return	自分の参照
		*/
		//-----------------------------------------------------------------//
		basic_input& operator % (int& val) {

			val = dec_();
			return *this;
		}


	};

	typedef basic_input<def_chainp> input;

}
