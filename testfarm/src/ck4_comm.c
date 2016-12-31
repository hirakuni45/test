////////////////////////////////////////////////////////////////////////////////////////////////
// Company          	: T.S.Labolatory Coporation
//
// Create Date			: 2013.07.18
// Project Name			: Checker4
// Module Name			: PLP300_comm.c
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
#include "ck4_gvar.h"
#include "ck4_i2c.h"
#include "ck4_timer.h"
#include "ck4_uart.h"
#include "ck4_fpga.h"
#include "libpic30.h"

void send_comma(void);
byte exec_command(char *);

char Make40bit(int);
void fpga_writemem();
void LtoA(unsigned long dat,char *buf);
unsigned long Ascii2Ulong(char *);

void eep_writeproc(void);
void eep_readproc(void);

int serial_proc(void);

//void usart_put_eeprom(unsigned int);
//void usart_put_eeprom2(unsigned int);
//void usart_put_eeprom12(unsigned int);
//void usart_put_itoa(int);
//float put_eeprom_atof(unsigned int,char *);

typedef struct
{
	char Command[6];
	byte num;
}ComTblST;

const ComTblST CommandTable[]={
{"DEVWR",1},			//0
{"DEVRD",0},
{"CLKWR",2},
{"CLKRD",0},
{"EEPWR",0},
{"EEPDT",1},
{"EEPED",0},
{"EEPRD",0},
{"EEPGT",0},
{"EEPIT",0},
{"FPABT",0},
{"SERST",2},
{"SERRD",0},
{"IDGET",0},
{"FPGAT",2},			//14		
{"",0}
};

const char serial_str[] = {"UDT_SERIAL      "};

const char copy_tbl_[24] = {
	32, 32, 32,
	 0,  1,  2,
	 0,  1,  2,
	 0,  1,  2,
	 0,  1,  2,
	 0,  1,  2,
	 0,  1,  2,
	 0,  1,  2
};

// USARTコマンドハンドラ
void usart_command_handler(void)
{
	static char	command[BUFMAX_COMMAND+1];		// コマンドバッファ
	static unsigned int	idx_command = 0;		// コマンドバッファの書込みポインタ
	static int	packet_receiving = 0;			// パケット受信中フラグ
	char c; 										// 文字格納用
//	int pp;	

	// USARTリングバッファから文字入力
	while ((c = getc_from_usart_buf()) >= 0) {
		UserTimerStart(USERTIMER0, 2000);			// 2sタイマスタート
		packet_receiving = 1;						// パケット受信中
		// 改行コード検出
		if (c == '\n' || c == '\r' || c == '\0') {
			packet_receiving = 0;					// パケット受信終了
			command[idx_command] = '\0';
			UserTimerStop(USERTIMER0);
			if (idx_command == 0) {
				// 空行はなにもしない
				continue;
			} else {
				// コマンド実行
				exec_command(command);
				idx_command = 0;
			}
		} else {
			// 改行コード以外はバッファに格納
			if ((unsigned char)idx_command >= BUFMAX_COMMAND) {
				idx_command = 0;
			} else {
				command[idx_command++] = c;
			}
		}
	}
			
	// コマンド受信タイムアウトチェック
	if (packet_receiving && !UserTimerStatus(USERTIMER0)) {
		packet_receiving = 0;						// パケット受信終了
		usart_puts("ERR: USART timeout\r\n");
		idx_command = 0;
		command[idx_command] = '\0';
		U1STAbits.OERR=0;
	}
}

// データのコピー
unsigned long copy_bits(unsigned long d)
{
	byte i;
	for(i = 0; i < 24; ++i) {
		char src = copy_tbl_[i];
		if(src < 0 || src >= 24) continue;  // 範囲外は[NC]
		if((d & (1L << src)) != 0) {
			d |=   1L << i;
		} else {
			d &= ~(1L << i);
		}
	}
	return d;
}

