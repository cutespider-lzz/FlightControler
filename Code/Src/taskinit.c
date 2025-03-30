#include "taskinit.h"

void TaskInit(void)
{
	//Create LEDTwink
	LEDTwink_Ret = xTaskCreate((TaskFunction_t)LEDTwink,"LEDTwink",32,(void *)1,LEDTwink_Prio,(TaskHandle_t *)(&LEDTwink_TCB));
	if(LEDTwink_Ret == pdPASS) ;
	else
	{
		while(1) ;
	}
	//Create SDWrite
	TFStorage_Ret = xTaskCreate((TaskFunction_t)TFStorage,"TFStorage",256,(void *)1,TFStorage_Prio,(TaskHandle_t *)(&TFStorage_TCB));
	if(TFStorage_Ret == pdPASS) ;
	else
	{
		while(1) ;
	}
	
	
	
	vTaskStartScheduler();
}

//LEDTwink函数声明
BaseType_t LEDTwink_Ret;
UBaseType_t LEDTwink_Prio=2;
TaskHandle_t LEDTwink_TCB;

void LEDTwink(void *pvParameters)
{
	while(1)
	{
		HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
		vTaskDelay(100);
	}
}

//TFStorage函数声明
BaseType_t TFStorage_Ret;
UBaseType_t TFStorage_Prio=12;
TaskHandle_t TFStorage_TCB;

void TFStorage(void *pvParameters)
{
	TFRet = f_mount(&SDFatFS,SDPath,1);
	if(TFRet != FR_OK)
	{
		switch(TFRet)
		{
			case FR_NO_FILESYSTEM:
				TFRet = f_mkfs(SDPath,FM_FAT32,0,work,_MAX_SS);
				if(TFRet==FR_OK) 
				{
					TFRet = f_mount(&SDFatFS,SDPath,1);
					if(TFRet == FR_OK) ;
					else
					{
						while(1) ;
					}
				}
				break;
			default:
				while(1) ;
		}
	}
	else if(retSD == FR_OK) 
	{
		f_open(&SDFile,"TF test.txt",FA_WRITE|FA_OPEN_APPEND);
		f_printf(&SDFile,"TF write test!\n");
		f_close(&SDFile);
	}
	vTaskSuspend(NULL);
	while(1)
	{
			
	}
}
