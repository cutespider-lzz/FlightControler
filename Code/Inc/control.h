#ifndef __CONTROL_H 
#define __CONTROL_H

#include "main.h"
#include "cmsis_os.h"

extern float ControlTime;
extern const float ControlDt;

extern SemaphoreHandle_t ControlSemaphore;//控制模块二值信号量
extern BaseType_t ControlHigherTaskSwitch;

typedef enum
{
	ServoChannel_1 = 0,
	ServoChannel_2,
	ServoChannel_3,
	ServoChannel_4,
	ServoChannel_5,
	ServoChannel_6,
	ServoChannel_7,
	ServoChannel_8,
}ServoChannel;

void ServoSet(ServoChannel channel,double angle);
void ControlInit(void);
void MYZControl(void);

#endif

