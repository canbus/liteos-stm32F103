
#include "los_sys.h"
#include "los_task.ph"
#include "los_memory.ph"

void task1(void)
{
	while(1)
	{
		LOS_TaskDelay(200);
		printf("task1 run\r\n");
	}
}

void task2(void)
{
	while(1)
	{
		LOS_TaskDelay(100);
		printf("task2 run\r\n");
	}
}

uint32_t taskCreate(void)
{
	uint32_t retVal;
	uint32_t task1ID,task2ID;
	TSK_INIT_PARAM_S taskInitParam;
	
	taskInitParam.pcName ="task1";
	taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)&task1;
	taskInitParam.usTaskPrio = 4;
	taskInitParam.uwStackSize = 512;
	retVal = LOS_TaskCreate(&task1ID,&taskInitParam);
	if(retVal != LOS_OK)
	{
		printf("LOS_TaskCreate() fail,%X\n", retVal);
	}
	
	taskInitParam.pcName ="task2";
	taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)&task2;
	taskInitParam.usTaskPrio = 5;
	taskInitParam.uwStackSize = 512;
	retVal = LOS_TaskCreate(&task2ID,&taskInitParam);
	if(retVal != LOS_OK)
	{
		printf("LOS_TaskCreate() fail,%X\n", retVal);
	}
	return LOS_OK;
}

int32_t main(void)
{
	uint32_t retVal;
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