// USARTコマンド実行
//void exec_command(uchar *com)
byte exec_command(char *com)
{
	int idx = 0;									// パラメータカウンター
	int i;
	unsigned int tmpi1,tmpi2;
//	short tmps;
	char  seps[] = ",";								// 分離文字
	char *string;									// コマンド文字列
	char *cmd;										// コマンド
	char cmp,error1,error2,e3;
	byte cmd_no1;
	char cmd_no2;
	long arg;
	char tmpc[10];
	byte tmpb;
	int ret;
	unsigned long eepa;
	byte busy_flg; 

	// ","で文字SPLIT
	string = strtok((char *)com, seps);
	cmd = string;

	while( string != NULL ){
		string = strtok( NULL, seps );
		//usart_put(string );
		com_data[idx] = string;
		idx++;
	}

	busy_flg=1;
	for(i=0;i<1000;i++){
		if (BUSY==0) {
			busy_flg=0;
			break;
		}
	}


	//コマンド検索(固定)
	cmd_no1 = 0;
	error1 = 0;
	error2 = 0;
	while(1){
		cmp=0;
		if(CommandTable[cmd_no1].Command[0]==NULL)
		{
		 	error1=1;cmd_no1=0xff;
			break;
		}
		for(i=0;i<5;i++)
		{
			if (CommandTable[cmd_no1].Command[i]!=cmd[i]) cmp=1;
		}
		if(cmp==0)break;
		++cmd_no1;
	}

	//error1=1 固定コマンドではない

	if(error1!=0){
		// Serialモード?
		if(device_no==UDT_SERIAL){
			if((cmd[0]=='C')&&(cmd[1]=='A')) {
				ret=serial_proc();
				if(ret!=0) {
					error2=2;
				}
//				usart_puts("OK");
//				goto normal_end;
				return TRUE;
			}
			else{
				error2=3;
				goto cmd_trns;
			}		
		}

		//PRAM CO,0 ?
		if(strcmp(device_name,PRAM_name)==0){
			if((cmd[0]=='C')&&(cmd[1]=='O')&&(*com_data[0]=='0')){
				cmd[2]='0';
				cmd[3]=NULL;
			}
		}

		// コマンド数をカウント
		cmd_no2 = 0;
		error2 = 1;
		for(i=0; i < command_num; i++) {
			if(strcmp(cmd, (char *)command_name[i]) == 0) {
				error2 = 0;
				break;
			}
			++cmd_no2;	
		}
		if(error2 == 0) {  //引数チェックしない。maxminでチェック。
		}
	}

cmd_trns:
	// Command転送 しない
//	usart_put(cmd); 
//	usart_puts(",");
	if(error2!=0)goto error_proc;

	if(error1==0){

		if(CommandTable[cmd_no1].num!=0){
			for(i=0;i<CommandTable[cmd_no1].num;i++){
				if (com_data[i]==NULL) goto error_proc;
			}
		}

		switch(cmd_no1)			//固定コマンド
		{
			case 0:// デバイス番号write
				if(eeprom_used==0)goto error_proc;   
				arg=atol(com_data[0]);
				if (arg>99) goto error_proc;
				device_no=arg;
				if(device_no==UDT_SERIAL){
					strcpy(device_name,serial_str);
					fpga_serial(1,0,serial_pin[0]);
					fpga_serial(1,1,serial_pin[1]);
					fpga_serial(1,2,serial_pin[2]);
					fpga_serial(1,3,serial_pin[3]);		
					usart_puts("DONE");
					break;
				}
				//	情報読み取り
				if(get_device_info()==0)	usart_puts("DONE");
				else goto error_proc;
				break;
			case 1:	// デバイス番号READ
				usart_put(cmd); 
				usart_puts(",");
				LtoA(device_no,tmpc);
				usart_put(tmpc);
				break;
			case 2:// クロックセット
//				strcpy(com_data[0],period_str);
				if(busy_flg!=0)goto busy_proc;
				tmpi1=atoi(com_data[0]);
				tmpi2=atoi(com_data[1]);
///				if(tmpi1 > 65535) goto error_proc;
				if(tmpi2>3)goto error_proc;
				clk_period=tmpi1;
				clk_phase=tmpi2;
				fpga_Control(tmpi1,tmpi2);
				usart_puts("DONE");
				break;
			case 3:// クロックリード
				usart_put(cmd); 
				usart_puts(",");
				LtoA(clk_period,tmpc);
				usart_put(tmpc);
				usart_puts(",");
				LtoA(clk_phase,tmpc);
				usart_put(tmpc);
				break;
			case 4:// EEPROM書き込み
				eepadr=0;
				eeprom_stat=1;
				usart_puts("DONE");
				break;
			case 5:// EEPROM書き込みデータ
				eep_writeproc();
				usart_puts("DONE");
				break;
			case 6:// EEPROM書き込み終了
				tmpb=eepadr&0xff;
				write_eeprom(EEP_USED,tmpb);
				tmpb=(eepadr>>8)&0xff;
				write_eeprom(EEP_USED+1,tmpb);
				tmpb=(eepadr>>16)&0xff;
				write_eeprom(EEP_USED+2,tmpb);
				tmpb=(eepadr>>24)&0xff;			//0
				write_eeprom(EEP_USED+3,tmpb);

				eepsize=eepadr;
				eeprom_stat=0;

				__delay_ms(500);
				get_device_info();

				usart_puts("DONE");
				break;
			case 7:// EEPROM読み込み
				eepadr=0;
				eeprom_stat=2;
				usart_puts("DONE");
//				usart_puts("\r\n");
				break;
			case 8:// EEPROM読み込みデータ
				usart_puts("EEPGT,");
				eep_readproc();
				break;
			case 9:// EEPROM初期化
				for(eepa=0;eepa<0x20000;eepa++){
					write_eeprom(eepa,0xff);
				__delay_ms(5);
				}
				__delay_ms(500);
				usart_puts("DONE");
				break;
			case 10:// FPGA ABORT
				fpga_abort();
				usart_puts("DONE");
				break;
			case 11:// シリアルpin SET
				tmpi1=atoi(com_data[0])-1;		//引数は1オリジン
				tmpi2=atoi(com_data[1])-1;
				if((tmpi1>=4)||(tmpi1<0))goto error_proc;
				if((tmpi2>=24)||(tmpi2<0))goto error_proc;
				serial_pin[tmpi1]=tmpi2;						
				fpga_serial(1,tmpi1,tmpi2);
				write_eeprom((unsigned long)(EEP_SERIAL+tmpi1),(byte)tmpi2);
				usart_puts("DONE");
				break;
			case 12:// シリアルpin READ
				usart_put(cmd); 
				usart_puts(",");
				LtoA(serial_pin[0]+1,tmpc);
				usart_put(tmpc);
				usart_puts(",");
				LtoA(serial_pin[1]+1,tmpc);
				usart_put(tmpc);
				usart_puts(",");
				LtoA(serial_pin[2]+1,tmpc);
				usart_put(tmpc);
				usart_puts(",");
				LtoA(serial_pin[3]+1,tmpc);
				usart_put(tmpc);
				break;
			case 13:// ID READ
				usart_put(cmd); 
				usart_puts(",");
				usart_puts(ID_name);
				break;
			case 14:// FPGA TEST
				arg=atol(com_data[0]);
				arg=arg&(0xf);
				SLC=arg<<1;
				arg=atol(com_data[1]);
				fpga_datset(arg);
				SLT=0;
			    SLT=1;
				Nop();
				Nop();
				SLT=0;
				usart_puts("DONE");
				break;
			default:
				break;
		}
	}
	else{						//設定コマンド
		if(busy_flg!=0) goto busy_proc;
		e3 = Make40bit(cmd_no2);
		if (e3 != 0) goto error_proc;
		fpga_TWE(pintofpga[0]);
		fpga_Control(clk_period,clk_phase);
		fpga_writemem();
		fpga_start();	
		usart_puts("DONE");
	}


//	while(BUSY!=0){}			//終わるまで待つ
	usart_puts("\r\n");
	return TRUE;

error_proc:
	usart_puts("NG\r\n");
	return TRUE;	
busy_proc:
	usart_puts("NG\r\n");
	return TRUE;	
}


