#include "all.h"
#define POLYSIZE 100
struct PPoly
{
	int  num;
	fp64 breaks[POLYSIZE];
	fp64 coefs[POLYSIZE][4];
};
fp64  Gyro_Convert1=-1.6068;    //正转系数
fp64  Gyro_Convert2=-1.6068;	//反转系数

// 校陀螺仪零漂用到的
int32_t Gyro_Float=0;
uint8_t Float_State;
uint8_t Gyro_Float_Flag1=0;
uint8_t Gyro_Float_Flag2=0;
uint8_t Gyro_Float_Flag=0;

uint8_t Gyro_Error_Flag=0;

//陀螺仪当前值
struct Gyro_Struct Gyro_Now={0,0};

//陀螺仪积分值(原始值、角度、弧度)
int64_t Gyro_Total;
int64_t Gyro_Total_NoFloat;
fp64 Gyro_Radian_Total;
fp64 Gyro_Angle_Total;

fp64 Gyro_Multiplier=1.0;//非线性系数
fp64 time_interval=1;

void Gyro_Update(void)
{
	Gyro_Total+=Gyro_Now.value;//1152 往大调是负偏
	//Gyro_Multiplier=PPoly_Eval(fabs(1000*Gyro_Radian_Step/time_interval),GREENGYRO);
	Gyro_Multiplier=1.0;
    
    Gyro_Total_NoFloat = Gyro_Total-Gyro_Float*Gyro_Now.count;
	
	Gyro_Angle_Total = Gyro_Total_NoFloat*0.0000001*Gyro_Convert1*Gyro_Multiplier;
	
	Gyro_Angle_Total=Gyro_Angle_Total-(int)(Gyro_Angle_Total/360)*360;
	
	if(Gyro_Angle_Total>180.0)
		Gyro_Angle_Total-=360.0;
	if(Gyro_Angle_Total<-180.0)
		Gyro_Angle_Total+=360.0;
	Gyro_Radian_Total=Gyro_Angle_Total*PI/180;
	
// 	LCD_WriteNumXY(Gyro_Angle_Total,0,0);
// 	LCD_WriteNumXY(Gyro_Float,0,1);

// 	switch((int)Float_State)
// 	{
// 		case Error:
// 			LCD_WriteStringXY("Error!",0,2);
// 			break;
// 		case Changed:
// 			LCD_WriteStringXY("Changed!",0,2);
// 			break; 
// 		case Normal:
// 			LCD_WriteStringXY("Normal!",0,2);
// 			break;
// 	}
}
// void Gyro_NonLinear(void)
// {
// 	Gyro_Radian_Total
// 	Gyro_Multiplier=PPoly_Eval(fabs(1000*Gyro_Radian_Step/time_interval),GREENGYRO);
// 	delay(1);
// }
void Gyro_CheckFloat(void)
{
	LED2_on;
	Gyro_Clear();
	Gyro_Float_Flag=1;
	Gyro_Float_Flag1=1;
	Gyro_Float_Flag2=1;
	while(Gyro_Float_Flag)
		delay(1);
	if(Gyro_Float_Total2-Gyro_Float_Total1==0)
		Float_State=CF_ERROR;
	else if(Int64Abs(Gyro_Float_Total2-Gyro_Float_Total1-Gyro_Float*1024)>40960)
	{
		Gyro_Float=(Gyro_Float_Total2-Gyro_Float_Total1)/1024;
		Float_State=CF_CHANGED;
		Data_Save();
	}
	else
	{
		Float_State=CF_NORMAL;
	}
	
	Gyro_Clear();
	LED2_off;
}
void Gyro_Init_P(void)
{
	Gyro_Convert1=3600*10000000.0/(fp64)Gyro_Total_NoFloat;
}
void Gyro_Init_N(void)
{
	Gyro_Convert2=-3600*10000000.0/(fp64)Gyro_Total_NoFloat;
}
void Gyro_Clear(void)
{
	Gyro_Total=0;
	Gyro_Now.count=0;
}
/********************************************************************************************************
Function:	Evaluate Piecewise Heimite Polynomial 
Created by 	.Somnus
Version 	1.0
*********************************************************************************************************/
fp64 PPoly_Eval(fp64 data,struct PPoly poly)
{
	fp64 result=1.13;
	int i;
	
	for (i=0;i<poly.num-1;i++)
	{
		if (data>poly.breaks[i]&&data<poly.breaks[i+1])
		{
			data=data-poly.breaks[i];
			result=poly.coefs[i][3]*pow(data,3)+poly.coefs[i][2]*
				pow(data,2)+poly.coefs[i][1]*pow(data,1)+poly.coefs[i][0];
			break;
		}
	}

	return result;
}

