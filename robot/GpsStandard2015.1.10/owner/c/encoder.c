#include "all.h"

struct Encoder_Stat Encoders[EncoderAmount]=
{
	{0,0,0,-0.319001,-0.318843, 1,0.0,   0,0},
	{0,0,0,-0.320864,-0.320624, 1,0.0,PI/2,0},
};
fp64 N[EncoderAmount*4]={0};//用来标定码盘时记录用的

int32_t GET_ENCODER(int8_t i)
{
	int32_t temp;
	assert_param((i>0)&&(i<5));
	temp = (int32_t)(	((int32_t)(r_now[i]))*CPR + (int32_t)(Tim_S[i]->CNT) );
	return temp;
}

void Encoder_Update(void)
{
	u8 i;
	int64_t now;
	int32_t step;
	
	for(i=0;i<EncoderAmount;i++)
	{
		now	= GET_ENCODER(i);
		step = now-Encoders[i].Now;
		if(step<2000&&step>-2000)
		{
			Encoders[i].Now = now;
	    Encoders[i].Total = Encoders[i].Now-Encoders[i].Last;
		  if (step == Int32Abs(step))
				Encoders[i].Distance += Int32Abs(step)*Encoders[i].Convert1*Encoders[i].dir;
		  else
				Encoders[i].Distance -= Int32Abs(step)*Encoders[i].Convert2*Encoders[i].dir;
	  }
	}
}

void Encoder_Clear(int index)
{
	Encoders[index].Distance=0.0;
	Encoders[index].Last=Encoders[index].Now;
	Encoders[index].Total=0;
}


void Encoder_InitXY(uint8_t t)
{
	u8	i;
	for(i=0;i<EncoderAmount;i++)
		N[i*2+t] = Encoders[i].Total;
}

void Encoder_InitR(void)
{
	u8	i;
	for(i=0;i<EncoderAmount;i++)
		Encoders[i].Radius = Encoders[i].Distance/(10*PI+Gyro_Radian_Total);
}

/****************************************************************************************************
码盘标定过程：分别沿+y,-y,+x,-x四个方向推三米，记录两个码盘的读数，以下为N[8]数组与码盘读数的对应关系
		+y		+x
E1	N[0]	N[1]
E2	N[2]	N[3]
****************************************************************************************************/
void Encoder_Init(void)
{ 
	fp64	C;						//转动系数
	fp64	Angle;				//与+y夹角
	u8		i;
	u16		length = 3000;
	
	for(i=0;i<EncoderAmount;i++)
	{
		if(N[i*2+0]<0)
		{
			N[i*2+0]=-N[i*2+0];
			N[i*2+1]=-N[i*2+1];
		}
		Angle = atan(N[i*2+1]/N[i*2+0]);
		C = length*cos(Angle)/N[i*2+0];

		Encoders[i].radian	= Angle;
		Encoders[i].Convert1	= C;
		Encoders[i].Convert2	= C;
	}
}
