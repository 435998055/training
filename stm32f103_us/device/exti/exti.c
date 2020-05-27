#include "exti.h"
#include "delay.h"
#include "led.h"
#include "key.h"


//�ⲿ�жϳ�ʼ��
void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();               //����GPIOAʱ��
    __HAL_RCC_GPIOC_CLK_ENABLE();               //����GPIOCʱ��
    
    GPIO_Initure.Pin=GPIO_PIN_0;                //PA0
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;      //�����ش���
    GPIO_Initure.Pull=GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
    GPIO_Initure.Pin=GPIO_PIN_15; 				//PA15
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;     //�½��ش���
    GPIO_Initure.Pull=GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
	GPIO_Initure.Pin=GPIO_PIN_5 | GPIO_PIN_4; 				//PC5
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
    
    //�ж���0-PA0
    HAL_NVIC_SetPriority(EXTI0_IRQn,2,3);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ3
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);             //ʹ���ж���0
    
	//�ж���5-PC5
    HAL_NVIC_SetPriority(EXTI9_5_IRQn,2,2);   	//��ռ���ȼ�Ϊ2�������ȼ�Ϊ2
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn); 
	
	//�ж���4-PC4
    HAL_NVIC_SetPriority(EXTI4_IRQn,2,1);   	//��ռ���ȼ�Ϊ2�������ȼ�Ϊ1
    HAL_NVIC_EnableIRQ(EXTI4_IRQn); 
	
    //�ж���15-PA15
    HAL_NVIC_SetPriority(EXTI15_10_IRQn,2,0); 	//��ռ���ȼ�Ϊ2�������ȼ�Ϊ0
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);        	//ʹ���ж���2
}


//void ExtiModeInit(void)
//{
//	EXTI_InitTypeDef  EXTI_InitStructure;
//	
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4);
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//  EXTI_InitStructure.EXTI_Line = EXTI_Line4;
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);
//	
//	
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//  EXTI_InitStructure.EXTI_Line = EXTI_Line5;
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);
//	
//}

//�жϷ�����
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);		//�����жϴ����ú���
}

void EXTI4_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);		//�����жϴ����ú���
	
//	if(__HAL_GPIO_EXTI_GET_FLAG(GPIO_PIN_4) == SET)
//		__HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_4);
}

void EXTI9_5_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);		//�����жϴ����ú���
}

void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);		//�����жϴ����ú���
}


//�жϷ����������Ҫ��������
//��HAL�������е��ⲿ�жϷ�����������ô˺���
//GPIO_Pin:�ж����ź�
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//    delay_ms(100);      	//����
//    switch(GPIO_Pin)
//    {
//        case GPIO_PIN_0:
//            if(WK_UP==1) 
//            {
//				LED1=!LED1;	//����LED0,LED1�������
//				LED0=!LED1;
//            }
//            break;
//        case GPIO_PIN_5:
//            if(KEY0==0)  	//����LED0��ת 
//            {
//                LED0=!LED0;
//            }
//            break;
//        case GPIO_PIN_15:
//            if(KEY1==0)  
//            {
//				LED1=!LED1;	//����LED0��ת
//            }
//            break;
//    }
//}
