/* SIE CONFIDENTIAL
 * HE-VAG DLL version 4.0.0.0
 * Copyright (C) 2015 Sony Interactive Entertainment Inc.
 * All Rights Reserved.
 */


#ifndef _SCE_AUDIODEC_CPU_H
#define _SCE_AUDIODEC_CPU_H

#include <stdint.h>


#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#if !defined(SCE_OK)
#define SCE_OK							0
#endif  /* !defined(SCE_OK) */


/**
 * @j 出力PCM データの語長を表す定数 @ej
 * @e Constants representing output PCM data word sizes @ee
 */
#define SCE_AUDIODEC_CPU_WORD_SZ_16BIT	(1)
#define SCE_AUDIODEC_CPU_WORD_SZ_FLOAT	(2)


/*
 * Error number definition
 */

/**
 * @j 内部エラー @ej
 * @e internal error @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_API_FAIL			-2131296256	/* 0x80F70000 */

/**
 * @j 不正なコーデックタイプが指定された @ej
 * @e Specified codec type is invalid @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_INVALID_TYPE		-2131296255	/* 0x80F70001 */

/**
 * @j 不正な引数が指定された @ej
 * @e Specified argument is invalid @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_ARG				-2131296254	/* 0x80F70002 */

/**
 * @j 不正な構造体サイズが指定された @ej
 * @e size of structure is invalid @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_INVALID_SIZE		-2131296253	/* 0x80F70003 */

/**
 * @j SceAudiodecCpuParam***構造体のuiSizeに不正なサイズが指定された @ej
 * @e size of SceAudiodecCpuParam*** structure is invalid @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_INVALID_PARAM_SIZE -2131296252	/* 0x80F70004 */

/**
 * @j SceAudiodecCpu***Info構造体のuiSizeに不正なサイズが指定された @ej
 * @e size of SceAudiodecCpu***Info structure is invalid @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_INVALID_BSI_INFO_SIZE -2131296251	/* 0x80F70005 */

/**
 * @j SceAudiodecCpuAuInfo構造体のuiSizeに不正なサイズが指定された @ej
 * @e size of SceAudiodecCpuAuInfo structure is invalid @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_INVALID_AU_INFO_SIZE -2131296250	/* 0x80F70006 */

/**
 * @j SceAudiodecCpuPcmItem構造体のuiSizeに不正なサイズが指定された @ej
 * @e size of SceAudiodecCpuPcmItem structure is invalid @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_INVALID_PCM_ITEM_SIZE -2131296249	/* 0x80F70007 */

/**
 * @j SceAudiodecCpuResource構造体のuiSizeに不正なサイズが指定された @ej
 * @e size of SceAudiodecCpuResource structure is invalid @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_INVALID_RESOURCE_SIZE -2131296248	/* 0x80F70008 */

/**
 * @j 不正なSceAudiodecCpuCtrlのポインタが指定された @ej
 * @e Specified invalid pointer of SceAudiodecCpuCtrl @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_INVALID_CTRL_POINTER -2131296247	/* 0x80F70009 */

/**
 * @j 不正なpParamポインタが指定された @ej
 * @e Specified invalid pointer of pParam @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_INVALID_PARAM_POINTER -2131296246	/* 0x80F7000A */

/**
 * @j 不正なpBsiInfoポインタが指定された @ej
 * @e Specified invalid pointer of pBsiInfo @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_INVALID_BSI_INFO_POINTER -2131296245	/* 0x80F7000B */

/**
 * @j 不正なpAuInfoポインタが指定された @ej
 * @e Specified invalid pointer of pAuInfo @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_INVALID_AU_INFO_POINTER -2131296244	/* 0x80F7000C */

/**
 * @j 不正なpPcmItemポインタが指定された @ej
 * @e Specified invalid pointer of pPcmItem @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_INVALID_PCM_ITEM_POINTER -2131296243	/* 0x80F7000D */

/**
 * @j 不正なSceAudiodecCpuResourceのポインタが指定された @ej
 * @e Specified invalid pointer of SceAudiodecCpuResource @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_INVALID_RESOURCE_POINTER -2131296242	/* 0x80F7000E */

/**
 * @j 不正なpAuAddrが指定された @ej
 * @e Specified invalid pAuAddr @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_INVALID_AU_POINTER -2131296241	/* 0x80F7000F */

/**
 * @j 不正なpPcmAddrが指定された @ej
 * @e Specified invalid pPcmAddr @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_INVALID_PCM_POINTER -2131296240	/* 0x80F70010 */

/**
 * @j 不正なpWorkMemAddrが指定された @ej
 * @e Specified invalid pWorkMemAddr @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_INVALID_WORK_MEM_POINTER -2131296239	/* 0x80F70011 */

/**
 * @j 不正な語長が指定された @ej
 * @e Specified invalid word length @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_INVALID_WORD_LENGTH -2131296238	/* 0x80F70012 */

/**
 * @j 不正なuiAuSizeが指定された @ej
 * @e Specified invalid uiAuSize @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_INVALID_AU_SIZE	-2131296237	/* 0x80F70013 */

/**
 * @j 不正なuiPcmSizeが指定された @ej
 * @e Specified invalid uiPcmSize @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_INVALID_PCM_SIZE	-2131296236	/* 0x80F70014 */

