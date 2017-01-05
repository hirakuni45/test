////////////////////////////////////////////////////////////////////////////////////////////////
// Company          	: T.S.Labolatory Coporation
//
// Create Date			: 2013.07.18
// Project Name			: Checker4
// Module Name			: ck4_comm.c
// Tool versions		: MPLAB v8.91
//						  MPLAB C XC16
// Description			:
//
// Dependencies			:
//
// Additional Comments	:
//
////////////////////////////////////////////////////////////////////////////////////////////////

#include "common.h"

int device_no=0;
int pre_device_no=0;
char device_name[17];
char ID_name[17];

char PRAM_name[17]="PRAM            ";

typedef struct{
	int dev_no;
	char com_name[16][5];
	unsigned long eeprom_adr;
} device_str;

device_str device_info[16];

typedef union{
	struct{
		unsigned int control;
		unsigned long dt;
	};
	byte db[6];
	unsigned long dl;
} data_str;

int outdat_cnt;				// 40bitデータの数
data_str outdat[FPGA_MEMORY];		// 40bitデータ
//unsigned long outdat24[2048];

byte command_num;
byte aux_init;
byte period_init[4];
byte phase_init;

byte pintofpga[40];			//40bit→24bit

byte pin_info[24][2];		//EEPROM pin情報


byte data_width;		//DATA線幅
unsigned long  data_mask;

unsigned long out_init;		//出力初期値 24bit

byte usepin[40];			//使っているか、

byte command_name[16][5];	//コマンド名

byte command_ramexist[16];	//全部持ってこれたか
byte command_define[DEFINE_MAX];//もって来れた
unsigned int command_mem[16];
unsigned long command_eeprom[16];

//typedef struct{
//	unsigned long max;
//	unsigned long min;
//}arg_str;

//arg_str arg_limit[16][16];

unsigned long arg_limit[16][16];

//byte command_extra[10240];

unsigned long clk_period;
unsigned long clk_phase;

byte eeprom_stat;
unsigned long eepadr;
byte eeprom_used;
unsigned long eepsize;
byte define_reading;

int serial_pin[4];

char *com_data[BUFMAX_LENGTH];						// パラメータバッファ

char pin_copy_tbl[24];	// pin copy table

/////////////////////

