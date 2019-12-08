/* SIE CONFIDENTIAL
* VAG Converter 2 version 2.5.0.2
* Copyright(C) 2018 Sony Interactive Entertainment Inc.
* All Rights Reserved
*/

#ifndef	_SCE_VAGCONV2_H_
#define	_SCE_VAGCONV2_H_
/*
	define(s)
*/
/* header version */
#define SCE_VAG_HDRVER_201			(0x00020001)
#define SCE_VAG_HDRVER_300			(0x00030000)

/* conversion mode */
#define	SCE_VAG_CONVMODE_BC			(0)
#define	SCE_VAG_CONVMODE_STD		(1)
#define	SCE_VAG_CONVMODE_HARD		(2)
#define	SCE_VAG_CONVMODE_SOFT		(3)
#define	SCE_VAG_CONVMODE_NOPREDICT	(4)

/* block attribute */
#define	SCE_VAG_BLKATR_1SHOT		(0)
#define	SCE_VAG_BLKATR_1SHOTEND		(1)
#define	SCE_VAG_BLKATR_LOOPSTART	(2)
#define	SCE_VAG_BLKATR_LOOPBODY		(3)
#define	SCE_VAG_BLKATR_LOOPEND		(4)

/* block size */
#define	SCE_VAG_BLOCKSIZE			(28)

#define	DLLEXP_VOID	extern "C" __declspec(dllexport) void  __stdcall
/*
	function(s)
*/

DLLEXP_VOID sceVagConvertInit( short conversion_mode );
DLLEXP_VOID sceVagConvertHeader( short y[], int version, int size, int rate, int ch, int loopTop, int loopEnd );
DLLEXP_VOID sceVagConvert( short x[], short y[], short block_attribute );
DLLEXP_VOID sceVagConvertFin( short y[] );

#endif	/* _SCE_VAGCONV2_H_ */