//事前解析
//input:pin_info,aux_init
//output:out_init,data_width,pintofpga
void PinAnalyze()
{
	int i;
	byte p1,p2;
	int datacnt=0;
	int	auxno; 
	unsigned long init=0;
	unsigned long b=1; 
	
	//usepin,pintofpga 作成
	for (i=0;i<40;i++){
		pintofpga[i]=0;
		usepin[i]=0;
	}

	for(i=0;i<24;i++){
		p1 = pin_info[i][0];
		p2 = pin_info[i][1];

		if((p1&0x80)!=0){	//使われている
			if((p1&0x40)!=0){
				usepin[0]=0x80;//TWE
				pintofpga[0]=i;
			}	
			else if((p1&0x20)==0){//制御線
				usepin[p2+1]=0x80+(p1&0x0f);
				pintofpga[p2+1]=i;
				init += ((p1 & 0x10) >> 4) << i;
			}
			else {				//DATA線 or AUX
				if((p1&0x1f)>23){  //AUX線
					usepin[(p1&0x1f)-16]=0x80;
					pintofpga[(p1&0x1f)-16]=i;
					auxno = (p1 & 0x1f) - 24;
					init += (((unsigned long)aux_init>>auxno)&0x01L)<<i;
				}
				else{
					usepin[(p1&0x1f)+16]=0x80;
					pintofpga[(p1&0x1f)+16]=i;
					datacnt++;
				}
			}
		
		}
	}

	out_init = init;

	data_width = datacnt;
	for (i=0;i<datacnt;i++){
		data_mask |= b << i;
	}
}

