
#include "los_sys.h"
#include "los_task.ph"
#include "los_memory.ph"
#include "los_queue.h"
#include "los_sem.h"

typedef struct{
	uint32_t handleID;
	BOOL isUsed;
}ResHandler;

static ResHandler sResHandler[3] = {
	{100,FALSE},{200,FALSE},{300,FALSE}
};

static uint32_t sSemID;

uint32_t tasksID;

static void * tasks(uint32_t arg)
{
	uint32_t retVal, i;
	printf("tasks(%d) should be Pending.\r\n",arg);
	retVal = LOS_SemPend(sSemID, LOS_WAIT_FOREVER);
	if(retVal == LOS_OK){
		for(i = 0; i< 3; i++){
			if(sResHandler[i].isUsed == FALSE){
				sResHandler[i].isUsed = TRUE;
				break;
			}
		}
		printf("I am(%d) wordking on it(%d)\r\n", arg, sResHandler[i].handleID);
		LOS_TaskDelay(1000);
		
		sResHandler[i].isUsed = FALSE;
		LOS_SemPost(sSemID);
	}
	printf("tasks(%d) finished\r\n",arg);
	return LOS_OK;
}


uint32_t taskCreate(void)
{
	uint32_t retVal,i;
	TSK_INIT_PARAM_S taskInitParam={0};
	
	retVal = LOS_SemCreate(3,&sSemID);//创建一个多值信号量
	if(retVal != LOS_OK){
		printf("LOS_SemCreate Failed\n");
		return LOS_NOK;
	}
	
	for(i=0; i < 10; i++){
		taskInitParam.pcName ="tasks";
		taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)&tasks;
		taskInitParam.usTaskPrio = 5;
		taskInitParam.uwStackSize = 512;
		taskInitParam.uwArg = i;
		retVal = LOS_TaskCreate(&tasksID,&taskInitParam);
		if(retVal != LOS_OK)
		{
			printf("LOS_TaskCreate() fail,%X\n", retVal);
		}
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
