/*
 * @Descripttion: 
 * @version: 
 * @Author: ������
 * @Date: 2024-08-24 23:29:04
 * @LastEditors: ������
 * @LastEditTime: 2024-08-26 10:36:25
 */
/*
 * @Descripttion: 
 * @version: 
 * @Author: ������
 * @Date: 2024-08-24 23:29:04
 * @LastEditors: ������
 * @LastEditTime: 2024-08-24 23:48:57
 */
#include "sys.h"

//********************************************************************************  
//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//�ر������ж�
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//���������ж�
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//����ջ����ַ
//addr:ջ����ַ
void MSR_MSP(u32 addr) 
{
__ASM volatile("MSR MSP, r0");
__ASM volatile("BX r14");
}
