#include "telemetry.h"

uint8_t TelemetrySendBuff[100];

void TelemetrySend(void)
{
	sprintf(TelemetrySendBuff,"p:%0.1f r:%0.1f y:%0.1f x:%0.1f y:%0.1f z:%0.1f\r\n",1,2,3,4,5,6);
	HAL_UART_Transmit_DMA(&huart8,TelemetrySendBuff,sizeof(TelemetrySendBuff));
}
