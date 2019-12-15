#pragma once
//=====================================================================//
/*!	@file
	@brief	PSP ADPCM decoder
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2019 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/glfw_app/blob/master/LICENSE
*/
//=====================================================================//
#include "utils/format.hpp"
#include "snd_io/pcm.hpp"

namespace psp {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	PSP ADPCM デコーダー
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	class adpcm {

		typedef uint8_t u8;
		typedef int16_t s16;
		typedef uint32_t u32;

		s16 samples[28];
		int curSample = 0;

		int curBlock_ = -1;
		int loopStartBlock_ = -1;
		int numBlocks_ = 0;

		// rolling state. start at 0, should probably reset to 0 on loops?
		int s_1 = 0;
		int s_2 = 0;

		bool loopEnabled_ = false;
		bool loopAtNextBlock_ = false;
		bool end_ = false;

		static inline s16 clamp_s16(int i) {
			if (i > 32767)
				return 32767;
			if (i < -32768)
				return -32768;
			return i;
		}

	public:
		adpcm() : samples{ 0 }
		{ }


		//-----------------------------------------------------------------//
		/*!
			@brief	開始
			@param[in]	vagSize	VAG データサイズ
			@param[in]	loopEnabled	ループを有効にする場合「true」
			@return	負の場合、ループ無し
		*/
		//-----------------------------------------------------------------//
		void start(uint32_t vagSize, bool loopEnabled)
		{
			loopEnabled_ = loopEnabled;
			loopAtNextBlock_ = false;
			loopStartBlock_ = -1;
			numBlocks_ = vagSize / 16;
			end_ = false;
			curSample = 28;
			curBlock_ = -1;
			s_1 = 0;	// per block?
			s_2 = 0;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	PSP 形式 ADPCM データのデコード
			@param[in]	src		ソース
			@return	次回ブロックのポインター
		*/
		//-----------------------------------------------------------------//
		const uint8_t* decode_block(const uint8_t* src)
		{
			static const uint8_t f[16][2] = {
				{   0,   0 },
				{  60,   0 },
				{ 115,  52 },
				{  98,  55 },
				{ 122,  60 },
				// TODO: The below values could use more testing, but match initial tests.
				// Not sure if they are used by games, found by tests.
				{   0,   0 },
				{   0,   0 },
				{  52,   0 },
				{  55,   2 },
				{  60, 125 },
				{   0,   0 },
				{   0,  91 },
				{   0,   0 },
				{   2, 216 },
				{ 125,   6 },
				{   0, 151 },
			};

			const uint8_t* readp = src;

			if (curBlock_ == numBlocks_ - 1) {
				end_ = true;
				return readp;
			}

			int predict_nr = *readp++;
			int shift_factor = predict_nr & 0xf;
			predict_nr >>= 4;
			int flags = *readp++;
			if (flags == 7) {
//				utils::format("VAG ending block at %d\n") % curBlock_;
				end_ = true;
				return readp;
			}
			else if (flags == 6) {
				loopStartBlock_ = curBlock_;
///				utils::format("Start loop: %d\n") % loopStartBlock_;
			}
			else if (flags == 3) {
				if (loopEnabled_) {
					loopAtNextBlock_ = true;
				}
///				utils::format("Loop next: %d -> %d\n") % curBlock_ % loopStartBlock_;
			}

			// Keep state in locals to avoid bouncing to memory.
			int s1 = s_1;
			int s2 = s_2;

			int coef1 =  f[predict_nr][0];
			int coef2 = -f[predict_nr][1];

			// TODO: Unroll once more and interleave the unpacking with the decoding more?
			for (int i = 0; i < 28; i += 2) {
				u8 d = *readp++;
				int sample1 = (short)((d & 0xf) << 12) >> shift_factor;
				int sample2 = (short)((d & 0xf0) << 8) >> shift_factor;
				s2 = clamp_s16(sample1 + ((s1 * coef1 + s2 * coef2) >> 6));
				s1 = clamp_s16(sample2 + ((s2 * coef1 + s1 * coef2) >> 6));
				samples[i] = s2;
				samples[i + 1] = s1;
			}

			s_1 = s1;
			s_2 = s2;
			curSample = 0;
			curBlock_++;

			return readp;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	ループを開始するブロックを取得
			@return	負の場合、ループ無し
		*/
		//-----------------------------------------------------------------//
		auto get_loop_start() const { return loopStartBlock_; }


		//-----------------------------------------------------------------//
		/*!
			@brief	ループか検査
			@return	「true」の場合、loop_start からループ
		*/
		//-----------------------------------------------------------------//
		bool is_loop() const { return loopAtNextBlock_; }


		uint32_t get_current_block() const { return curBlock_; }


		bool is_fin() const { return end_; }


		void render_pcm(al::audio aif)
		{
			for(int i = 0; i < 28; ++i) {
				al::pcm16_m w(samples[i]);
				aif->put(i + curBlock_ * 28, w);
			}
		}


		void render_raw(utils::file_io& fo)
		{
			for(int i = 0; i < 28; ++i) {
				fo.put16(samples[i]);
			}
		}

	};
}
