#include "config.h"
#include "BspTimer.h"



void Tim2ModeInit(int prescaler,int period)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_DeInit(TIM2);										//���½�Timer����Ϊȱʡֵ
	TIM_InternalClockConfig(TIM2);							//�����ڲ�ʱ�Ӹ�TIM2�ṩʱ��Դ
	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t)(prescaler-1);//����prescaler=7200;			//Ԥ��Ƶϵ��Ϊ7200-1������������ʱ��Ϊ72MHz/7200 = 10kHz��ע�⣺Ԥ��Ƶϵ��ȡֵ��ΧΪ1-65535
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ü�����ģʽΪ���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period = (period<2)?1:period-1;		//���ü��������С��ÿ��period�����Ͳ���һ�������¼�
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);			//������Ӧ�õ�TIM2��
	TIM_ARRPreloadConfig(TIM2, DISABLE);					//��ֹARRԤװ�ػ�����
}
void Tim2NvicInit(void)
{
	
	NVIC_InitTypeDef NVIC_InitStructure;
	// ����TIM�ж�	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;      	// ѡ��TIM2���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	// ��ռʽ�ж����ȼ�����Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		// ��Ӧʽ�ж����ȼ�����Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			// �ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);							// �����ж�
}

/*******************************************************************************************************
** ����: Tim2Init, Tim��ʱ����ʱ����
**------------------------------------------------------------------------------------------------------
** ����: prescaler Ԥ��Ƶϵ��
         period  ��������
Example:
����
	TIM_Period ��Auto Reload Register(ARR) = 1000  , TIM_Prescaler--71 
���ж�����Ϊ = 1/(72MHZ /72) * 1000 = 1ms

	TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> �ж� ��TIMxCNT����Ϊ0���¼��� 
        
** ����: void
********************************************************************************************************/
void Tim2Init(int prescaler,int period)
{
  Tim2ModeInit(prescaler,period);
  Tim2NvicInit();
	

  TIM_ClearFlag(TIM2, TIM_FLAG_Update);					//�������жϱ�־
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);				//����TIM2���ж�
	TIM_Cmd(TIM2,ENABLE);
	
}


void  TIM2_IRQHandler(void)
{


	if ( TIM_GetITStatus( TIM2, TIM_IT_Update) != RESET ) 
	{	
		//----------------------------------------------------------
		//����û����� Add User Code
		
     ButtonProj();//������ʱɨ��

			
		

		//---------------------------------------------------------
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);  		 
	}		 	
}