/********************************************************************************************************
Polynomials
Data acquired by Zhou Changxing
*********************************************************************************************************/
struct PPoly WHITEGYRO=
{17,
{0.00000,0.37400,0.74800,1.12200,1.49600,1.87000,2.24399,2.61799,2.99199,3.36599,3.73999,4.11399,4.48799,4.86199,5.23599,5.60999,5.98399,6.35799},
{{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.01576,-0.01437},
{1.00145,0.00576,0.03865,-0.06497},
{1.00561,0.00740,-0.01917,0.03799},
{1.00769,0.00900,0.09920,-0.15853},
{1.01664,0.01668,-0.01796,0.02029},
{1.02142,0.01176,-0.01731,0.03996},
{1.02549,0.01559,0.06806,-0.09704},
{1.03577,0.02577,-0.00681,0.00751},
{1.04484,0.02382,-0.01136,0.02728},
{1.05359,0.02678,0.02438,-0.03282},
{1.06530,0.03124,-0.00815,0.02135},
{1.07696,0.03410,-0.03582,0.12099},
{1.07696,0.03410,-0.03582,0.12099}}
};

struct PPoly BLACKGYRO=
{17,
{0.00000,0.37400,0.74800,1.12200,1.49600,1.87000,2.24399,2.61799,2.99199,3.36599,3.73999,4.11399,4.48799,4.86199,5.23599,5.60999,5.98399,6.35799},
{{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.03856,-0.05956},
{1.00228,0.00385,-0.00598,0.00859},
{1.00333,0.00298,-0.00531,0.01554},
{1.00452,0.00553,0.10353,-0.16144},
{1.01262,0.01523,-0.02226,0.03457},
{1.01701,0.01308,-0.00129,0.01556},
{1.02254,0.01865,0.04372,-0.06959},
{1.03199,0.02215,-0.02226,0.04214},
{1.03937,0.02319,0.02497,-0.03145},
{1.04988,0.02866,0.00090,0.00159},
{1.06081,0.03000,-0.01226,0.03867},
{1.07234,0.03706,-0.00875,0.09057},
{1.07234,0.03706,-0.00875,0.09057}}
};


struct PPoly BLUEGYRO=
{17,
{0.00000,0.37400,0.74800,1.12200,1.49600,1.87000,2.24399,2.61799,2.99199,3.36599,3.73999,4.11399,4.48799,4.86199,5.23599,5.60999,5.98399,6.35799},
{{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.02323,-0.02863},
{1.00175,0.00537,0.00081,0.00436},
{1.00410,0.00780,0.00617,0.00136},
{1.00795,0.01298,0.03752,-0.06754},
{1.01452,0.01271,-0.02435,0.04541},
{1.01824,0.01355,0.03744,-0.04528},
{1.02618,0.02256,0.00739,-0.00894},
{1.03518,0.02434,0.00443,-0.00991},
{1.04439,0.02349,-0.01478,0.03224},
{1.05279,0.02596,0.02765,-0.03981},
{1.06429,0.02994,-0.04080,0.10369},
{1.07520,0.04294,0.15261,-0.13468},
{1.07520,0.04294,0.15261,-0.13468}}
};

struct PPoly GREENGYRO=
{17,
{0.00000,0.37400,0.74800,1.12200,1.49600,1.87000,2.24399,2.61799,2.99199,3.36599,3.73999,4.11399,4.48799,4.86199,5.23599,5.60999,5.98399,6.35799},
{{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.00000,0.00000},
{1.00000,0.00000,0.08772,-0.13579},
{1.00517,0.00863,-0.01692,0.02840},
{1.00751,0.00789,0.00335,0.01065},
{1.01149,0.01487,0.06468,-0.10271},
{1.02073,0.02015,-0.01587,0.02005},
{1.02709,0.01670,-0.01539,0.03892},
{1.03322,0.02151,0.06183,-0.09520},
{1.04493,0.02781,-0.01648,0.02404},
{1.05429,0.02557,-0.01052,0.03231},
{1.06407,0.03126,-0.00895,0.07814},
{1.07860,0.05736,0.24189,-0.27338},
{1.11959,0.12357,0.09803,-0.13286},
{1.11959,0.12357,0.09803,-0.13286}}
};
