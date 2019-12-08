/* SIE CONFIDENTIAL
 * HE-VAG DLL version 4.0.0.0
 * Copyright (C) 2015 Sony Interactive Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef _SCE_HEVAG_ADAPTER_H
#define _SCE_HEVAG_ADAPTER_H

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/**
 * Audio decoder type
 */
#define SCE_AUDIODEC_CPU_TYPE_HEVAG					0x1008

/**
 * Maximum output channels
 */
#define SCE_AUDIODEC_CPU_HEVAG_MAX_CHANNELS			8

/**
 * HE-VAG data block definitions
 */
#define SCE_AUDIODEC_CPU_HEVAG_BLOCK_SAMPLES		28		///< Block samples
#define SCE_AUDIODEC_CPU_HEVAG_BLOCK_SIZE			16		///< Block size [byte]

/*
 * Parameter value definitions
 */

/**
 * Output format
 */
typedef enum
{
	SCE_AUDIODEC_CPU_HEVAG_INTERLEAVED_OUTPUT  	= 0,	///< Interleaved
	SCE_AUDIODEC_CPU_HEVAG_NON_INTERLEAVED_OUTPUT		///< Non-interleaved
} SceAudiodecCpuHevagOutputFormatParam;

/*
 * Error number definition
 */

/**
 * @j 不正なHE-VAGのチャネル数が指定された @ej
 * @e Specified invalid number of channels of HE-VAG @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_HEVAG_INVALID_CHANNELS -2131293184	/* 0x80F70C00 */

/**
 * @j 不正なHE-VAGの出力サンプリング周波数が指定された @ej
 * @e Specified invalid output sampling frequency of HE-VAG @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_HEVAG_INVALID_SAMPLING_FREQ -2131293183	/* 0x80F70C01 */

/**
 * @j 不正なHE-VAGのバージョンが指定された @ej
 * @e Specified invalid version of HE-VAG @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_HEVAG_INVALID_VERSION -2131293182	/* 0x80F70C02 */

/**
 * @j 不正なHE-VAGの識別IDが指定された @ej
 * @e Specified invalid identification ID of HE-VAG @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_HEVAG_INVALID_ID	-2131293181	/* 0x80F70C03 */

/**
 * @j 不正なHE-VAGのデータサイズが指定された @ej
 * @e Specified invalid data size of HE-VAG @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_HEVAG_INVALID_DATA_SIZE -2131293180	/* 0x80F70C04 */

/**
 * @j 不正なHE-VAGのデータ名称が指定された @ej
 * @e Specified invalid data name of HE-VAG @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_HEVAG_INVALID_DATA_NAME -2131293179	/* 0x80F70C05 */

/**
 * @j 不正なHE-VAGの出力形式が指定された @ej
 * @e Specified invalid output format of HE-VAG @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_HEVAG_INVALID_OUTPUT_FORMAT -2131293178	/* 0x80F70C06 */

/**
 * @j VAG Headerが見つからなかった @ej
 * @e Not found HE-VAG header @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_HEVAG_NOT_FOUND_HEADER -2131293177	/* 0x80F70C07 */

/**
 * @j 不正なHE-VAGのループフラグを検出した @ej
 * @e Detected invalid loop flag of HE-VAG @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_HEVAG_INVALID_LOOP_FLAG -2131293176	/* 0x80F70C08 */

typedef struct SceAudiodecCpuParamHevag {
	uint32_t	uiSize;						///< Size of this structure
	uint8_t		ucId[4];					///< Identification ID
	uint32_t	uiVersion;					///< Version
	uint8_t		ucReserved1[4];				///< Reserved
	uint32_t	uiDataSize;					///< Data size
	uint32_t	uiFs;						///< Sampling Frequency(Hz)
	uint8_t		ucReserved2[10];			///< Reserved
	uint8_t		ucNumChannels;				///< Number of channels
	uint8_t		ucReserved3;				///< Reserved
	uint8_t		ucName[16];					///< Data name
	int32_t 	iBwPcm;						///< PCM data word size
	uint8_t		ucOutFormat;				///< Output format
	uint8_t		ucReserved4[7];				///< Reserved
} SceAudiodecCpuParamHevag;

typedef struct SceAudiodecCpuHevagInfo {
	uint32_t	uiSize;						///< Size of this structure
	uint8_t		ucId[4];					///< Identification ID
	uint32_t	uiVersion;					///< Version
	uint32_t	uiDataSize;					///< Data size
	uint32_t	uiFs;						///< Sampling frequency (Hz)
	uint8_t		ucNumChannels;				///< Number of channels
	uint8_t 	ucReserved1[3];				///< Reserved
	uint8_t		ucName[16];					///< Data name
	uint8_t		ucVagLoopFlag[8];			///< Loop flag
} SceAudiodecCpuHevagInfo;

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /* _SCE_HEVAG_ADAPTER_H */
