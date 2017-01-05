//////////////////////////////////////////////////////////////////////////////////
// Company:		T.S.Labolatory Coporation
// Engineer:		
// 
// Create Date:   	Jul/18/2013
// Project Name: 	Cheker4
// Module Name:   	ck4_i2c.c
// Tool versions: 	MPLAB v8.91
//				Microchip XC16
// Description: 
//
// Dependencies: 
//
// Revision: 		0.0.0.0 - Initial
// Additional Comments: 
//
//
//////////////////////////////////////////////////////////////////////////////////
#include "i2c.h"
#include "common.h"
#include "ck4_i2c.h"
#include "ck4_gvar.h"
#include "ck4_fpga.h"

void PinAnalyze();

#define S_LATC	SLC

unsigned int jDone;

/******************
 * I2C初期化関数
 ******************/
void i2c_init(void)
{
	OpenI2C2(	// I2C2CON
//			I2C1_ON				/*I2C module enabled */
//			& I2C1_IDLE_CON		/*continue I2C module in Idle mode */
//			& I2C1_CLK_REL		/*release clock */
//			& I2C1_IPMI_DIS		/*IPMI mode not enabled */
//			& I2C1_7BIT_ADD		/*I2CADD is 7-bit address */
//			& I2C1_SLW_EN			/*Enable Slew Rate Control for 400KHz */
//	//		& I2C1_SM_EN			/*Enable SM bus specification */
//			& I2C1_SM_DIS			/*Disable SM bus specification */
//			& I2C1_GCALL_DIS		/*Disable General call address. */
//			& I2C1_STR_EN			/*Enable clock stretching */
//			& I2C1_ACK			/*Transmit 0 to send ACK as acknowledge */
//			& I2C1_ACK_EN			/*Initiate Acknowledge sequence */
//			& I2C1_RCV_DIS		/*Receive sequence not in progress */
//			& I2C1_STOP_EN		/*Initiate Stop sequence */
//			& I2C1_RESTART_EN		/*Initiate Restart sequence */
//			& I2C1_START_EN,		/*Initiate Start sequence */

			I2C2_ON
			& I2C2_IDLE_CON
//			& I2C2_CLK_HLD
			& I2C2_CLK_REL
			& I2C2_IPMI_DIS
			& I2C2_7BIT_ADD
//			& I2C2_SLW_DIS		/*Enable Slew Rate Control for 100KHz */
			& I2C2_SLW_EN			/*Enable Slew Rate Control for 400KHz */
			& I2C2_SM_DIS
			& I2C2_GCALL_DIS
			& I2C2_STR_EN
//			& I2C2_STR_DIS
			& I2C2_NACK
			& I2C2_ACK_DIS
			& I2C2_RCV_DIS
			& I2C2_STOP_DIS
			& I2C2_RESTART_DIS
			& I2C2_START_DIS,

			// I2CBRG				//See DS70330B_JP  p14
			98);

	ConfigIntI2C2(SI2C2_INT_PRI_6	/* Priority for Slave I2C1 Interrupt */
				& MI2C2_INT_PRI_6	/* Priority for Master I2C1 Interrupt */
				& SI2C2_INT_OFF	/* Slave I2C1 Interrupt Enable */
				& MI2C2_INT_OFF);	/* Master I2C1 Interrupt Disable */
//				& MI2C2_INT_ON);	/* Master I2C1 Interrupt Disable */
}

//------------------------------------------------------------------------------------------------------------
/******************
 * EEPROMへの書き込み
 ******************/
void write_eeprom(unsigned long eAddress, byte eData)
{
	byte sAddr;
	byte A16;


	if((eAddress & 0x10000)!=0) A16=2;
	else A16=0;

	// Write Polling
	do{
		I2C2CONbits.I2CEN = 0;		// I2C1 InActive
		Nop();
		I2C2CONbits.I2CEN = 1;		// I2C1 Active

		IdleI2C2();				// アイドル状態まで待機

		StartI2C2();				// generate start condition
		IdleI2C2();				// アイドル状態まで待機

		// slave adress & RW = 0
		sAddr = IIC_EEPROM_SLAVE_ADDR | IIC_DATA_WRITE|A16;

	}while(MasterWriteI2C2(sAddr)<0);

	IdleI2C2();	// アイドル状態まで待機


	// EEPROMアドレスの送信
	MasterWriteI2C2(((eAddress)>>8)&0xff);
	MasterWriteI2C2(eAddress&0xff);


	// データの書き込み
	MasterWriteI2C2(eData);


	// stop condition
	StopI2C2();

	IdleI2C2();	// アイドル状態まで待機

}


