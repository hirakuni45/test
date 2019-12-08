#pragma once
//=====================================================================//
/*!	@file
	@brief	PS4/VAG Encoder
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2019 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/glfw_app/blob/master/LICENSE
*/
//=====================================================================//
#include "utils/file_io.hpp"
#include "utils/format.hpp"
#include "snd_io/pcm.hpp"

#include "vagconv2.h"

namespace ps4 {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	PS4/VAG エンコーダー
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	class vag_conv {
	public:

		typedef std::vector<uint8_t> OUT;

	private:

		static void write_(OUT& out, const void* src, uint32_t len) {
			const uint8_t* p = static_cast<const uint8_t*>(src);
			for(uint32_t i = 0; i < len; ++i) {
				out.push_back(p[i]);
			}
		}

	public:
		static OUT encode(const char* name, const al::audio aif, bool loop)
		{
			sceVagConvertInit(SCE_VAG_CONVMODE_STD);

			OUT out;

			uint32_t num = aif->get_samples();
			{
				short tmp[48 / sizeof(short)];
				auto rate = aif->get_rate();
				uint32_t ch = aif->get_chanel();
				uint32_t loop_top = 0;
				uint32_t loop_end = 0;
				if(loop) {
					loop_end = num - 1;
				}
				char n[16] = { 0 };
				strncpy(n, name, 16);
				memcpy(&tmp[32 / sizeof(short)], n, 16);
				sceVagConvertHeader(tmp, SCE_VAG_HDRVER_201, num / 2, rate, ch,
					loop_top / 2, loop_end / 2);
				write_(out, tmp, sizeof(tmp));
			}

			uint32_t pos = 0;
			while(pos < num) {
				short tmp[8];
				short src[28];
				short attr = SCE_VAG_BLKATR_1SHOT;
				for(int i = 0; i < 28; ++i) {
					al::pcm16_m v(0);
					if((pos + i) < num) {
						aif->get(pos + i, v);
					}
					src[i] = v.w;
				}
				if((pos + SCE_VAG_BLOCKSIZE) >= num) {
					attr = SCE_VAG_BLKATR_1SHOTEND;
				}
				sceVagConvert(src, tmp, attr);
				write_(out, tmp, 16);
				pos += SCE_VAG_BLOCKSIZE;
			}

			return out;
		}
	};
}
