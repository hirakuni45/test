////////////////////////////////////////////////////////////////////////////////////////////////
// Company          	: T.S.Labolatory Coporation
//
// Create Date			: 2013.07.18
// Project Name			: Checker4
// Module Name			: ck4_fpga.c
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
#include "ck4_fpga.h"
#include "typedefs.h"
#include "libpic30.h"

void fpga_init(void)
{
	fpga_abort();
	fpga_0clear();	//Memory clear
	__delay_ms(300);
	while(BUSY!=0){}
	fpga_serial(0,0,0);
	fpga_TWE(0);
	fpga_Control(20,0);

//	fpga_debug();			//FPGA debug
}

void fpga_start(void)
{
	while(BUSY!=0){}
	SLC=Start;
	SLT=0;
    SLT=1;
	Nop();
	SLT=0;
}

void fpga_0clear(void)
{
//	while(BUSY!=0){}
	SLC=memory0set;
	SLT=0;
    SLT=1;
	Nop();
	SLT=0;

}
void fpga_1clear(void)
{
	while(BUSY!=0){}
	SLC=memory1set;
	SLT=0;
    SLT=1;
	Nop();
	SLT=0;
}

void fpga_Control(unsigned int pc,int twe)
{
	unsigned long dat=0;
	while(BUSY!=0){}
	SLC=Control;
	dat=(unsigned long)pc+((unsigned long)twe<<16);
	fpga_datset(dat);
	SLT=0;
    SLT=1;
	Nop();
	SLT=0;
}

void fpga_setadrs(int adr)
{
	unsigned long dat=0;
	while(BUSY!=0){}
	SLC=Set_adrs;
	dat=adr;
	fpga_datset(dat);
	SLT=0;
    SLT=1;
	Nop();
	SLT=0;
	
}

void fpga_setdata(unsigned long d,unsigned long mask)
{
	unsigned long dat=0;

	while(BUSY!=0){}
	SLC=Set_data;
	dat=d|mask;
//	if(end!=0) d+= 0x4000000;
//	if(twe!=0) d+= 0x2000000;
//	if(delay!=0) d+=0x1000000;

	fpga_datset(dat);
	SLT=0;
    SLT=1;
	Nop();
	SLT=0;
	
}

void fpga_TWE(int b)
{
	unsigned long dat=0,di=1;

	while(BUSY!=0){}
	SLC=TWEasign;
	if(b<24)	dat=di<<b;
	else dat=0;
	fpga_datset(dat);
	SLT=0;
    SLT=1;
	Nop();
	SLT=0;

}

void fpga_serial(int mode ,int sk ,int s)
{
	unsigned long dat=0,di=1;

	while(BUSY!=0){}
	SLC=Serialmode;
	if(mode==0) dat=0;
	else
	{
		dat|=0x4000000;
		dat|=di<<s;
		dat|=(unsigned long)sk<<24;
	}
	fpga_datset(dat);
	SLT=0;
    SLT=1;
	Nop();
	SLT=0;
}

void fpga_datset(unsigned long d)
{
	unsigned int d0,d1;

	while(BUSY!=0){}
	d0=(unsigned int)((d<<2)&0xfffc);
	SDT0G=d0;
	d1=(unsigned int)((d>>14)&0x1fff);
	SDT1G=d1;
}

void fpga_abort(void)
{
	ABORT=0;
	ABORT=1;
	Nop();
	Nop();
	ABORT=0;
}

void fpga_debug(void)
{
	int i;
	unsigned long di=1;

	fpga_TWE(0);
	fpga_Control(20,0);
	fpga_setadrs(0);
	fpga_datset(0x000ff005a);
	SLT=0;
    SLT=1;
	Nop();
	SLT=0;
	fpga_datset(0x000000000);
	SLT=0;
    SLT=1;
	Nop();
	SLT=0;
	for (i=0;i<24;i++){
		fpga_datset(di<<i);
		SLT=0;
    	SLT=1;
		Nop();
		SLT=0;
	}
	fpga_datset(0x004000000);
	SLT=0;
    SLT=1;
	Nop();
	SLT=0;
	fpga_start();
	__delay_ms(100);

	fpga_setadrs(2032);
	for (i=0;i<24;i++){
		fpga_datset(di<<i);
		SLT=0;
    	SLT=1;
		Nop();
		SLT=0;
	}

	
}

