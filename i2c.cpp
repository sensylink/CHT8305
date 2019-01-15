#include "stdafx.h"
#include"I2C.h"

/*******************************************************************************
* 函数名         : Delay10us()
* 函数功能		   : 延时10us
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
int SCL;
int SDA;
void Delay10us(int tout=1)
{
	unsigned char a, b;
	int i;
	for (i = 0; i <tout; i++) {
		for (b = 1; b>0; b--)
		for (a = 2; a > 0; a--);
	}

}
/*******************************************************************************
* 函数名         : i2c_start()
* 函数功能		 : 起始信号：在SCL时钟信号在高电平期间SDA信号产生一个下降沿
* 输入           : 无
* 输出         	 : 无
* 备注           : 起始之后SDA和SCL都为0
*******************************************************************************/

void i2c_start()
{
	SDA = 1;
	Delay10us();
	SCL = 1;
	Delay10us();//建立时间是SDA保持时间>4.7us
	SDA = 0;
	Delay10us();//保持时间是>4us
	SCL = 0;
	Delay10us();
}
/*******************************************************************************
* 函数名         : i2c_stop()
* 函数功能		 : 终止信号：在SCL时钟信号高电平期间SDA信号产生一个上升沿
* 输入           : 无
* 输出         	 : 无
* 备注           : 结束之后保持SDA和SCL都为1；表示总线空闲
*******************************************************************************/

void i2c_stop()
{
	SDA = 0;
	Delay10us();
	SCL = 1;
	Delay10us();//建立时间大于4.7us
	SDA = 1;
	Delay10us();
}
/*******************************************************************************
* 函数名         : i2c_sendbyte(unsigned char dat)
* 函数功能		 : 通过I2C发送一个字节。在SCL时钟信号高电平期间，保持发送信号SDA保持稳定
* 输入           : num
* 输出         	 : 0或1。发送成功返回1，发送失败返回0
* 备注           : 发送完一个字节SCL=0,SDA=1
*******************************************************************************/

unsigned char i2c_sendbyte(unsigned char dat)
{
	unsigned char a = 0, b = 0;//最大255，一个机器周期为1us，最大延时255us。		
	for (a = 0; a<8; a++)//要发送8位，从最高位开始
	{
		SDA = dat >> 7;	 //起始信号之后SCL=0，所以可以直接改变SDA信号
		dat = dat << 1;
		Delay10us();
		SCL = 1;
		Delay10us();//建立时间>4.7us
		SCL = 0;
		Delay10us();//时间大于4us		
	}
	SDA = 1;
	Delay10us();
	SCL = 1;
	while (SDA)//等待应答，也就是等待从设备把SDA拉低
	{
		b++;
		if (b>200)	 //如果超过2000us没有应答发送失败，或者为非应答，表示接收结束
		{
			SCL = 0;
			Delay10us();
			return 0;
		}
	}
	SCL = 0;
	Delay10us();
	return 1;
}
/*******************************************************************************
* 函数名         : i2c_readbyte()
* 函数功能		   : 使用I2c读取一个字节
* 输入           : 无
* 输出         	 : dat
* 备注           : 接收完一个字节SCL=0,SDA=1.
*******************************************************************************/

unsigned char i2c_readbyte(uchar NACK=1)
{
	unsigned char a = 0, dat = 0;
	SDA = 1;			//起始和发送一个字节之后SCL都是0
	Delay10us();
	for (a = 0; a<8; a++)//接收8个字节
	{
		SCL = 1;
		Delay10us();
		dat <<= 1;
		dat |= SDA;
		Delay10us();
		SCL = 0;
		Delay10us();
	}
	//Set the NACK
	SCL = 1;
	Delay10us();
	SDA = NACK;
	Delay10us();
	SCL = 0;
	return dat;
}


/*******************************************************************************
* 函数名         : void reg_write(unsigned char addr,unsigned char dat)
* 函数功能		   : 往24c02的一个地址写入一个数据
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/

void i2c_reg_write(unsigned char dev_addr, unsigned char addr, unsigned int dat)
{
	uchar wdata;
	i2c_start();
	i2c_sendbyte(dev_addr);//发送写器件地址
	i2c_sendbyte(addr);//发送要写入内存地址
	wdata = (dat >> 8) & 0xff;
	i2c_sendbyte(wdata);	//发送数据 MSB
	wdata = (dat     ) & 0xff;
	i2c_sendbyte(wdata);	//发送数据 LSB
	i2c_stop();
}
/*******************************************************************************
* 函数名         : unsigned char reg_read(unsigned char addr)
* 函数功能		   : 读取24c02的一个地址的一个数据
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/

unsigned char i2c_reg_read(unsigned char dev_addr, unsigned char addr)
{
	unsigned char num;
	uchar read_addr = dev_addr | 0x01;
	i2c_start();
	i2c_sendbyte(dev_addr); //发送写器件地址
	i2c_sendbyte(addr); //发送要读取的地址
	i2c_start();
	i2c_sendbyte(read_addr); //发送读器件地址
	num = i2c_readbyte(); //读取数据
	i2c_stop();
	return num;
}