/**
 * @j 不正なuiWorkMemSizeが指定された @ej
 * @e Specified invalid uiWorkMemSize @ee
 */
#define SCE_AUDIODEC_CPU_ERROR_INVALID_WORK_MEM_SIZE -2131296235	/* 0x80F70015 */

/**
 * @brief
 * Audio decoder resources management structure.
 *
 * This structure is used for managing memory used by the audio decoder.
 *
 * For uiSize, specify the size of this structure.
 * For pWorkMemAddr, specify a pointer to the memory area
 * that was prepared by the application for using the audio decoder.
 * For uiWorkMemSize, specify the size of the memory to be used by the audio decoder.
 *
 * @see
 * sceAudiodecCpuQueryMemSize, sceAudiodecCpuInitDecoder,
 * sceAudiodecCpuDecode, sceAudiodecCpuClearContext
 */
typedef struct SceAudiodecCpuResource {
	uint32_t uiSize;
	void *pWorkMemAddr;
	uint32_t uiWorkMemSize;
} SceAudiodecCpuResource;


/**
 * @brief
 * Access unit information structure.
 *
 * This structure is used for holding access unit information.
 * It is used as part of the SceAudiodecCpuCtrl structure.
 *
 * For uiSize, specify the size of this structure.
 * For pAuAddr, specify a pointer to the access unit buffer to be decoded
 * For uiAuSize, specify the size of the access unit buffer.
 *
 * @see
 * sceAudiodecCpuDecode
 */
typedef struct SceAudiodecCpuAuInfo {
	uint32_t uiSize;
	void *pAuAddr;
	uint32_t uiAuSize;
} SceAudiodecCpuAuInfo;


/**
 * @brief
 * PCM information structure
 *
 * This structure is used for holding information about the buffer that stores
 * the decoded PCM data.
 * It is used as part of the SceAudiodecCpuCtrl structure.
 *
 * For uiSize, specify the size of this structure.
 * For pPcmAddr, specify a pointer to the PCM buffer.
 * For uiPcmSize, specify the size of the PCM buffer.
 *
 * @see
 * sceAudiodecCpuDecode
 */
typedef struct SceAudiodecCpuPcmItem {
	uint32_t uiSize;
	void *pPcmAddr;
	uint32_t uiPcmSize;
} SceAudiodecCpuPcmItem;


/**
 * @brief
 * Audio decoder control structure.
 *
 * This structure is used as an argument to sceAudiodecCpuInitDecoder()
 * when initialize a decoder and by sceAudiodecCpuDecode() when
 * performing decoding.
 *
 * For pParam, when querying memory, specify a pointer to the decoding
 * parameter structure to set the number of channels, etc.
 * Since the required parameters differ depending on the decoder type,
 * a different decoder parameter structure is defined for each decoder type.
 * Specify a pointer to the appropriate parameter structure, cast to void*.
 * For pBsiInfo, specify a pointer to the bit stream information structure for
 * receiving bit stream information. Since the information that can be obtained
 * differs according to the decoder type, a different bit stream information
 * structure is defined for each decoder type. Specify a pointer to the
 * appropriate bit stream information structure, cast to void*.
 * For pAuInfo, when performing decoding, specify a pointer to the 
 * SceAudiodecCpuAuInfo structure which has been set with information of the
 * access unit to be decoded.
 * For pPcmItem, when performing decoding, specify a pointer to the
 * SceAudiodecCpuPcmItem structure which has been set with information of the
 * decoded PCM.
 *
 * @see
 * sceAudiodecCpuQueryMemSize, sceAudiodecCpuInitDecoder,
 * sceAudiodecCpuDecode
 */
typedef struct SceAudiodecCpuCtrl {
	void *pParam;
	void *pBsiInfo;
	SceAudiodecCpuAuInfo *pAuInfo;
	SceAudiodecCpuPcmItem *pPcmItem;
} SceAudiodecCpuCtrl;


#ifdef SCE_AUDIODEC_CPU_HEVAGDLL
#define SCE_AUDIODEC_CPU_DLLIMPORT		__declspec(dllexport)
#else /* HEVAGDLL */
#define SCE_AUDIODEC_CPU_DLLIMPORT		__declspec(dllimport)
#endif /* HEVAGDLL */

/* prototype */
SCE_AUDIODEC_CPU_DLLIMPORT int32_t sceAudiodecCpuQueryMemSize(SceAudiodecCpuCtrl *pCtrl, SceAudiodecCpuResource *pRes, uint32_t uiCodecType);
SCE_AUDIODEC_CPU_DLLIMPORT int32_t sceAudiodecCpuInitDecoder(SceAudiodecCpuCtrl *pCtrl, SceAudiodecCpuResource *pRes, uint32_t uiCodecType);
SCE_AUDIODEC_CPU_DLLIMPORT int32_t sceAudiodecCpuDecode(SceAudiodecCpuCtrl *pCtrl, SceAudiodecCpuResource *pRes, uint32_t uiCodecType);
SCE_AUDIODEC_CPU_DLLIMPORT int32_t sceAudiodecCpuClearContext(SceAudiodecCpuResource *pRes, uint32_t uiCodecType);
SCE_AUDIODEC_CPU_DLLIMPORT uint32_t sceAudiodecCpuVersion(void);


#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /* _SCE_AUDIODEC_CPU_H */
