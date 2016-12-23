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


// zñ|C^`
typedef struct _POINT08A {
	int sta;		// X^[g|C^
	int cnt;		// vf
}POINT08A;

// zñ|C^a
typedef struct _POINT08B {
	int sta;		// X^[g|C^
	int cnt;		// vf
	byte sum;		// ÏZvf
	byte n;		// }ÅÌêvñ
}POINT08B;

// ÀWf[^
typedef struct _POINT {
	int x;		// wÀW
	int y;		// xÀW
}POINT;

// ÊÚ®Ê
typedef struct _POINT2 {
	int n;		// o»px or vf
	int x;		// wÀW
	int y;		// xÀW
}POINT2;

// üª
typedef struct _OPTICAL {
	byte a;		// ³ÌÀWA
	byte b;		// ³ÌÀWB
	int x;		// üªÌw¬ª
	int y;		// üªÌx¬ª
	char d;		// êv»èÌë·(mè)
}OPTICAL;

// Ú®Ê
typedef struct _DIST {
	char x;		// Ú®ÊH
	char y;		// Ú®ÊV
	char d;		// êv»èÌë·óâ
	byte b;		// ³Ìüª(¡)
}DIST;

// DMAóMf[^Ìª
typedef struct _DETECT {
	byte n;		// PxÊ
	byte i;		// Px
	int x;		// wÀW
	int y;		// xÀW
}DETECT;

// â~»èpqXgO
typedef struct _FIELD_A {
	int x;				// wÀW
	int y;				// xÀW
	byte t[S_LVL_MAX];		// PxÊ
	byte sum;				// ÏZîñ
	byte psum;			// ÏZîñ(Oñ)
	byte mx;				// w}[W}[N
	byte my;				// x}[W}[N
}FIELD_A;

// Ú®opqXgO
typedef struct _FIELD_B {
	int x;				// wÀW
	int y;				// xÀW
	byte t[M_LVL_MAX];		// PxÊ
	byte sum;				// ÏZîñ
	byte mx;				// w}[W}[N
	byte my;				// x}[W}[N
}FIELD_B;

#endif //TYPEDEFS_H
