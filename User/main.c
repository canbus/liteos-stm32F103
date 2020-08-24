
#include "los_sys.h"
#include "los_task.ph"
#include "los_memory.ph"
#include "los_queue.h"
#include "los_swtmr.h"

enum LIGHT_TYPE{
	LIGHT_ON = 0,
	LIGHT_OFF 
}LIGHT_TYPE ;

uint32_t vKeyHitTaskID;
enum LIGHT_TYPE backLightState = LIGHT_OFF;
uint16_t timerID;
void timeHandle(uint32_t arg)
{
	backLightState = LIGHT_OFF;
	printf("time over:light off\n");
}

void vKeyHitTask(uint32_t arg)
{
	uint32_t retVal;
	while(1)
	{
		LOS_TaskDelay(500);
		if(backLightState == LIGHT_OFF)
		{
			backLightState = LIGHT_ON;
			printf("time over:light on\n");
			//ÖØ¸´¶¨Ê±Æ÷
			retVal = LOS_SwtmrCreate(5000, LOS_SWTMR_MODE_ONCE, timeHandle, &timerID, 20 ,
															OS_SWTMR_ROUSES_IGNORE, OS_SWTMR_ALIGN_INSENSITIVE);
			if(retVal != LOS_OK)
			{
				printf("create timer fail\n");
			}
			LOS_SwtmrStart(timerID);
		}

	}
}


uint32_t taskCreate(void)
{
	uint32_t retVal;
	TSK_INIT_PARAM_S taskInitParam={0};
	
	taskInitParam.pcName ="vKeyHitTask";
	taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)&vKeyHitTask;
	taskInitParam.usTaskPrio = 4;
	taskInitParam.uwStackSize = 512;
	taskInitParam.uwArg = 100;
	retVal = LOS_TaskCreate(&vKeyHitTaskID,&taskInitParam);
	if(retVal != LOS_OK)
	{
		printf("LOS_TaskCreate() fail,%X\n", retVal);
	}
	
	return LOS_OK;
}

int32_t main(void)
{
	uint32_t retVal;
	
	//initGPIO();
	initUart();
	
	retVal = LOS_KernelInit();
	if(retVal != LOS_OK)
	{
		printf("LOS_KernelInit() fail,%X\n", retVal);
	}
	retVal = taskCreate();
	if(retVal != LOS_OK)
	{
		printf("taskCreate() fail,%X\n", retVal);
	}
	LOS_Start();
}