/******************
 * EEPROMからの読み出し
 ******************/
byte read_eeprom(unsigned long eAddress)
{
	byte sAddr;
	byte eData;
	char ret;
	byte A16;

	IdleI2C2();	// アイドル状態まで待機

	// generate start condition
	StartI2C2();

	IdleI2C2();	// アイドル状態まで待機

	if((eAddress & 0x10000)!=0) A16=2;
	else A16=0;

	// slave adress & RW = 0
	sAddr = IIC_EEPROM_SLAVE_ADDR | IIC_DATA_WRITE|A16;
	ret = MasterWriteI2C2(sAddr);



	IdleI2C2();	// アイドル状態まで待機

	// EEPROMアドレスの送信
	ret = MasterWriteI2C2((eAddress>>8)&0xff);
	ret = MasterWriteI2C2(eAddress&0xff);

	IdleI2C2();	// アイドル状態まで待機

	// restart condition
	RestartI2C2();

	IdleI2C2();	// アイドル状態まで待機

	// slave adress & RW = 1
	sAddr = IIC_EEPROM_SLAVE_ADDR | IIC_DATA_READ|A16;
	MasterWriteI2C2(sAddr);

	IdleI2C2();	// アイドル状態まで待機

	eData = MasterReadI2C2();	// EEPROMのデータを読み出し

	IdleI2C2();	// アイドル状態まで待機

	NotAckI2C2();			//通信終了のNackを送信

	IdleI2C2();	// アイドル状態まで待機

	// stop condition
	StopI2C2();

	IdleI2C2();	// アイドル状態まで待機

	return eData;
}
//------------------------------------------------------------------------------------------------------------

/******************
 * MASTER割り込み
 ******************/
void __attribute__((interrupt, no_auto_psv)) _MI2C1Interrupt(void)
{
	jDone=1;
	_MI2C2IF = 0;		//Clear Interrupt Flag;
}

/******************
 * SLAVE割り込み
 ******************/
void __attribute__((interrupt, no_auto_psv)) _SI2C1Interrupt(void)
{
	byte temp;

	//割り込みフラグクリア
	_SI2C2IF = 0;

	// アドレス受信
	if(_R_W==0 && _D_A==0){

		temp = SlaveReadI2C2();	// ダミーリード
		IdleI2C2();

		_SCLREL = 1;	// クロック動作を再開

	// データ受信
	}else if(_R_W==0 && _D_A==1){

//		i2c_buff = SlaveReadI2C2();

		_SCLREL = 1;	// クロック動作を再開

	// データ送信要求
	}else if(_R_W==1 && _D_A==0){

		IdleI2C2();
		SlaveWriteI2C2('0');
		IdleI2C2();
		
	}

}

//eepromからデバイス情報読み出し
//input:device_no

