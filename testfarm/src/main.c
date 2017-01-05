//////////////////////////////////////////////////////////////////////////////////
// Company:		T.S.Labolatory Coporation
// Engineer:		
// 
// Create Date:   	Jul/18/2013
// Project Name: 	CK4
// Module Name:   	main.c
// Tool versions: 	MPLAB v8.91
//				Microchip XC16
// Description: 
//
// Dependencies: 
//
// Revision: 		0.0.0.0 - Initial
//                  0.0.0.1 - �i��؂�ւ����̏����l���o �ǉ�
// Additional Comments: 
//
//
//////////////////////////////////////////////////////////////////////////////////
#include "ports.h"
#include "common.h"
#include "ck4_gpio.h"
#include "ck4_timer.h"
#include "ck4_uart.h"
#include "ck4_gvar.h"
#include "ck4_i2c.h"
#include "ck4_fpga.h"
#include "libpic30.h"

// Set Configuration Bit
#include "Config_24E.h"

/* �ÓI�ϐ��錾 */
byte CmdFlag = 0;
byte Key_Access;		//!< @brief �L�[��t����/�֎~�t���O

	int stat_1;
/**
 *	@brief �V���A���ԍ�
 */


/**
 *	@brief �t�@�[���o�[�W����
 */
const char SOURCE_VERSION[] = Firm_Version;



/** @brief
 *	�X�e�[�g�}�V��
 */
enum _status {
	STAT_PWUP = 0,			//!< �N���O����
	STAT_WAIT,				//!< FPGA�N���ҋ@
	STAT_IDLE,				//!< �A�C�h�����
	STAT_MARK_1,				//!< �X�C�b�`���͏��
	STAT_MARK_2,				//!< �X�C�b�`���͏��
	STAT_MARK_3,				//!< �X�C�b�`���͏��
	STAT_MODE_1,				//!< �X�C�b�`���͏��
	STAT_MODE_2,				//!< �X�C�b�`���͏��
	STAT_MODE_3,				//!< �X�C�b�`���͏��
	STAT_MODE_4,				//!< �X�C�b�`���͏��
	STAT_MODE_5,				//!< �X�C�b�`���͏��
	STAT_MODE_6,				//!< �X�C�b�`���͏��
	STAT_MODE_7,				//!< �X�C�b�`���͏��
	STAT_FPGA,				//!< FPGA�փf�[�^�]�����
	STAT_DEBUG,				//!< DEBUG�p
	STAT_TRACK,				//!< �g���b�L���O���s
	STAT_WAIT_SW_RELEASE,		//!< �X�C�b�`�����[�X�҂�
	STAT_WAIT_SW_RELEASE_2,	//!< �X�C�b�`�����[�X�҂�_2
	STAT_WAIT_TIME,			//!< �����|�W�V���j���O�̕\���ҋ@
	STAT_FINISH,				//!< �I������
};
enum _CommandFlag {
	CMD_NON = 0,		//!< �J�ڂȂ�
	CMD_WRITE,		//!< �ݒ�R�}���h
	CMD_READ,			//!< �ǂݏo���R�}���h
	CMD_TRK_STA,		//!< �g���b�L���O�R�}���h
	CMD_TRK_STP,		//!< �g���b�L���O�R�}���h
};

/** @brief
 *	�T�u�X�e�[�g�}�V��
 */

/** @brief
 *	FPGA�R�}���h
 */

// Parameter -----------------------------------------------------------

// �[��I2C��M�p
	byte dbg_i2c = FALSE;
	byte i2c_1st = 0;
	byte i2c_2nd = 0;
// �[��I2C��M�p



byte dbg_flg = FALSE;		// for Debug



// �v���g�^�C�v�錾
int main(void);
void loop(void);
void init(void);
byte usart_command_handler(void);
byte exec_command(char *com);
void exec_i2c_command(void);
void delay_us(int uSec);
void delay_ms(int mSec);
void var_init(void);



// �O���[�o���ϐ��錾

/*********************************
 * �������֐�
 *********************************/
void init(void)
{
	gpio_init();		// GPIO������
	var_init();		// ���荞�ݑO�ϐ�������
	uart_init();		// uart������
	timer_init();		// �^�C�}�[������
	i2c_init();		// I2C������(Master Mode)

	int i;
	for(i = 0; i < 24; ++i) {
		pin_copy_tbl[i] = -1;
	}
}

