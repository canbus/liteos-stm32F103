
#include "los_sys.h"
#include "los_task.ph"
#include "los_memory.ph"
#include "los_queue.h"
#include "los_swtmr.h"

uint32_t taskSendID,taskRevID;
uint32_t queueID;

struct DATA{
	uint32_t id;
	char *msg;
};

struct DATA data1={100,"task1 data"};

uint16_t timer1ID,timer2ID;
void time1Handle(uint32_t arg)
{
	printf("time over %d\n",arg);
}

void taskSend(uint32_t arg)
{
	uint32_t i,pri,retVal;
	//一次性定时器
	retVal = LOS_SwtmrCreate(100, LOS_SWTMR_MODE_ONCE, time1Handle, &timer1ID, 10 ,OS_SWTMR_ROUSES_IGNORE, OS_SWTMR_ALIGN_INSENSITIVE);
	if(retVal != LOS_OK)
	{
		printf("create timer fail\n");
	}
	LOS_SwtmrStart(timer1ID);
	//重复定时器
	retVal = LOS_SwtmrCreate(500, LOS_SWTMR_MODE_PERIOD, time1Handle, &timer2ID, 20 ,OS_SWTMR_ROUSES_IGNORE, OS_SWTMR_ALIGN_INSENSITIVE);
	if(retVal != LOS_OK)
	{
		printf("create timer fail\n");
	}
	LOS_SwtmrStart(timer2ID);
	while(1)
	{
		LOS_TaskDelay(200);
//		if(arg == data1.id)
//			LOS_QueueWriteCopy(queueID, &data1, sizeof(struct DATA), 0);
//		printf("write queue %d\n", arg);
	}
}

void taskRev(void)
{
	uint32_t size,retVal;
	struct DATA queuedata;
	while(1)
	{
//		retVal = LOS_QueueReadCopy(queueID, &queuedata, &size, 0);
//		if(retVal == LOS_OK){
//			printf("receive:%d,%s\n", queuedata.id, queuedata.msg);
//		}
		LOS_TaskDelay(10);
	}
}

uint32_t taskCreate(void)
{
	uint32_t retVal;
	TSK_INIT_PARAM_S taskInitParam;
	
	retVal = LOS_QueueCreate("queue", 5, &queueID, 0,sizeof(struct DATA));
	
	taskInitParam.pcName ="task1";
	taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)&taskSend;
	taskInitParam.usTaskPrio = 4;
	taskInitParam.uwStackSize = 512;
	taskInitParam.uwArg = 100;
	retVal = LOS_TaskCreate(&taskSendID,&taskInitParam);
	if(retVal != LOS_OK)
	{
		printf("LOS_TaskCreate() fail,%X\n", retVal);
	}
	
	
	taskInitParam.pcName ="taskRev";
	taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)&taskRev;
	taskInitParam.usTaskPrio = 3;
	taskInitParam.uwStackSize = 512;
	retVal = LOS_TaskCreate(&taskRevID,&taskInitParam);
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
