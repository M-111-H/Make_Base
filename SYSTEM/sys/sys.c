/*
 * @Descripttion: 
 * @version: 
 * @Author: 王明红
 * @Date: 2024-08-24 23:29:04
 * @LastEditors: 王明红
 * @LastEditTime: 2024-08-26 10:36:25
 */
/*
 * @Descripttion: 
 * @version: 
 * @Author: 王明红
 * @Date: 2024-08-24 23:29:04
 * @LastEditors: 王明红
 * @LastEditTime: 2024-08-24 23:48:57
 */
#include "sys.h"

//********************************************************************************  
//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//关闭所有中断
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//设置栈顶地址
//addr:栈顶地址
void MSR_MSP(u32 addr) 
{
__ASM volatile("MSR MSP, r0");
__ASM volatile("BX r14");
}
