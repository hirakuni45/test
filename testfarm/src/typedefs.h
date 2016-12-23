/*********************************************************************

 ********************************************************************/

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

//#define S_LVL_MAX			(8)
#define S_LVL_MAX			(11)
#define S_LVL_MAX_M1		(S_LVL_MAX-1)
#define M_LVL_MAX			(3)
#define M_LVL_MAX_M1		(M_LVL_MAX-1)

typedef unsigned char   byte;           // 8-bit
typedef unsigned int    word;           // 16-bit
typedef unsigned long   dword;          // 32-bit

typedef union _BYTE
{
    byte _byte;
    struct
    {
        unsigned b0:1;
        unsigned b1:1;
        unsigned b2:1;
        unsigned b3:1;
        unsigned b4:1;
        unsigned b5:1;
        unsigned b6:1;
        unsigned b7:1;
    };
} BYTE;

typedef union _WORD
{
    word _word;
    struct
    {
        byte byte0;
        byte byte1;
    };
    struct
    {
        BYTE Byte0;
        BYTE Byte1;
    };
    struct
    {
        BYTE LowB;
        BYTE HighB;
    };
    struct
    {
        byte v[2];
    };
} WORD;
#define LSB(a)      ((a).v[0])
#define MSB(a)      ((a).v[1])

typedef union _DWORD
{
    dword _dword;
    struct
    {
        byte byte0;
        byte byte1;
        byte byte2;
        byte byte3;
    };
    struct
    {
        word word0;
        word word1;
    };
    struct
    {
        BYTE Byte0;
        BYTE Byte1;
        BYTE Byte2;
        BYTE Byte3;
    };
    struct
    {
        WORD Word0;
        WORD Word1;
    };
    struct
    {
        byte v[4];
    };
} DWORD;
#define LOWER_LSB(a)    ((a).v[0])
#define LOWER_MSB(a)    ((a).v[1])
#define UPPER_LSB(a)    ((a).v[2])
#define UPPER_MSB(a)    ((a).v[3])

typedef void(*pFunc)(void);

typedef union _POINTER
{
    struct
    {
        byte bLow;
        byte bHigh;
        //byte bUpper;
    };
    word _word;                         // bLow & bHigh
    
    //pFunc _pFunc;                       // Usage: ptr.pFunc(); Init: ptr.pFunc = &<Function>;

    byte* bRam;                         // Ram byte pointer: 2 bytes pointer pointing
                                        // to 1 byte of data
    word* wRam;                         // Ram word poitner: 2 bytes poitner pointing
                                        // to 2 bytes of data

    //rom byte* bRom;                     // Size depends on compiler setting
    //rom word* wRom;
    //rom near byte* nbRom;               // Near = 2 bytes pointer
    //rom near word* nwRom;
    //rom far byte* fbRom;                // Far = 3 bytes pointer
    //rom far word* fwRom;
} POINTER;

typedef enum _BOOL { FALSE = 0, TRUE } BOOL;


// 配列ポインタＡ
typedef struct _POINT08A {
	int sta;		// スタートポインタ
	int cnt;		// 要素数
}POINT08A;

// 配列ポインタＢ
typedef struct _POINT08B {
	int sta;		// スタートポインタ
	int cnt;		// 要素数
	byte sum;		// 積算要素数
	byte n;		// ±での一致回数
}POINT08B;

// 座標データ
typedef struct _POINT {
	int x;		// Ｘ座標
	int y;		// Ｙ座標
}POINT;

// 結果移動量
typedef struct _POINT2 {
	int n;		// 出現頻度 or 要素数
	int x;		// Ｘ座標
	int y;		// Ｙ座標
}POINT2;

// 線分
typedef struct _OPTICAL {
	byte a;		// 元の座標A
	byte b;		// 元の座標B
	int x;		// 線分のＸ成分
	int y;		// 線分のＹ成分
	char d;		// 一致判定時の誤差(確定)
}OPTICAL;

// 移動量
typedef struct _DIST {
	char x;		// 移動量H
	char y;		// 移動量V
	char d;		// 一致判定時の誤差候補
	byte b;		// 元の線分(今)
}DIST;

// DMA受信データの分割
typedef struct _DETECT {
	byte n;		// 輝度順位
	byte i;		// 輝度
	int x;		// Ｘ座標
	int y;		// Ｙ座標
}DETECT;

// 停止判定用ヒストグラム
typedef struct _FIELD_A {
	int x;				// Ｘ座標
	int y;				// Ｙ座標
	byte t[S_LVL_MAX];		// 輝度順位
	byte sum;				// 積算情報
	byte psum;			// 積算情報(前回)
	byte mx;				// Ｘマージマーク
	byte my;				// Ｙマージマーク
}FIELD_A;

// 移動検出用ヒストグラム
typedef struct _FIELD_B {
	int x;				// Ｘ座標
	int y;				// Ｙ座標
	byte t[M_LVL_MAX];		// 輝度順位
	byte sum;				// 積算情報
	byte mx;				// Ｘマージマーク
	byte my;				// Ｙマージマーク
}FIELD_B;

#endif //TYPEDEFS_H