//40bitデータ作成
//input:comnum,command_define,com_data
//output:outdat,outdat_cnt;
char Make40bit(int comnum)
{
	int i,j,k,k1,cnt,tds;
	unsigned int klng;
	int comadr=0;
	int datadr=0,srcadr,sadr;
	byte b[7];
//	byte p1,p2;
	int property;
	byte outnum;
	byte argnum;
//	byte base;
	unsigned long argument;
	unsigned long mm,kk,nn;
//	byte xx,yy;
	unsigned long bb=1;

	unsigned int proccnt = 0;		//繰り返しのために、カウントしておく
	unsigned long repeat;

//	char arg[20];
	char tds_chk; 

	byte endflg=0;

	unsigned int ctrl;
	unsigned long dat;

	unsigned int memadr;

	memadr = command_mem[comnum];  // Commadn HEAD

	for(i=0;i<DEFINE_MAX;i++){
		ctrl=0;
		dat=0;
		
		b[0] = command_define[memadr + comadr];
		comadr++;
		b[1] = command_define[memadr + comadr];
		comadr++;
		for(j = 0; j < 8; j++){  //制御線
			if((usepin[j] & 0x80)!=0){		//使用
				property = (~(b[0] >> 6)) & 0x3;
				ctrl |= ((usepin[j] >> property) & 0x1) << j;
			}
		}
		ctrl |= ((unsigned int)b[1]) << 8;	//AUX

		// DATA
		if((b[0] & 0x3f) != 0x1f) proccnt = 0;
		switch((b[0] & 0x3f)) {
			case 0:	
				break;
			case 1:// 定数
				b[2] = command_define[memadr + comadr];
				comadr++;
				b[3] = command_define[memadr + comadr];
				comadr++;
				b[4] = command_define[memadr + comadr];
				comadr++;
				dat = (unsigned long)b[2] + ((unsigned long)b[3] << 8) + ((unsigned long)b[4] << 16);
				outdat[datadr].control = ctrl;
				outdat[datadr].dt = dat;
				datadr++;
				proccnt++;
				break;
			case 2: // 引数
				b[2] = command_define[memadr + comadr];
				comadr++;
				outnum = b[2] >> 5;
				argnum = b[2] & 0xf;
				if ((b[2] & 0x10) != 0){
					argument = Ascii2Ulong(com_data[argnum]);
				}
				else {
					argument = atol(com_data[argnum]);
				}
				dat = argument;
				for (k = 0; k < outnum + 1; k++) {
					outdat[datadr].control = ctrl;
					outdat[datadr].dt = dat & data_mask;
					datadr++;
					proccnt++;
					dat = dat >> data_width;					
				}
				if(dat != 0) goto error_p;	//残っているのは変
				break;
			case 3: // 除算bit
				b[2] = command_define[memadr + comadr];
				comadr++;
				b[3] = command_define[memadr + comadr];
				comadr++;
				b[4] = command_define[memadr + comadr];
				comadr++;
				outnum = b[2] >> 5;
				argnum = b[2] & 0xf;
				if ((b[2]&0x10)!=0){
					argument=Ascii2Ulong(com_data[argnum]);
				}
				else {
					argument=atol(com_data[argnum]);
				}
				if(argument==0)goto error_p;	//1 orijinなので0は変
				mm=(unsigned long)b[3]+((unsigned long)b[4]<<8);
				dat=(argument-1)/(mm+1)+1;
				dat=bb<<(dat-1);
				if(dat==0)goto error_p;		//bitが無いのは変

				for (k = 0; k < outnum + 1; k++){
					outdat[datadr].control=ctrl;
					outdat[datadr].dt = (dat & data_mask);
					datadr++;
					proccnt++;
					dat=dat>>data_width;					
				}
				if(dat!=0) goto error_p;	//残っているのは変
				break;
			case 4://除加算
				b[2]=command_define[memadr+comadr];comadr++;
				b[3]=command_define[memadr+comadr];comadr++;
				b[4]=command_define[memadr+comadr];comadr++;
				b[5]=command_define[memadr+comadr];comadr++;
				b[6]=command_define[memadr+comadr];comadr++;
				outnum=b[2]>>5;
				argnum=b[2]&0xf;
				if ((b[2]&0x10)!=0){
					argument=Ascii2Ulong(com_data[argnum]);
				}
				else {
					argument=atol(com_data[argnum]);
				}
				if(argument==0)goto error_p;	//1 orijinなので0は変
				mm=(unsigned long)b[3]+((unsigned long)b[4]<<8);
				kk=(unsigned long)b[5]+((unsigned long)b[6]<<8);
				dat=((argument-1)/(mm+1))+kk;
				for (k=0;k<outnum+1;k++){
					outdat[datadr].control=ctrl;
					outdat[datadr].dt = (dat & data_mask);
					datadr++;
					proccnt++;
					dat=dat>>data_width;					
				}
				if(dat!=0) goto error_p;	//残っているのは変
				break;
			case 5://TDS
				b[2]=command_define[memadr+comadr];comadr++;
				b[3]=command_define[memadr+comadr];comadr++;
				b[4]=command_define[memadr+comadr];comadr++;
				b[5]=command_define[memadr+comadr];comadr++;
				b[6]=command_define[memadr+comadr];comadr++;
				outnum=b[2]>>5;
				argnum=b[2]&0xf;
				if ((b[2]&0x10)!=0) {
					argument=Ascii2Ulong(com_data[argnum]);
				}
				else {
					argument=atol(com_data[argnum]);
				}
				if(argument==0)goto error_p;	//1 orijinなので0は変
				mm=(unsigned long)b[3]+((unsigned long)b[4]<<8);
				nn=(unsigned long)b[5]+((unsigned long)b[6]<<8);
				tds=(argument-1)%(mm+1);
				tds_chk=0;
				for (k=0,cnt=0;k<nn+1;k++){
					dat=0;
					if( ((tds-cnt)<data_width)&&(tds-cnt)>=0){
						dat = bb<<(tds-cnt);tds_chk=1;
					}
					outdat[datadr].control=ctrl;
					outdat[datadr].dt= (dat & data_mask);
					datadr++;
					proccnt++;
					cnt+=data_width;					
				}
				if(tds_chk==0) goto error_p;	//残っているのは変
				break;
			case 6://Chip
				b[2]=command_define[memadr+comadr];comadr++;
				outnum=b[2]>>5;
				argnum=b[2]&0xf;
				if ((b[2]&0x10)!=0){
					argument=Ascii2Ulong(com_data[argnum]);
				}
				else {
					argument=atol(com_data[argnum]);
				}
				dat=argument;
				for (k=0;k<outnum;k++){
					dat = dat >> data_width;
				}
				outdat[datadr].control=ctrl;
				outdat[datadr].dt = (dat & data_mask);
				datadr++;proccnt++;
				break;
			case 7://指定bit
				b[2]=command_define[memadr+comadr];comadr++;
				b[3]=command_define[memadr+comadr];comadr++;
				b[4]=command_define[memadr+comadr];comadr++;
				outnum=b[2]>>5;
				argnum=b[2]&0xf;
				if ((b[2]&0x10)!=0){
					argument=Ascii2Ulong(com_data[argnum]);
				}
				else {
					argument=atol(com_data[argnum]);
				}
				if ((b[4]>31)||(b[3]>31)){
					goto error_p;	
				}
				dat = argument;
				dat = dat << (31 - b[4]);
				dat = dat >> (31 - b[4] + b[3]);
				for (k=0;k<outnum+1;k++){
					outdat[datadr].control=ctrl;
					outdat[datadr].dt = (dat & data_mask);
					datadr++;proccnt++;
					dat=dat>>data_width;					
				}

				break;
			case 8://HW
				b[2]=command_define[memadr+comadr];comadr++;
				b[3]=command_define[memadr+comadr];comadr++;
				b[4]=command_define[memadr+comadr];comadr++;
				dat=(unsigned long)b[2]+((unsigned long)b[3]<<8)+((unsigned long)b[4]<<16);
				outdat[datadr].control=0;
				outdat[datadr].dt=dat+0x80000000;datadr++;proccnt++;				
				break;
			case 9://delay
				b[2]=command_define[memadr+comadr];comadr++;
				b[3]=command_define[memadr+comadr];comadr++;
				b[4]=command_define[memadr+comadr];comadr++;
				dat=(unsigned long)b[2]+((unsigned long)b[3]<<8)+((unsigned long)b[4]<<16);
				outdat[datadr].control=0;
				outdat[datadr].dt=dat+0x40000000;datadr++;proccnt++;				
				break;
			case 0x1f:				//繰り返し proccnt分コピー
				b[2]=command_define[memadr+comadr];comadr++;
				b[3]=command_define[memadr+comadr];comadr++;
				repeat=(unsigned long)b[2]+((unsigned long)b[3]<<8);
				srcadr=datadr-proccnt;
				for(klng=0;klng<repeat;klng++){
					sadr=srcadr;
					for(k1=0;k1<proccnt;k1++){
						outdat[datadr].control=outdat[sadr].control;
						outdat[datadr].dt=outdat[sadr].dt;datadr++;sadr++;
					}
				}
				break;
			case 0x3f:
				endflg=1;
				break;
			default:
				break;
		}
		if (endflg!=0) break;
	}
	
	outdat_cnt=datadr;
	return 0;

error_p:
	return 1;
}

