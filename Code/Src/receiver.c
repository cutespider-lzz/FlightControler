#include "receiver.h"
#include "siyi.h"

uint8_t ReceiverReceiveBuff[25];//Receiver接收缓存数组
uint8_t ReceiverFifoBuff[25];//Receiver数据处理数组，缓存数组接收数据后保存至处理数组中

uint16_t ReceiverChannel[16];//Receiver接收通道数据
uint16_t ReceiverChannelPrevious[16]={1500,1500,1000,1500,1000,1000,1000,1500,1500,1500,1500,1500,1500,1500,1500,1500};//Receiver接收通道之前数据
uint16_t ReceiverChannelNeutral[16] = {1500,1500,1000,1500,1000,1000,1000,1500,1500,1500,1500,1500,1500,1500,1500,1500};//Receiver接收通道中立数据

SemaphoreHandle_t ReceiverSemaphore=NULL;//Receiver二值信号量
BaseType_t ReceiverHigherTaskSwitch;
