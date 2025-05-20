#include "telemetry.h"

uint8_t TelemetrySendBuff[100];



void TelemetrySend(void)
{
	sprintf(TelemetrySendBuff,"Roll:%0.2f Pitch:%0.2f \r\n",AHRSData.Roll,AHRSData.Pitch);
	HAL_UART_Transmit_DMA(&huart8,TelemetrySendBuff,sizeof(TelemetrySendBuff));
}
//#include "telemetry.h"

//uint8_t TelemetrySendBuff[100];

//void TelemetrySend(void)
//{
//	sprintf(TelemetrySendBuff,"p:%0.1f r:%0.1f y:%0.1f x:%0.1f y:%0.1f z:%0.1f\r\n",1.0f,2.0f,3.0f,4.0f,5.0f,6.0f);
//	HAL_UART_Transmit_DMA(&huart8,TelemetrySendBuff,sizeof(TelemetrySendBuff));
//}
