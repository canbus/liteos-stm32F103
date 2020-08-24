
#include "los_sys.h"
#include "los_task.ph"
#include "los_memory.ph"
#include "los_queue.h"
#include "los_sem.h"

uint32_t vKeyHitTaskID,task2ID;

uint32_t semID;

void vKeyHitTask(uint32_t arg)
{
	uint32_t retVal;
	while(1)
	{
		LOS_TaskDelay(500);
		LOS_SemPost(semID);
		printf("sem post\n");
	}
}

void task2(uint32_t arg)
{
	uint32_t retVal;
	while(1)
	{
		retVal = LOS_SemPend(semID, LOS_WAIT_FOREVER);
		if(retVal != LOS_OK)
		{
			printf("LOS_SemPend fail\n");
		}
		printf("get sem\n");
	}
}

uint32_t taskCreate(void)
{
	uint32_t retVal;
	TSK_INIT_PARAM_S taskInitParam={0};
	
	taskInitParam.pcName ="vKeyHitTask";
	taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)&vKeyHitTask;
	taskInitParam.usTaskPrio = 5;
	taskInitParam.uwStackSize = 512;
	taskInitParam.uwArg = 100;
	retVal = LOS_TaskCreate(&vKeyHitTaskID,&taskInitParam);
	if(retVal != LOS_OK)
	{
		printf("LOS_TaskCreate() fail,%X\n", retVal);
	}
	taskInitParam.pcName ="task2";
	taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)&task2;
	taskInitParam.usTaskPrio = 4;
	taskInitParam.uwStackSize = 512;
	retVal = LOS_TaskCreate(&task2ID,&taskInitParam);
	if(retVal != LOS_OK)
	{
		printf("LOS_TaskCreate() fail,%X\n", retVal);
	}
	
	LOS_BinarySemCreate(0,&semID);
	
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
