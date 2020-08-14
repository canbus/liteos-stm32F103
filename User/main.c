#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "los_sys.h"
#include "los_task.ph"
#include "los_memory.ph"

#define LED1_ON() 	{GPIO_ResetBits(GPIOB, GPIO_Pin_10);}
#define LED1_OFF() 	{GPIO_SetBits(GPIOB, GPIO_Pin_10);}
#define LED2_ON() 	{GPIO_ResetBits(GPIOB, GPIO_Pin_11);}
#define LED2_OFF() 	{GPIO_SetBits(GPIOB, GPIO_Pin_11);}

static void Led1Task(void)
{
    while(1)
    {
        LED1_ON();
        LOS_TaskDelay(1000);
        LED1_OFF();
        LOS_TaskDelay(1000);
    }
}
static void Led2Task(void)
{
    while(1)
    {
        LED2_ON();
        LOS_TaskDelay(200);
        LED2_OFF();
        LOS_TaskDelay(200);
    }
}
UINT32 RX_Task_Handle;
UINT32 TX_Task_Handle;
static UINT32 AppTaskCreate(void)
{
    UINT32 uwRet = LOS_OK;

    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 4;
    task_init_param.pcName = "RxTask";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)Led1Task;
    task_init_param.uwStackSize = 512;
    uwRet = LOS_TaskCreate(&RX_Task_Handle, &task_init_param);
    if (uwRet != LOS_OK)
    {
        printf("Led1Task create failed,%X\n", uwRet);
        return uwRet;
    }

    task_init_param.usTaskPrio = 4;
    task_init_param.pcName = "TxTask";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)Led2Task;
    task_init_param.uwStackSize = 512;
    uwRet = LOS_TaskCreate(&TX_Task_Handle, &task_init_param);
    if (uwRet != LOS_OK)
    {
        printf("Led2Task create failed,%X\n", uwRet);
        return uwRet;
    }
    return LOS_OK;
}
void initGPIO(void)
{
    GPIO_InitTypeDef initDef;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    initDef.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    initDef.GPIO_Mode = GPIO_Mode_Out_PP;
    initDef.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &initDef);
}
int main()
{
    UINT32 uwRet = LOS_OK;
		initGPIO();
    LOS_KernelInit();
    uwRet = AppTaskCreate();
    if(uwRet != LOS_OK)
    {
        printf("LOS Creat task failed\r\n");
    }
    LOS_Start();
}
