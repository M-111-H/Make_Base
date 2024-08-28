/*
 * @Descripttion:
 * @version:
 * @Author: 王明红
 * @Date: 2024-08-24 23:29:04
 * @LastEditors: 王明红
 * @LastEditTime: 2024-08-28 09:54:44
 */
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "misc.h"
#include <stdio.h>
#include <projdefs.h>

// 任务优先级
#define START_TASK_PRIO 1
// 任务堆栈大小
#define START_STK_SIZE 128
// 任务句柄
TaskHandle_t StartTask_Handler;
// 任务函数
void start_task(void);

// 任务优先级
#define LED0_TASK_PRIO 2
// 任务堆栈大小
#define LED0_STK_SIZE 50
// 任务句柄
TaskHandle_t LED0Task_Handler;
// 任务函数
void led0_task(void);

// 任务优先级
#define LED1_TASK_PRIO 3
// 任务堆栈大小
#define LED1_STK_SIZE 50
// 任务句柄
TaskHandle_t LED1Task_Handler;
// 任务函数
void led1_task(void);
/************************************************
 跑马灯实验
*************************************************/

int main(void)
{
	delay_init();									// 延时函数初始化
	LED_Init();										// 初始化与LED连接的硬件接口
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); // 设置系统中断优先级分组4
	uart_init(115200);								// 初始化串口
	// 创建开始任务
	xTaskCreate((TaskFunction_t)start_task,			 // 任务函数
				(const char *)"start_task",			 // 任务名称
				(uint16_t)START_STK_SIZE,			 // 任务堆栈大小
				(void *)NULL,						 // 传递给任务函数的参数
				(UBaseType_t)START_TASK_PRIO,		 // 任务优先级
				(TaskHandle_t *)&StartTask_Handler); // 任务句柄

	vTaskStartScheduler();
}

// 开始任务任务函数
void start_task(void)
{
	taskENTER_CRITICAL(); // 进入临界区
	// 创建LED0任务
	xTaskCreate((TaskFunction_t)led0_task,
				(const char *)"led0_task",
				(uint16_t)LED0_STK_SIZE,
				(void *)NULL,
				(UBaseType_t)LED0_TASK_PRIO,
				(TaskHandle_t *)&LED0Task_Handler);
	// 创建LED1任务
	xTaskCreate((TaskFunction_t)led1_task,
				(const char *)"led1_task",
				(uint16_t)LED1_STK_SIZE,
				(void *)NULL,
				(UBaseType_t)LED1_TASK_PRIO,
				(TaskHandle_t *)&LED1Task_Handler);

	vTaskDelete(StartTask_Handler); // 删除开始任务
	taskEXIT_CRITICAL(); // 退出临界区
}

// LED0任务函数
void led0_task(void)
{
	while (1)
	{
		LED0 = ~LED0;
		vTaskDelay(200);
	}
}

// LED1任务函数
void led1_task(void)
{
	while (1)
	{
		LED1 = 0;
		vTaskDelay(100);
		LED1 = 1;
		vTaskDelay(100);
	}
}