//40bit→fpga
//input:outdat,outdat_cnt
//output:FPGA 
void fpga_writemem() {
	unsigned long d1=1;
	unsigned long mask=0;
	int i, j, k;

	fpga_setadrs(0);
	//初期値セット
	fpga_setdata(copy_bits(out_init), mask);
	//データセット
	for(i = 0; i < outdat_cnt; i++)
	{
		unsigned long data = 0;
		int pin;
		if((outdat[i].dt & 0xC0000000)==0){
			data = 0;
			pin = 0;
			mask = 0;
			for(j = 0; j < 5; j++) {
				for(k = 0; k < 8; k++) {
					if((usepin[pin] & 0x80) != 0) {
						data |= (d1 & (outdat[i].db[j] >> k)) << pintofpga[pin];  //bit操作
					}
					pin++;
				}
			}
			data = copy_bits(data);
		}
		else if((outdat[i].dt & 0x80000000) != 0) { //HW
			mask = 0;
			data = outdat[i].dt & 0xffffff;
		}
		else if((outdat[i].dt & 0x40000000) != 0){ //DELAY
			mask = 0x1000000;
			data = outdat[i].dt & 0xffffff;
		}
		fpga_setdata(data, mask);

	}
	//終了セット
	fpga_setdata(0,0x4000000);

}

