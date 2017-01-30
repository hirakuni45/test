#pragma once
//=====================================================================//
/*! @file
    @brief  input クラス @n
			数値、文字列などの入力クラス
			Copyright 2017 Kunihito Hiramatsu
    @author 平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include <cstdint>

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
		uint32_t	cnt_;
		def_chainp(const char* str = nullptr) : str_(str), cnt_(0) { }

		void prev() {
			if(cnt_ > 0) {
				--cnt_;
				--str_;
			}
		}

		char operator() () {
			if(str_ == nullptr) {
				return sci_getch();
			} else {
				auto ch = *str_;
				if(ch != 0) { ++str_; ++cnt_; }
				return ch;
			}
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

		uint32_t bin_() {
			uint32_t a = 0;
			char ch;
			while((ch = inp_()) != 0 && ch >= '0' && ch <= '1') {
				a <<= 1;
				a += ch - '0';
			}
			return a;
		}

		uint32_t oct_() {
			uint32_t a = 0;
			char ch;
			while((ch = inp_()) != 0 && ch >= '0' && ch <= '7') {
				a <<= 3;
				a += ch - '0';
			}
			return a;
		}

		uint32_t dec_() {
			uint32_t a = 0;
			char ch;
			while((ch = inp_()) != 0 && ch >= '0' && ch <= '9') {
				a *= 10;
				a += ch - '0';
			}
			return a;
		}

		uint32_t hex_() {
			uint32_t a = 0;
			char ch;
			while((ch = inp_()) != 0) {
				if(ch >= '0' && ch <= '9') {
					a <<= 4;
					a += ch - '0';
				} else if(ch >= 'A' && ch <= 'F') {
					a <<= 4;
					a += ch - 'A' + 10;
				} else if(ch >= 'a' && ch <= 'f') {
					a <<= 4;
					a += ch - 'a' + 10;
				} else {
					break;
				}
			}
			return a;
		}

		enum class mode : uint8_t {
			NONE,
			BIN,
			OCT,
			DEC,
			HEX,
		};
		mode	mode_;
		bool	err_;

		void next_()
		{
			bool ena = false;
			char ch;
			while((ch = *form_++) != 0) {
				if(ena) {
					if(ch == 'b' || ch == 'B') {
						mode_ = mode::BIN;
						return;
					} else if(ch == 'o' || ch == 'O') {
						mode_ = mode::OCT;
						return;
					} else if(ch == 'd' || ch == 'D') {
						mode_ = mode::DEC;
						return;
					} else if(ch == 'x' || ch == 'X') {
						mode_ = mode::HEX;
						return;
					} else {
						err_ = true;
					}
				} else {
					if(ch == '%' && *form_ != '%') ena = true;
					else if(ch != inp_()) {
						err_ = true;
						return;
					}
				}
			}
		}

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief  コンストラクター
		*/
		//-----------------------------------------------------------------//
		basic_input(const char* form, const char* inp = nullptr) : form_(form), inp_(inp),
			mode_(mode::NONE), err_(false)
		{
			next_();
		}


		//-----------------------------------------------------------------//
		/*!
			@brief  オペレーター「%」（int）
			@param[in]	val	整数値
			@return	自分の参照
		*/
		//-----------------------------------------------------------------//
		basic_input& operator % (int& val)
		{
			switch(mode_) {
			case mode::BIN:
				val = bin_();
				inp_.prev();
				next_();
				break;
			case mode::OCT:
				val = oct_();
				inp_.prev();
				next_();
				break;
			case mode::DEC:
				val = dec_();
				inp_.prev();
				next_();
				break;
			case mode::HEX:
				val = hex_();
				inp_.prev();
				next_();
				break;
			default:
				break;
			}
			return *this;
		}
	};

	typedef basic_input<def_chainp> input;

}
