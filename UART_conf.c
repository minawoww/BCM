#include"UART_conf.h"

/*
{
    uint8 TranRecMode;
    uint8 BaudRate;
    uint8 FrameSize;
    uint8 InterruptMode;
}
*/

UART_Conf_t UART_Array_Conf[Num_UART] =  { {TranRecMode_Tran_Rec, BaudRate_9600, FrameSize_8, InterruptMode_On }};
