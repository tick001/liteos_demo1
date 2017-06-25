#include "stdio.h"
#include "stdlib.h"

#include "los_sys.h"
#include "los_tick.h"
#include "los_task.ph"
#include "los_config.h"

#include "los_bsp_led.h"
#include "los_bsp_key.h"
#include "los_bsp_uart.h"
#include "stm32f10x.h"

#include <string.h>

UINT32 g_TestTskId;

VOID Led_Task(VOID)
{
	UINT32 uwRet = LOS_OK;
	UINT8  cmd = LED_OFF;
//	LOS_TaskDelay(1000);
	while(1)
	{
		//·­×ªled
		if(cmd == LED_OFF)
			cmd = LED_ON;
		else
			cmd = LED_OFF;
		LOS_EvbLedControl(LOS_LED1, cmd);
		uwRet = LOS_TaskDelay(500);
		if(uwRet != LOS_OK)
			return;
	}
}

#define LOSCFG_LED_TSK_STACK_SIZE	0x8D0
UINT32 Led_CreateTask(VOID)
{
	UINT32 uwRet;
	TSK_INIT_PARAM_S taskInitParam;
	taskInitParam.usTaskPrio = 0;
	taskInitParam.pcName = "Led_Task";
	taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)Led_Task;
	taskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
	taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskId, &taskInitParam);
	
	return uwRet;
}

VOID hardWare()
{

}

UINT32 osAppInit(VOID)
{
	UINT32 uwRet;
	ErrorStatus hwRet;
	
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_ON);
	hwRet = RCC_WaitForHSEStartUp();
	if(hwRet != SUCCESS)
	{
		return ERROR;
	}
	
	
	LOS_EvbLedInit();
	uwRet = Led_CreateTask();
		
	return uwRet;
}

int main(VOID)
{
	UINT32 uwRet;
	
	uwRet = osMain();
	if(uwRet != LOS_OK)
		return LOS_NOK;
	
	osAppInit();
	LOS_Start();
	
    for (;;);
    /* Replace the dots (...) with your own code. */
}

