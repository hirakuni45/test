#pragma once
//=====================================================================//
/*!	@file
	@brief	PSP/PHD 構造
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2019 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/glfw_app/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>
#include <cstring>

namespace psp {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	PPTN / toneParam
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	class toneParam {
	public:

		static uint32_t get32(const void* top) {
			auto p = static_cast<const uint8_t*>(top);
			uint32_t val = 0;
			val  = static_cast<uint32_t>(p[0]);
			val |= static_cast<uint32_t>(p[1]) << 8;
			val |= static_cast<uint32_t>(p[2]) << 16;
			val |= static_cast<uint32_t>(p[3]) << 24;
			return val;
		}


		uint32_t	vagParamCtxIndex;	///< このトーンパラメータが使用するVAG パラメータのインデックス（ノイズ音源の場合は 0xffffffff）
		uint32_t	priority;			///< プライオリティ 0(低)～127(高)
		uint32_t	group;				///< グループ 0～127
		uint32_t	noiseClock;			///< ノイズクロック 0～60
		uint32_t	note_low;			///< 発音下限ノートナンバー 0～127
		uint32_t	note_high;			///< 発音上限ノートナンバー 0～127
		uint32_t	reserve1[2];
		int32_t		volume;				///< ボリューム 0～4096
		uint32_t	panpot;				///< パンポット 0(左)～64～127(右)
		int32_t		sendVolume;			///< エフェクトセンドボリューム 0～4096
		uint32_t	sendPanpot;			///< エフェクトセンドパンポット 0(左)～64～127(右)
		uint32_t	centerNote;			///< サンプリング時のノートナンバー 0～127
		int32_t		centerDetune;		///< サンプリング時のデチューン -99～99(cent)
		int32_t		detune;				///< デチューン -99～99(cent)
		uint32_t	reserve2;
		uint32_t	bendRange_lower;	///< ベンドレンジ－ 0～24(ノート数)
		uint32_t	bendRange_upper;	///< ベンドレンジ＋ 0～24(ノート数)
		uint32_t	portamentTime;		///< ポルタメント推移時間 0
		uint32_t	reserve3;
		uint32_t	adsr1;				///< ADSR1（エンベロープ）
		uint32_t	adsr2;				///< ADSR2（エンベロープ）
		uint32_t	reserve4[2];


		void list() const {
			utils::format("    vagParamCtxIndex: %d\n") % vagParamCtxIndex;
			utils::format("    priority:         %d\n") % priority;
			utils::format("    group:            %d\n") % group;
			utils::format("    noiseClock:       %d\n") % noiseClock;
			utils::format("    note_low:         %d\n") % note_low;
			utils::format("    note_high:        %d\n") % note_high;
			utils::format("    volume:           %d\n") % volume;
			utils::format("    panpot:           %d\n") % panpot;
			utils::format("    sendVolume:       %d\n") % sendVolume;
			utils::format("    sendPanpot:       %d\n") % sendPanpot;
			utils::format("    centerNote:       %d\n") % centerNote;
			utils::format("    centerDetune:     %d\n") % centerDetune;
			utils::format("    detune:           %d\n") % detune;
			utils::format("    bendRange_lower:  %d\n") % bendRange_lower;
			utils::format("    bendRange_upper:  %d\n") % bendRange_upper;
			utils::format("    portamentTime:    %d\n") % portamentTime;
			utils::format("    adsr1:            %d\n") % adsr1;
			utils::format("    adsr2:            %d\n") % adsr2;
		}


		void load(const void* org)
		{
			auto src = static_cast<const uint32_t*>(org);
			auto dst = reinterpret_cast<uint32_t*>(this);
			for(int i = 0; i < sizeof(toneParam); i += 4) {
				auto v = get32(src);
				++src;
				memcpy(dst, &v, 4);
				++dst;
			}
		}
	};


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	PPPG / progParam
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	class progParam {
	public:
		uint32_t	toneParamCtx_count;	///< 
	};
}

