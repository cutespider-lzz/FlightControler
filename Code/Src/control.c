#include "control.h"

float ControlTime;
const float ControlDt = 0.01f;

SemaphoreHandle_t ControlSemaphore;//控制模块二值信号量
BaseType_t ControlHigherTaskSwitch;

const double Kp_roll=1,Ki_roll=0.2,Kd_roll=0.2,Kp_pitch=2,Ki_pitch=0.1,Kd_pitch = 1,Kp_yaw=1.5,Ki_yaw=0.1,Kd_yaw=0.1;//姿态控制参数
double expected_roll,expected_pitch,expected_yaw,expected_height;//各通道期望值
double servo_roll,servo_pitch,servo_yaw;//对应通道角度
double integtal_roll,integtal_pitch;//俯仰角误差积分
FMUControlModeEnum FMUControlMode = FMU_Manual;//飞控工作模式选择
//FMUControlModeEnum FMUControlModePrevious = FMU_Manual;

void ServoSet(ServoChannel channel,double angle)//
{
	//漫游者舵机参数
	uint8_t ServoDirection[8] = {0,1,0,0,1,0,0,0};
	int16_t ServoOffset[8] = {0,100,0,0,0,120,0,0};
	int16_t angle_int16;
	switch(channel)
	{
		case ServoChannel_1:
			if(ServoDirection[ServoChannel_1]) angle = -angle;
			angle_int16 = angle*11.1+1500+ServoOffset[ServoChannel_1];
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,angle_int16);
			break;
		case ServoChannel_2:
			if(ServoDirection[ServoChannel_2]) angle = -angle;
			angle_int16 = angle*11.1+1500+ServoOffset[ServoChannel_2];
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,angle_int16);
			break;
		case ServoChannel_3:
			if(ServoDirection[ServoChannel_3]) angle = -angle;
			angle_int16 = angle*11.1+1500+ServoOffset[ServoChannel_3];
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,angle_int16);
			break;
		case ServoChannel_4:
			if(ServoDirection[ServoChannel_4]) angle = -angle;
			angle_int16 = angle*11.1+1500+ServoOffset[ServoChannel_4];
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,angle_int16);
			break;
		case ServoChannel_5:
			if(ServoDirection[ServoChannel_5]) angle = -angle;
			angle_int16 = angle*11.1+1500+ServoOffset[ServoChannel_5];
			__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,angle_int16);
			break;
		case ServoChannel_6:
			if(ServoDirection[ServoChannel_6]) angle = -angle;
			angle_int16 = angle*11.1+1500+ServoOffset[ServoChannel_6];
			__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_2,angle_int16);
			break;
		case ServoChannel_7:
			if(ServoDirection[ServoChannel_7]) angle = -angle;
			angle_int16 = angle*11.1+1500+ServoOffset[ServoChannel_7];
			__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_3,angle_int16);
			break;
		case ServoChannel_8:
			if(ServoDirection[ServoChannel_8]) angle = -angle;
			angle_int16 = angle*11.1+1500+ServoOffset[ServoChannel_8];
			__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,angle_int16);
			break;
	}
}

void ControlInit(void)
{
	__HAL_TIM_SET_COUNTER(&htim3,0);
	__HAL_TIM_ENABLE_IT(&htim3,TIM_IT_UPDATE);
	ControlTime = 0;
}

