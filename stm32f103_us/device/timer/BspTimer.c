/**
 ********************************************************
 * @file           : BspTimer.c
 * @brief          : Project file name BspTimer.c
 * @details        : The file provides code for the configuration of the tim2.
 * @version        : V1.0
 ********************************************************
*/


#include "config.h"
#include "BspTimer.h"

//��ʱ��2���
TIM_HandleTypeDef TIM_TimeBaseStructure;

/**
 *@brief      ͨ�ö�ʱ��3�жϳ�ʼ��
 *@param      prescaler���Զ���װֵ��
 *@param      period��ʱ��Ԥ��Ƶ��
 *@retval     None
 */
void Tim2ModeInit(int prescaler,int period)
{
	
	TIM_TimeBaseStructure.Instance = TIM2;									//���½�Timer����Ϊȱʡֵ
//	TIM_InternalClockConfig(TIM2);							//�����ڲ�ʱ�Ӹ�TIM2�ṩʱ��Դ
	TIM_TimeBaseStructure.Init.Prescaler = (uint16_t)(prescaler-1);//����prescaler=7200;			//Ԥ��Ƶϵ��Ϊ7200-1������������ʱ��Ϊ72MHz/7200 = 10kHz��ע�⣺Ԥ��Ƶϵ��ȡֵ��ΧΪ1-65535
	TIM_TimeBaseStructure.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;	//����ʱ�ӷָ�
	TIM_TimeBaseStructure.Init.CounterMode = TIM_COUNTERMODE_UP;	//���ü�����ģʽΪ���ϼ���ģʽ
	TIM_TimeBaseStructure.Init.Period = (period<2)?1:period-1;		//���ü��������С��ÿ��period�����Ͳ���һ�������¼�
	HAL_TIM_Base_Init(&TIM_TimeBaseStructure);			//������Ӧ�õ�TIM2��
//	TIM_ARRPreloadConfig(TIM2, DISABLE);					//��ֹARRԤװ�ػ�����
	HAL_TIM_Base_Start_IT(&TIM_TimeBaseStructure); //ʹ�ܶ�ʱ��2�Ͷ�ʱ��2�����жϣ�TIM_IT_UPDATE 
	
}

/**
 *@brief    ��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
 *@retval   None
 */
void Tim2NvicInit(void)
{
	__HAL_RCC_TIM2_CLK_ENABLE();            //ʹ��TIM2ʱ��
	HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

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
	

//  TIM_ClearFlag(TIM2, TIM_FLAG_Update);					//�������жϱ�־
//	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);				//����TIM2���ж�
//	TIM_Cmd(TIM2,ENABLE);
	
	__HAL_TIM_CLEAR_FLAG(&TIM_TimeBaseStructure, TIM_FLAG_UPDATE);
	__HAL_TIM_ENABLE_IT(&TIM_TimeBaseStructure, TIM_IT_UPDATE);
	__HAL_TIM_ENABLE(&TIM_TimeBaseStructure);
	
}

/**
 *@brief       ��ʱ��2�жϷ�����
 *@retval      None
 */
void  TIM2_IRQHandler(void)
{


	if ( __HAL_TIM_GET_IT_SOURCE(&TIM_TimeBaseStructure, TIM_IT_UPDATE) != RESET ) 
	{	
		//----------------------------------------------------------
		//����û����� Add User Code
		
		ButtonProj();//������ʱɨ��


		//---------------------------------------------------------
//		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);  
		__HAL_TIM_CLEAR_IT(&TIM_TimeBaseStructure, TIM_IT_UPDATE);
	}		 	
}



