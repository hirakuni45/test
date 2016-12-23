//////////////////////////////////////////////////////////////////////////////////
// Company:		T.S.Labolatory Coporation
// Engineer:	
// 
// Create Date:   	Jul/18/2013
// Project Name: 	Checker4
// Module Name:   	ck4_i2c.h
// Tool versions: 	MPLAB v8.91
//				Microchip XC16
// Description: 
//
// Dependencies: 
//
//
//////////////////////////////////////////////////////////////////////////////////
#ifndef _CK4_I2C_H
#define _CK4_I2C_H


#define IIC_DATA_WRITE			0x00
#define IIC_DATA_READ			0x01

#define IIC_EEPROM_SLAVE_ADDR	0xA0


void i2c_init(void);
void write_eeprom(unsigned long eAddress, byte eData);
byte read_eeprom(unsigned long eAddress);

int get_device_info();
void init_device_info(void);

#endif /*_CK4_I2C_H*/