void MYZControl(void)
{
	volatile double pitch = AHRSData.Pitch;
	volatile double roll = AHRSData.Roll;
	volatile double yaw = AHRSData.Heading;
	double gx = AHRSData.PitchSpeed;
	double gy = AHRSData.RollSpeed;
	double gz = AHRSData.HeadingSpeed;
	switch(FMUControlMode)
	{
		case FMU_Manual:
		{
			//第一组舵机
			expected_roll = (ReceiverChannel[0]-ReceiverChannelNeutral[0])*0.09;
			expected_pitch = (ReceiverChannel[1]-ReceiverChannelNeutral[1])*0.03;
			expected_yaw = (ReceiverChannel[3]-ReceiverChannelNeutral[3])*0.015;
			servo_roll = expected_roll;
			servo_pitch = expected_pitch;
			servo_yaw = expected_yaw;
			ServoSet(ServoChannel_1,servo_roll);
			ServoSet(ServoChannel_5,servo_roll);
			ServoSet(ServoChannel_2,servo_pitch);
			ServoSet(ServoChannel_6,servo_pitch);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,ReceiverChannel[2]);
			ServoSet(ServoChannel_4,servo_yaw);
			ServoSet(ServoChannel_7,servo_yaw);
			break;
		}
		case FMU_Stable:
		{
			//滚转与俯仰角期望值 0.09为45°
			expected_roll = (ReceiverChannel[0]-ReceiverChannelNeutral[0])*0.09;
			expected_pitch = (ReceiverChannel[1]-ReceiverChannelNeutral[1])*0.06;
			expected_yaw = (ReceiverChannel[3]-ReceiverChannelNeutral[3])*0.015;
			//计算俯仰角误差积分
			integtal_pitch = integtal_pitch+(expected_pitch-pitch)*ControlDt;
      integtal_pitch = integtal_pitch>10?10:integtal_pitch;
      integtal_pitch = integtal_pitch<-10?-10:integtal_pitch;
			//计算舵机角度
//			servo_roll = Kp_roll*(expected_roll-roll)-Kd_roll*IMUData.tran_gyr_y;
			servo_roll = Kp_roll*(expected_roll-roll)-Kd_roll*gy;
			servo_roll = servo_roll>45?45:servo_roll;
			servo_roll = servo_roll<-45?-45:servo_roll;
//			servo_pitch = Kp_pitch*(expected_pitch-pitch)-Kd_pitch*IMUData.tran_gyr_x+Ki_pitch*integtal_pitch;
			servo_pitch = Kp_pitch*(expected_pitch-pitch)-Kd_pitch*gx+Ki_pitch*integtal_pitch;
			servo_pitch = servo_pitch>16?16:servo_pitch;
			servo_pitch = servo_pitch<-16?-16:servo_pitch;
			servo_yaw = expected_yaw;
			ServoSet(ServoChannel_1,servo_roll);
			ServoSet(ServoChannel_5,servo_roll);
			ServoSet(ServoChannel_2,servo_pitch);
			ServoSet(ServoChannel_6,servo_pitch);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,ReceiverChannel[2]);
			ServoSet(ServoChannel_4,servo_yaw);
			ServoSet(ServoChannel_7,servo_yaw);
			break;
		}
//		case FMU_Height:
//		{
//			//滚转与俯仰角期望值
//			expected_height = expected_height + ((ReceiverChannel[1]-ReceiverChannelNeutral[1]))*3.0/50000.0;
//			expected_roll = (ReceiverChannel[0]-ReceiverChannelNeutral[0])*0.09;
//			expected_pitch = Kp_height*(expected_height-IMUData.height)+fabs(IMUData.roll)*0.3;
//			//限制俯仰角上下限
//			expected_pitch = expected_pitch>30?30:expected_pitch;
//			expected_pitch = expected_pitch<-30?-30:expected_pitch;
//			//计算俯仰角误差积分
//			integtal_pitch = integtal_pitch+(expected_pitch-pitch)*ControlDt;
//      integtal_pitch = integtal_pitch>10?10:integtal_pitch;
//      integtal_pitch = integtal_pitch<-10?-10:integtal_pitch;
//			//计算舵机角度
////			servo_roll = Kp_roll*(expected_roll-roll)-Kd_roll*IMUData.tran_gyr_y;
//			servo_roll = Kp_roll*(expected_roll-roll)-Kd_roll*gy;
//			servo_roll = servo_roll>30?30:servo_roll;
//			servo_roll = servo_roll<-30?-30:servo_roll;
////			servo_pitch = Kp_pitch*(expected_pitch-pitch)-Kd_pitch*IMUData.tran_gyr_x+Ki_pitch*integtal_pitch;
//			servo_pitch = Kp_pitch*(expected_pitch-pitch)-Kd_pitch*gx+Ki_pitch*integtal_pitch;
//			servo_pitch = servo_pitch>16?16:servo_pitch;
//			servo_pitch = servo_pitch<-16?-16:servo_pitch;
//			ServoSet(ServoChannel_1,servo_roll);
//			ServoSet(ServoChannel_5,servo_roll);
//			ServoSet(ServoChannel_2,servo_pitch);
//			ServoSet(ServoChannel_6,servo_pitch);
//			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,ReceiverChannel[2]);
//			ServoSet(ServoChannel_4,expected_yaw);
//			ServoSet(ServoChannel_7,expected_yaw);
//			break;
//		}
	}
}

