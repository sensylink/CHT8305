#include "stdafx.h"
#include"I2C.h"

/*******************************************************************************
* ������         : Delay10us()
* ��������		   : ��ʱ10us
* ����           : ��
* ���         	 : ��
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
* ������         : i2c_start()
* ��������		 : ��ʼ�źţ���SCLʱ���ź��ڸߵ�ƽ�ڼ�SDA�źŲ���һ���½���
* ����           : ��
* ���         	 : ��
* ��ע           : ��ʼ֮��SDA��SCL��Ϊ0
*******************************************************************************/

void i2c_start()
{
	SDA = 1;
	Delay10us();
	SCL = 1;
	Delay10us();//����ʱ����SDA����ʱ��>4.7us
	SDA = 0;
	Delay10us();//����ʱ����>4us
	SCL = 0;
	Delay10us();
}
/*******************************************************************************
* ������         : i2c_stop()
* ��������		 : ��ֹ�źţ���SCLʱ���źŸߵ�ƽ�ڼ�SDA�źŲ���һ��������
* ����           : ��
* ���         	 : ��
* ��ע           : ����֮�󱣳�SDA��SCL��Ϊ1����ʾ���߿���
*******************************************************************************/

void i2c_stop()
{
	SDA = 0;
	Delay10us();
	SCL = 1;
	Delay10us();//����ʱ�����4.7us
	SDA = 1;
	Delay10us();
}
/*******************************************************************************
* ������         : i2c_sendbyte(unsigned char dat)
* ��������		 : ͨ��I2C����һ���ֽڡ���SCLʱ���źŸߵ�ƽ�ڼ䣬���ַ����ź�SDA�����ȶ�
* ����           : num
* ���         	 : 0��1�����ͳɹ�����1������ʧ�ܷ���0
* ��ע           : ������һ���ֽ�SCL=0,SDA=1
*******************************************************************************/

unsigned char i2c_sendbyte(unsigned char dat)
{
	unsigned char a = 0, b = 0;//���255��һ����������Ϊ1us�������ʱ255us��		
	for (a = 0; a<8; a++)//Ҫ����8λ�������λ��ʼ
	{
		SDA = dat >> 7;	 //��ʼ�ź�֮��SCL=0�����Կ���ֱ�Ӹı�SDA�ź�
		dat = dat << 1;
		Delay10us();
		SCL = 1;
		Delay10us();//����ʱ��>4.7us
		SCL = 0;
		Delay10us();//ʱ�����4us		
	}
	SDA = 1;
	Delay10us();
	SCL = 1;
	while (SDA)//�ȴ�Ӧ��Ҳ���ǵȴ����豸��SDA����
	{
		b++;
		if (b>200)	 //�������2000usû��Ӧ����ʧ�ܣ�����Ϊ��Ӧ�𣬱�ʾ���ս���
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
* ������         : i2c_readbyte()
* ��������		   : ʹ��I2c��ȡһ���ֽ�
* ����           : ��
* ���         	 : dat
* ��ע           : ������һ���ֽ�SCL=0,SDA=1.
*******************************************************************************/

unsigned char i2c_readbyte(uchar NACK=1)
{
	unsigned char a = 0, dat = 0;
	SDA = 1;			//��ʼ�ͷ���һ���ֽ�֮��SCL����0
	Delay10us();
	for (a = 0; a<8; a++)//����8���ֽ�
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
* ������         : void reg_write(unsigned char addr,unsigned char dat)
* ��������		   : ��24c02��һ����ַд��һ������
* ����           : ��
* ���         	 : ��
*******************************************************************************/

void i2c_reg_write(unsigned char dev_addr, unsigned char addr, unsigned int dat)
{
	uchar wdata;
	i2c_start();
	i2c_sendbyte(dev_addr);//����д������ַ
	i2c_sendbyte(addr);//����Ҫд���ڴ��ַ
	wdata = (dat >> 8) & 0xff;
	i2c_sendbyte(wdata);	//�������� MSB
	wdata = (dat     ) & 0xff;
	i2c_sendbyte(wdata);	//�������� LSB
	i2c_stop();
}
/*******************************************************************************
* ������         : unsigned char reg_read(unsigned char addr)
* ��������		   : ��ȡ24c02��һ����ַ��һ������
* ����           : ��
* ���         	 : ��
*******************************************************************************/

unsigned char i2c_reg_read(unsigned char dev_addr, unsigned char addr)
{
	unsigned char num;
	uchar read_addr = dev_addr | 0x01;
	i2c_start();
	i2c_sendbyte(dev_addr); //����д������ַ
	i2c_sendbyte(addr); //����Ҫ��ȡ�ĵ�ַ
	i2c_start();
	i2c_sendbyte(read_addr); //���Ͷ�������ַ
	num = i2c_readbyte(); //��ȡ����
	i2c_stop();
	return num;
}