//2byte HEXを binary byteに変換
byte HEXtoBIN(char *buf){
	byte result;
	byte b0,b1;

	b0=*buf;
	b1=*(buf+1);
	if(b0>='A') b0-=55;
	else b0-=48;

	if(b1>='A') b1-=55;
	else b1-=48;

	result=(b0<<4)+b1;

	return result;
}

//binary byteを 2byte HEXに変換
void BINtoHEX(byte dat,char *buf){
	byte b0,b1;

	b0=(dat>>4);
	b1=dat&0xf;

	if (b0>=10) *buf=(char)(b0+55);
	else *buf=(char)(b0+48);

	if (b1>=10) *(buf+1)=(char)(b1+55);
	else *(buf+1)=(char)(b1+48);

}

//long をascii MAX99999
void LtoA(unsigned long dat,char *buf){
	unsigned long a,s;
	if(dat>99999) dat=99999;	//MAX

	if(dat>9999){
		s=dat/10000;
		a=dat%10000;
		*(buf)=(char)(s+'0');
		s=a/1000;
		a=a%1000;
		*(buf+1)=(char)(s+'0');
		s=a/100;
		a=a%100;
		*(buf+2)=(char)(s+'0');
		s=a/10;
		a=a%10;
		*(buf+3)=(char)(s+'0');
		*(buf+4)=(char)(a+'0');
		*(buf+5)=NULL;
	}
	else if (dat>999){
		s=dat/1000;
		a=dat%1000;
		*(buf)=(char)(s+'0');
		s=a/100;
		a=a%100;
		*(buf+1)=(char)(s+'0');
		s=a/10;
		a=a%10;
		*(buf+2)=(char)(s+'0');
		*(buf+3)=(char)(a+'0');
		*(buf+4)=NULL;
	}
	else if (dat>99){
		s=dat/100;
		a=dat%100;
		*(buf)=(char)(s+'0');
		s=a/10;
		a=a%10;
		*(buf+1)=(char)(s+'0');
		*(buf+2)=(char)(a+'0');
		*(buf+3)=NULL;
	}
	else if(dat>9){
		s=dat/10;
		a=dat%10;
		*(buf)=(char)(s+'0');
		*(buf+1)=(char)(a+'0');
		*(buf+2)=NULL;
	}
	else{
		*(buf)=(char)(dat+'0');
		*(buf+1)=NULL;
	}
}

