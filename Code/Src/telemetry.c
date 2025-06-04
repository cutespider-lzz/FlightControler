#include "telemetry.h"

uint8_t TelemetrySendBuff[100];



void TelemetrySend(void)
{
	sprintf(TelemetrySendBuff,"Roll:%0.2f Pitch:%0.2f Heading:%0.2f Longitude:%0.5f Latitude:%0.5f Height:%0.2f Location_Down:%0.2f Location_East:%0.2f Location_North:%0.2f V_Down:%0.2f V_East:%0.5f V_North:%0.2f \r\n",AHRSData.Roll,AHRSData.Pitch,AHRSData.Heading,Geodetic_Position_data.Longitude,Geodetic_Position_data.Latitude,Geodetic_Position_data.Height,INSGPSData.Location_Down,INSGPSData.Location_East,INSGPSData.Location_North,INSGPSData.Velocity_Down,INSGPSData.Velocity_East,INSGPSData.Velocity_North);
	HAL_UART_Transmit_DMA(&huart8,TelemetrySendBuff,sizeof(TelemetrySendBuff));

}
