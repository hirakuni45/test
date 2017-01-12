////////////////////////////////////////////////////////////////////////////////////////////////
// Company          	: T.S.Labolatory Coporation
//
// Create Date			: 2013.07.18
// Project Name			: Checker4
// Module Name			: ck_comm.c
// Tool versions		: MPLAB v8.60
//						  MPLAB C for PIC18 v3.34 in LITE mode
// Description			:
//
// Dependencies			:
//
// Additional Comments	:
//
////////////////////////////////////////////////////////////////////////////////////////////////


//typedef struct{
//	unsigned int control;
//	byte data[3];
//}data_str;

extern int device_no;
extern int pre_device_no;
extern char device_name[];
extern char ID_name[];

extern char PRAM_name[];

typedef union{
	struct{
		unsigned int control;
		unsigned long dt;
	};
	byte db[5];
	unsigned long dl;
}data_str;

extern int outdat_cnt;				//40bitデータの数
extern data_str outdat[];		//40bitデータ

extern byte command_num;
extern byte aux_init;
extern byte period_init[];
extern byte phase_init;

extern byte pintofpga[];

extern byte pin_info[24][2];

extern byte command_name[16][5];

extern byte command_ramexist[];
extern unsigned long command_eeprom[];

extern byte data_width;		//DATA線幅
extern unsigned data_mask;

extern unsigned long out_init;//出力初期値 24bit

extern byte usepin[];

extern byte command_define[DEFINE_MAX];
extern unsigned int command_mem[];

extern unsigned long arg_limit[16][16];

extern unsigned long clk_period;
extern unsigned long clk_phase;

extern byte eeprom_stat;
extern unsigned long eepadr;
extern byte eeprom_used;
extern unsigned long eepsize;

extern byte define_reading;

extern int serial_pin[];

extern char *com_data[];						// パラメータバッファ

//////////////////////

extern word tgDut;

extern char pin_copy_tbl[];

