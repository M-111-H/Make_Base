/*
 * @Descripttion:
 * @version:
 * @Author: ������
 * @Date: 2024-08-24 23:29:04
 * @LastEditors: ������
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

// �������ȼ�
#define START_TASK_PRIO 1
// �����ջ��С
#define START_STK_SIZE 128
// ������
TaskHandle_t StartTask_Handler;
// ������
void start_task(void);

// �������ȼ�
#define LED0_TASK_PRIO 2
// �����ջ��С
#define LED0_STK_SIZE 50
// ������
TaskHandle_t LED0Task_Handler;
// ������
void led0_task(void);

// �������ȼ�
#define LED1_TASK_PRIO 3
// �����ջ��С
#define LED1_STK_SIZE 50
// ������
TaskHandle_t LED1Task_Handler;
// ������
void led1_task(void);
/************************************************
 �����ʵ��
*************************************************/

int main(void)
{
	delay_init();									// ��ʱ������ʼ��
	LED_Init();										// ��ʼ����LED���ӵ�Ӳ���ӿ�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); // ����ϵͳ�ж����ȼ�����4
	uart_init(115200);								// ��ʼ������
	// ������ʼ����
	xTaskCreate((TaskFunction_t)start_task,			 // ������
				(const char *)"start_task",			 // ��������
				(uint16_t)START_STK_SIZE,			 // �����ջ��С
				(void *)NULL,						 // ���ݸ��������Ĳ���
				(UBaseType_t)START_TASK_PRIO,		 // �������ȼ�
				(TaskHandle_t *)&StartTask_Handler); // ������

	vTaskStartScheduler();
}

// ��ʼ����������
void start_task(void)
{
	taskENTER_CRITICAL(); // �����ٽ���
	// ����LED0����
	xTaskCreate((TaskFunction_t)led0_task,
				(const char *)"led0_task",
				(uint16_t)LED0_STK_SIZE,
				(void *)NULL,
				(UBaseType_t)LED0_TASK_PRIO,
				(TaskHandle_t *)&LED0Task_Handler);
	// ����LED1����
	xTaskCreate((TaskFunction_t)led1_task,
				(const char *)"led1_task",
				(uint16_t)LED1_STK_SIZE,
				(void *)NULL,
				(UBaseType_t)LED1_TASK_PRIO,
				(TaskHandle_t *)&LED1Task_Handler);

	vTaskDelete(StartTask_Handler); // ɾ����ʼ����
	taskEXIT_CRITICAL(); // �˳��ٽ���
}

// LED0������
void led0_task(void)
{
	while (1)
	{
		LED0 = ~LED0;
		vTaskDelay(200);
	}
}

// LED1������
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
