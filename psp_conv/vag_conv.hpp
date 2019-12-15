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

		static uint32_t big4_(const void* src) {
			const uint8_t* p = static_cast<const uint8_t*>(src);
			uint32_t v = p[0];
			v <<= 8;
			v |= p[1];
			v <<= 8;
			v |= p[2];
			v <<= 8;
			v |= p[3];
			return v;
		}

	public:
		static OUT encode(const char* name, const al::audio aif, int loop_start, uint32_t loop_trg)
		{
			sceVagConvertInit(SCE_VAG_CONVMODE_STD);

			OUT out;

			auto sample_num = aif->get_samples();
			auto block_num = sample_num / 28;
			{
				short tmp[48 / sizeof(short)];
				auto rate = aif->get_rate();
				uint32_t ch = aif->get_chanel();
				char n[16] = { 0 };
				strncpy(n, name, 16);
				memcpy(&tmp[32 / sizeof(short)], n, 16);
				
/// loop_start = -1;
/// loop_trg = block_num - 1;
				auto size = block_num * 16;
//				if(loop_start < 0) {
//					size += 16;
//				}
				sceVagConvertHeader(tmp, SCE_VAG_HDRVER_201, size, rate, ch, loop_start, loop_trg);

//	utils::format("Block: %d, Loop start: %d, Loop trg: %d\n") % block_num % loop_start % loop_trg;
///				utils::format("Header: %d, size: %u\n") % sizeof(tmp) % big4_(&tmp[6]);
				write_(out, tmp, sizeof(tmp));
			}

			uint32_t pos = 0;
			uint32_t block = 0;
			while(block < block_num) {
				short tmp[8];
				short src[28];
				short attr = SCE_VAG_BLKATR_1SHOT;
				if(loop_start >= 0) {
					if(loop_start == block) { 
						attr = SCE_VAG_BLKATR_LOOPSTART;
					} else if(loop_trg == block) {
						attr = SCE_VAG_BLKATR_LOOPEND;
					} else {
						attr = SCE_VAG_BLKATR_LOOPBODY;
					}
				} else {
					if(block == (block_num - 1)) {
						attr = SCE_VAG_BLKATR_1SHOTEND;
					}
				}
				for(int i = 0; i < SCE_VAG_BLOCKSIZE; ++i) {
					al::pcm16_m v(0);
					if((pos + i) < sample_num) {
						aif->get(pos + i, v);
					}
					src[i] = v.w;
				}
				sceVagConvert(src, tmp, attr);
				write_(out, tmp, 16);
				pos += SCE_VAG_BLOCKSIZE;
				++block;
			}
#if 0
			if(loop_start < 0) {
				short tmp[8];
				sceVagConvertFin(tmp);
				write_(out, tmp, 16);
				++block;
			}
#endif
			return out;
		}
	};
}
