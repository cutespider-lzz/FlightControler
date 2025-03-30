#ifndef __TASKINIT_H
#define __TASKINIT_H

#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include "tf.h"

//任务初始化
void TaskInit(void);

//LEDTwink函数声明
extern BaseType_t LEDTwink_Ret;
extern UBaseType_t LEDTwink_Prio;
extern TaskHandle_t LEDTwink_TCB;

void LEDTwink(void *pvParameters);

//TFStorage函数声明
extern BaseType_t TFStorage_Ret;
extern UBaseType_t TFStorage_Prio;
extern TaskHandle_t TFStorage_TCB;

void TFStorage(void *pvParameters);

#endif
