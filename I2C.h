#ifndef I2C_H
#define I2C_H

//#include <reg52.h>

#define uchar unsigned char

//sbit SCL = P2 ^ 1;
//sbit SDA = P2 ^ 0;

void Delay10us(int tout);
void i2c_start();
void i2c_stop();
uchar i2c_sendbyte(uchar dat);
uchar i2c_readbyte(uchar NACK);
void  i2c_reg_write(uchar dev_addr,uchar addr, unsigned int dat);
uchar i2c_reg_read(uchar dev_addr, uchar addr);

#endif
