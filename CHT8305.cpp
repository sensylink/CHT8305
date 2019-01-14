#include "CHT8305.h"

#include "I2C.h"

byte configReg; //higher 8 bits of the configuration register

byte buf[4];





void CHT8305_init()

{

	configReg = 0x10; //POR default

}



bool CHT8305_is_connected()

{

	byte ret =0;

	uint id = 0;



	i2c_start();

	ret = i2c_sendbyte(CHT8305_DEV_ADDR);

	id = CHT8305_get_manufacturerID();

	if ((ret == 1) && ( id== 0x5959))

		return true;

	return false;

}



void CHT8305_read_reg(byte regAddr, byte numOfBytes)

{

	byte READ_ADDR; 

	i2c_start();

	i2c_sendbyte(CHT8305_DEV_ADDR); //发送写器件地址

	i2c_sendbyte(regAddr); //发送要读取的地址



	if (regAddr == CHT8305_REG_TEMP || regAddr == CHT8305_REG_HUMI) {

		Delay10us(1000);  //10ms delay

	}

	

	READ_ADDR=CHT8305_DEV_ADDR | 0x01;

	i2c_start();

	i2c_sendbyte(READ_ADDR); //发送读器件地址

	



	for (int i = 0; i < numOfBytes-1; i++) {

		buf[i] = i2c_readbyte(0);  

	}

	buf[numOfBytes - 1] = i2c_readbyte(1); //Give NACK

	i2c_stop();



}



unsigned int CHT8305_get_manufacturerID()

{

	CHT8305_read_reg(CHT8305_REG_MID, 2);

	return buf[0] << 8 | buf[1];

}



unsigned int CHT8305_get_deviceID()

{

	CHT8305_read_reg(CHT8305_REG_DID, 2);

	return buf[0] << 8 | buf[1];

}



void CHT8305_update_config_reg()

{

	uint data = configReg << 8;

	i2c_reg_write(CHT8305_DEV_ADDR, 0x02, data);

}



void CHT8305_set_temperature_res(byte res)

{

	if (res > 1) res = T_RES_14;



	configReg &= ~(1 << BIT_T_RES);

	configReg |= res << BIT_T_RES;

}



void CHT8305_set_humidity_res(byte res)

{

	if (res > 2) res = H_RES_14;



	configReg &= ~(0x3 << BIT_H_RES);

	configReg |= res << BIT_H_RES;

}



void CHT8305_turnon_heater(bool heaterOn)

{

	if (heaterOn)

		configReg |= 1 << BIT_HEATER;

	else

		configReg &= ~(1 << BIT_HEATER);

}



bool CHT8305_battery_OK()

{

	CHT8305_read_reg(CHT8305_REG_CONF, 2);

	configReg = buf[0];



	return (configReg & (1 << BIT_BATTERY_OK)) == 0;

}



uint CHT8305_get_temp_humi(float *t, float *h)

{

	CHT8305_read_reg(CHT8305_REG_TEMP, 4);

	unsigned int th, tl, hh, hl;



	th = buf[0];

	tl = buf[1];

	hh = buf[2];

	hl = buf[3];



	(*t) = (th << 8 | tl) * 165.0 / 65535.0 - 40.0;

	(*h) = (hh << 8 | hl) * 100.0 / 65535.0;



	return 1;

}





int _tmain(int argc, _TCHAR* argv[])

{

	float temp, humi;

	CHT8305_init();



	CHT8305_get_temp_humi(&temp, &humi);



	return 0;

}

