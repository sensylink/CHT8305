#ifndef CHT8305_H

#define CHT8305_H



/*

* CHT8305 Temperature/Humidity Sensor Library

* Product can be seen @ http://www.sensylink.com

* 10/2017

*/



#define byte unsigned char

#define uint unsigned int



#define CHT8305_DEV_ADDR  0x40<<1

#define CHT8305_REG_TEMP   0x00

#define CHT8305_REG_HUMI  0x01

#define CHT8305_REG_CONF  0x02

#define CHT8305_REG_MID     0xFE //0x5959

#define CHT8305_REG_DID      0xFF //0x8305



static const byte BIT_T_RES              = 2;

static const byte BIT_H_RES             = 0;

static const byte BIT_BATTERY_OK  = 3;

static const byte BIT_ACQ_MODE    = 4;

static const byte BIT_HEATER           = 5;

static const byte BIT_RST                  = 7;



static const byte T_RES_14 = 0;

static const byte T_RES_11 = 1;



static const byte H_RES_14 = 0;

static const byte H_RES_11 = 1;

static const byte H_RES_8   = 2;



void CHT8305_init();

bool CHT8305_is_connected();

uint CHT8305_get_manufacturerID();

uint CHT8305_get_deviceID();



void CHT8305_set_temperature_res(byte res);

void CHT8305_set_humidity_res(byte res);

void CHT8305_turnon_heater(bool heaterOn);

bool CHT8305_battery_OK();



void CHT8305_update_config_reg();

void CHT8305_read_reg(byte regAddr, byte numOfBytes);



uint CHT8305_get_temp_humi(float *t, float *h);



#endif
