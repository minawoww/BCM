
#include "avr_reg.h"
#include "STD_Types.h"

#ifndef BCM_H_
#define BCM_H_

#define LOCKED	 1
#define UNLOCKED 0

#define BCM_ID 0xA5

typedef enum { SendInProcess , NullPtr , BCM_ok }EnumBCMError_t;
typedef enum { Idle , SendingByte , SendingByteComplete , SendingFrameComplete }EnumBCMState_t;

typedef struct 
{
	uint8 Module_ID;
	uint8 *PtrData;
	uint8 CheckSum;
	uint16 DataSize;
	
}StrBCMInternalBuffer_t;

void BCM_Init(void);
EnumBCMError_t BCM_Send(uint8 *Data_Buffer , uint16 Buffer_Size);
EnumBCMError_t BCM_DispatcherTx(void);


#endif