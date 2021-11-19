/**********************
*文件名称：beeplayer.c
*作者：
*说明：振动传感器受到振动时，蜂鸣器工作  
*修改记录：
***********************/

/**********************
基于STC15F2K60S2系列单片机C语言编程实现
***********************/
#include "STC15F2K60S2.h"
#define uint unsigned int
#define uchar unsigned char
sbit vibrate=P2^4;		  //振动传感器
sbit beep=P3^4;			  //蜂鸣器
bit flag;				  //控制蜂鸣器发声
uchar timeh,timel;		  //定义定时器的重装值
uint time_counter;        //定时器中断计数
uchar quzi_index;	      //quzi数组的下标控制，在中断服务程序中用到
																						
uchar quzi[] ={	  //此数组数据为各个音符在定时器中的重装值，第一列是高位，第二列是低位
	0xf8,0x8c,	  //低八度，低1
	0xf9,0x5b,	  
	0xfa,0x15,	  //低3
	0xfa,0x67,
	0xfb,0x04,	  //低5
	0xfb,0x90,
	0xfc,0x0c,	  //低7	
	0xfc,0x44,	  //中央C调
	0xfc,0xac,	  //中2
	0xfd,0x09,
	0xfd,0x34,	  //中4
	0xfd,0x82,
	0xfd,0xc8,	  //中6
	0xfe,0x06,
	0xfe,0x22,	  //高八度，高1	
	0xfe,0x56,
	0xfe,0x85,	  //高3	
	0xfe,0x9a,
	0xfe,0xc1,	  //高5
	0xfe,0xe4,
	0xff,0x03	  //高7
};

uchar huanlesong[] = {
0xfd,0x09, //E5
0xfd,0x09, //E5
0xfd,0x34, //F5
0xfd,0x82, //G5
0xfd,0x82, //G5
0xfd,0x34, //F5
0xfd,0x09, //E5
0xfc,0xac, //D5
0xfc,0x44, //C5
0xfc,0x44, //C5
0xfc,0xac, //D5
0xfd,0x09, //E5
0xfd,0x09, //E5
0xfc,0xac, //D5
0xfc,0xac, //D5
0xfc,0xac, //D5				  
};
/**********************
*函数名称：void delay()
*功能描述：延时
*入口参数：xms：输入延时时间
*出口参数：无
*备注：
*
***********************/
void delay(unsigned int xms)
{
	uint i,j;						   
	 for(i=xms;i>0;i--)
	 	for(j=124;j>0;j--);
}

/**********************
*函数名称：void init_sys()
*功能描述：系统初始化，功能是配置IO口
*入口参数：无
*出口参数：无
*备注：
*
***********************/
void init_sys()			  
{
	P0M1=0x00; P0M0=0xff; 
	P2M1=0x10; P2M0=0x00;
	P3M1=0x00; P3M0=0x00;
						  //IO端口设置

	beep=0;	//蜂鸣器初试化为低电平
	TH0=0x00;
	TL0=0x00;	//初始化定时器计数初始值
	TMOD=0x01;  //定时器0，方式1
	EA=1;//总中断使能
	ET0=1;	//定时器0中断使能
	TR0=0;	 //暂时关闭定时器0
}
void main()
{
	init_sys();

	flag=0;
	time_counter=0;
	quzi_index=0; //变量初始化
	while(1)//不等地扫描是否有振动发生   
	{	    
		if(vibrate==0)
		{
			delay(5); //消抖
			if(vibrate==0)
			{
				flag=1;//有振动则将振动标志位置1
				TR0=1; //启动定时器
			}
		} 
			   
	}
	                                        		
}

void tim1() interrupt 1						//计时器控制频率
{
	if(flag)
	{
		time_counter++;	 //中断计数
		if(time_counter==(300+30*quzi_index))//用于控制每个特定频率声音的发声时长
		{
			time_counter=0;
			if(quzi_index<40)
				quzi_index=quzi_index+2;
			else 
					{
						quzi_index=0;
						 flag=0;
						 beep=0; //变量清零，为下次工作做好准备
					}
		}
		timeh=quzi[quzi_index];
		timel=quzi[quzi_index+1];	
		TH0=timeh;
		TL0=timel;
	
		beep=~beep;//beep管脚取反
	}

}