void var_init(void){
	device_no=~PORTE;
	device_no=pre_device_no=((device_no>>4)&0xf)*10+(device_no&0xf);
	define_reading=0;
	eeprom_stat=0;
	eeprom_used=0;
	serial_pin[0]=0;
	serial_pin[1]=1;
	serial_pin[2]=2;
	serial_pin[3]=3;

	ID_name[0]=NULL;

	tgDut=0x0001;

}

/*********************************
 * ���C���֐�
 *********************************/
int main(void)
{
	byte b[4];
	byte a;

	// Configure Oscillator
	// Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
	// Fosc= 8*40/(2*2)= 80Mhz
	PLLFBD = 38;				// PLLDIV=1/40 to 160MHz
	CLKDIVbits.PLLPRE = 0;		// N1= 2
	CLKDIVbits.PLLPOST = 0;	// N2= 2

	OSCTUNbits.TUN = 23;				// FRC�̎��g������(��8MHz)

	while(OSCCONbits.LOCK != 1) {};		// Wait for PLL to lock

	// IEC all clear
	IEC0 = 0x0000;
	IEC1 = 0x0000;
	IEC2 = 0x0000;
	IEC3 = 0x0000;
	IEC4 = 0x0000;
	IEC5 = 0x0000;
//	IEC6 = 0x0000;
	IEC7 = 0x0000;
	IEC8 = 0x0000;

	_NSTDIS = 1;	// ���荞�݂̃l�X�e�B���O(0:���C1:�֎~)

	init();		// �e�평����


//	I2C1ADD = 0x0;				// �O������ݒ肳�ꂽI2C�X���[�u�A�h���X(�Ƃ肠�����O)


	// �}�C�R���N����̑ҋ@
	__delay_ms(2500);				// 2.5s wait�iFPGA�N���ҋ@�j

	fpga_init();
	

//debug
//				write_eeprom(0,0xa5);
//				write_eeprom(EEP_USED,0xff);
//				write_eeprom(EEP_USED+1,0xff);
//				write_eeprom(EEP_USED+2,0xff);
//				write_eeprom(EEP_USED+3,0xff);
////
	__delay_ms(5);
	a=read_eeprom(0);
	b[0]=read_eeprom(EEP_USED);
	b[1]=read_eeprom(EEP_USED+1);
	b[2]=read_eeprom(EEP_USED+2);
	b[3]=read_eeprom(EEP_USED+3);

	if((b[3]==0xff)&&(b[2]==0xff)&&(b[1]==0xff)&&(b[0]==0xff)){
		//eeprom ����
		eepsize=0;
		eeprom_used=0;
	}
	else{
		eepsize=(unsigned long)b[0]+((unsigned long)b[1]<<8)+((unsigned long)b[2]<<16)+((unsigned long)b[3]<<24);
		eeprom_used=1;
		get_device_info();
	}	
	b[0]=read_eeprom(EEP_SERIAL);
	b[1]=read_eeprom(EEP_SERIAL+1);
	b[2]=read_eeprom(EEP_SERIAL+2);
	b[3]=read_eeprom(EEP_SERIAL+3);
	if(b[0]>=24) serial_pin[0]=0;
	else serial_pin[0]=(int)b[0];
	if(b[1]>=24) serial_pin[1]=1;
	else serial_pin[1]=(int)b[1];
	if(b[2]>=24) serial_pin[2]=2;
	else serial_pin[2]=(int)b[2];
	if(b[3]>=24) serial_pin[3]=3;
	else serial_pin[3]=(int)b[3];


	loop();	// �G���h���X���[�v

	return 0;
}

//*********************************
//* ���[�v�֐�
//*********************************
void loop(void)
{

	// �X�e�[�g�}�V��
	while(1){
		usart_command_handler();
		__delay_ms(5);
//		fpga_debug();

		//DBG_1 ^= 1;

		// �E�H�b�`�h�b�O�^�C�}���N���A
//		ClrWdt();


		// �X�e�[�g�}�V�� ���g�p
		switch(stat_1){

			//*****************
			// �N������
			//*****************
			case STAT_PWUP:
				break;

			//*****************
			// �ҋ@
			//*****************
			case STAT_WAIT:
				break;

			//*****************
			// �A�C�h�����
			//*****************
			case STAT_IDLE:
				break;



			//*****************
			// ���̑��̃X�e�[�g
			//*****************
			default:
				stat_1 = STAT_IDLE;		// �A�C�h���֑J��
				break;

		}// end switch




	}// end while
}



//------------------------------------------------------------------------------------------------------------
// main.c END

