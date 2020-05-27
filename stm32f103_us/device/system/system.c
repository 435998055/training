/**
 ********************************************************
 * @file           : system.c
 * @brief          : The file describes the execution of 
                     various tasks of the system
 * @author         : weijianx
 * @email          : 3176184465@qq.com
 * @date           : 2020-05-19
 * @version        : V1.0
 ********************************************************
*/

#include "config.h"
#include "system.h"



/** 
 * @brief      CPU�������
 * @retval     None
 */
void CpuInit(void)
{
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//ѡ���2���ж����ȼ�����2λ�����ȼ���2λ�����ȼ�
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);     //�������ȼ�Ϊ����2
}

/** 
 * @brief        ϵͳ��ʼ��
 * @retval       None
 */
void SysInit(void)
{
	
	CpuInit();//����ϵͳ��Ϣ
	
	
	//����������ʼ��
	SysTickInit();//ϵͳ�δ�ʱ����ʼ��
	
	HAL_Init();                    	 	    //��ʼ��HAL��    
	delay_init(72);               		    //��ʼ����ʱ����
	uart_init(115200);					    //��ʼ������
	LED_Init();							    //��ʼ��LED	
	KEY_Init();							    //��ʼ������
	Beep_Init();                            //��ʼ��������
	ADC_Init();                             //��ʼ��ADC
	EXTI_Init();                            //��ʼ���ж�
	Tim2Init(72,1000);                      //�ж�����Ϊ1ms,���ڰ���ɨ��
//	MX_ADC1_Init();
	MX_DMA_Init();

	
}



