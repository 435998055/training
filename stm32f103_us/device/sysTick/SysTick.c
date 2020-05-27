#include "config.h"
#include "SysTick.h"

//uint32 sysTimer[MAX_TIMER];//�����ʱ������
//static uint32 g_iRunTimer = 0;//����ϵͳȫ������ʱ��



void SysTickInit(void)
{

	Stm32_Clock_Init(RCC_PLL_MUL9); 
//	SysTick_Config(72000000/1000);//ÿ��1ms��һ��ϵͳ�δ��ж���Ӧ��������Ƶ72M
	
}

void rt_hw_us_delay(rt_uint32_t us)
{
    rt_uint32_t delta;
    rt_uint32_t current_delay;

    /* �����ʱ������tick�� */
    us = us * (SysTick->LOAD/(1000000/RT_TICK_PER_SECOND));

    /* ��õ�ǰʱ�� */
    delta = SysTick->VAL;

    /* ѭ����õ�ǰʱ�䣬ֱ���ﵽָ����ʱ����˳�ѭ�� */
    do
    {
			if ( delta > SysTick->VAL )
				current_delay = delta - SysTick->VAL;
			else
        /* ��ʱ��Խ��һ��OS tick�ı߽�ʱ�Ĵ��� */
				current_delay = SysTick->LOAD + delta - SysTick->VAL;
    } while( current_delay < us );
}


#if 0
void rt_hw_us_delay(rt_uint32_t us)
{
    rt_uint32_t delta;
    /* �����ʱ������ tick �� */
    us = us * (SysTick->LOAD/(1000000/RT_TICK_PER_SECOND));
    /* ��õ�ǰʱ�� */
    delta = SysTick->VAL;
    /* ѭ����õ�ǰʱ�䣬ֱ���ﵽָ����ʱ����˳�ѭ�� */
    while (delta - SysTick->VAL< us);
}
#endif