void eep_writeproc(void){
	char *bufadr;
	byte	b;
	int		i;

	bufadr=com_data[0];
	for (i=0;i<16;i++){
		b=HEXtoBIN(bufadr); 
		write_eeprom(eepadr,b);
		bufadr+=2;
		eepadr++;
		if(*bufadr==NULL)break;
		__delay_ms(2);
	}	
}

//
void eep_readproc(void){
//	unsigned long ui;
	int	j;
	byte b[4];
//	unsigned long adr=0;
//	unsigned long us;
	int num;
	char tmpc[33];
	char *tc;

	tmpc[32]=NULL;
//	us=(unsigned long)b[0]+((unsigned long)b[1]<<8)+((unsigned long)b[2]<<16)+((unsigned long)b[3]<<24);

//	for(ui=0;ui<us;ui=ui+16){
	if(eepsize>eepadr){
		tc=tmpc;
		if(eepsize-eepadr>16)num=16;
		else num=eepsize-eepadr;

		for(j=0;j<num;j++){
			b[0]=read_eeprom(eepadr);
			BINtoHEX(b[0],tc);
			eepadr++;tc+=2;
		}
		*tc=NULL;	
		usart_put(tmpc);
		usart_puts("\r\n");
	}
	else {
		eeprom_stat=0;
		usart_puts("DONE\r\n");
	}
}
unsigned long Ascii2Ulong(char *s)
{
	byte	i=0;
	unsigned long	temp=0;

	for(i=0;i<8;i++){
		if (s[i]=='\0') break;
		temp =temp<<4;

		if(s[i]>='0' && s[i]<='9'){
			temp += (s[i] - '0');
		}else if(s[i]>='A' && s[i]<='F'){
			temp += (s[i] - 55);
		}else if(s[i]>='a' && s[i]<='f'){
			temp += (s[i] - 87);
		}else{
			return 0xffffffff;	// 引数チェックでNG
		}

	}

	return temp;
}