int get_device_info(void){

//	unsigned long eepadr=0;
	byte h,hn,cn,cm;
	int i,j,k;
	byte get=0;
	byte cnt,buf;
	unsigned long stadr;
//	byte b[4];
//	byte argnum;
//	int outcnt;	
	unsigned int period_i;

	unsigned int memadr=0;

	byte mem_lost=0;

	byte cmsize[11]={2,5,3,5,7,7,3,5,5,5,0};

	define_reading=1;			//読み込み中

	for (i=0;i<16;i++){
		ID_name[i]=read_eeprom(i);
		if(ID_name[i] == -1){
			ID_name[i]=NULL;
			break;
		}
	}
	ID_name[17]=NULL;

	if(device_no==99){		//serial
		fpga_serial(1,0,serial_pin[0]);
		fpga_serial(1,1,serial_pin[1]);
		fpga_serial(1,2,serial_pin[2]);
		fpga_serial(1,3,serial_pin[3]);		
		S_LATC = 0x04000;
		define_reading=0;
		return 0;
	}

	fpga_serial(0,0,0);

	eepadr=0;

	eepadr+=16;
	h=read_eeprom(eepadr);eepadr++;
	if(h>64){
		mem_lost=1;
		goto end_p;
	}
	
	for(i=0;i<h;i++){
		for(j=0;j<16;j++){
			device_name[j]=read_eeprom(eepadr+j);
		}
		device_name[16]=NULL;	

		eepadr+=16;
		hn=read_eeprom(eepadr);	eepadr++;		//品種番号

		if (hn==device_no){		//データ作成
			cn=read_eeprom(eepadr);
			if(cn>16){
				mem_lost=1;
				goto end_p;
			}
			command_num=cn;
			eepadr++;
		//pin_info
			for (j=0;j<24;j++){
				pin_info[j][0]=read_eeprom(eepadr);eepadr++;
				pin_info[j][1]=read_eeprom(eepadr);eepadr++;
			}
			aux_init=read_eeprom(eepadr);eepadr++;
			period_init[0]=read_eeprom(eepadr);eepadr++;
			period_init[1]=read_eeprom(eepadr);eepadr++;
			period_init[2]=read_eeprom(eepadr);eepadr++;
			phase_init=read_eeprom(eepadr);eepadr++;

			PinAnalyze();

			// pin copy info (24 bytes)
			for(j = 0; j < 24; ++j) {
				pin_copy_tbl[j] = read_eeprom(eepadr);
				eepadr++;
			}

			for(j=0;j<cn;j++){
				command_name[j][0]=read_eeprom(eepadr);eepadr++;
				command_name[j][1]=read_eeprom(eepadr);eepadr++;
				if (command_name[j][1]==' ') command_name[j][1]=NULL;
				command_name[j][2]=read_eeprom(eepadr);eepadr++;
				if (command_name[j][2]==' ') command_name[j][2]=NULL;
				command_name[j][3]=read_eeprom(eepadr);eepadr++;
				if (command_name[j][3]==' ') command_name[j][3]=NULL;
				command_name[j][4]=NULL;
//				for(k=0;k<16;k++){
//					b[0]=read_eeprom(eepadr);eepadr++;
//					b[1]=read_eeprom(eepadr);eepadr++;
//					b[2]=read_eeprom(eepadr);eepadr++;
//					b[3]=read_eeprom(eepadr);eepadr++;
//					arg_limit[j][k]=(unsigned long)b[0]+((unsigned long)b[1]<<8)+((unsigned long)b[2]<<16)+((unsigned long)b[3]<<24);					
//				}
				command_eeprom[j]=eepadr;
				command_mem[j]=memadr;
				cm=0;
				buf=0;
				stadr=eepadr;
				while (cm!=0x3f){
					cm=(read_eeprom(eepadr))&0x3f;

					if (cm==0x1f) cnt=4;
					else if (cm==0x3f) cnt=2;
					else {
						cnt=cmsize[cm];
					}
//					if ((long)((long)eepadr-(long)stadr-(long)cnt)<0){	//読みきれない
					if(memadr+cnt>DEFINE_MAX){
						buf=1;
					}
					for(k=0;k<cnt;k++){
						if(buf==0)
							command_define[memadr]=read_eeprom(eepadr);eepadr++;memadr++;
					}
					if(eepadr>eepsize){
						mem_lost=1;
						goto end_p;
					}
				}
				if(buf==0)command_ramexist[j]=1;
				else command_ramexist[j]=0;

			}
			

			get=1;goto loopout;
		}
		else{					//読み飛ばし
			cn=read_eeprom(eepadr);
			if(cn>16){
				mem_lost=1;
				goto end_p;
			}
			eepadr += 54;
			eepadr += 16;  // pin copy info (24 bytes)
			for(j=0;j<cn;j++){
				eepadr+=4;
				cm=0;
				while (cm!=0x3f){
					cm=(read_eeprom(eepadr))&0x3f;
					if (cm==0x1f) eepadr+=4;
					else if (cm==0x3f) eepadr+=2;
					else eepadr+=cmsize[cm];
					if(eepadr>eepsize){
						mem_lost=1;
						goto end_p;
					}
				}
			}
		}
	}
	end_p:
	if (mem_lost!=0){
		init_device_info();
		define_reading=0;
		return 2;
	}
	if (get==0){				//無い
		init_device_info();
		define_reading=0;
		return 1;
	}
loopout:
	//FPGA 設定
	fpga_TWE(pintofpga[0]);
	period_i=(unsigned int)period_init[0]+((unsigned int)period_init[1]<<8);
	clk_period=period_i;
	clk_phase=(unsigned int)phase_init;
	fpga_Control(period_i,(int)phase_init);

	fpga_setadrs(0);				//品種決定時 初期値送出
	fpga_setdata(out_init,0);	

	define_reading=0;
	return 0;
}

void init_device_info(void){
	int i;

//	device_no=0;
	device_name[0]=NULL;
	command_num=0;
	for(i=0;i<16;i++){
		command_name[i][0]=NULL;
	}

	//品種無い場合の初期値送出
	fpga_TWE(30);		//クロック無し(24以上)
	fpga_setadrs(0);
	fpga_setdata(0,0);	

}
// ck4_i2c.c END
