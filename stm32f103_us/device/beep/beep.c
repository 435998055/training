/**
 ********************************************************
 * @file           : beep.c
 * @details        : The file provides code for the configuration
                      of the  beep.
 * @author         : weijianx
 * @email          : 3176184465@qq.com
 * @date           : 2020-05-19
 * @version        : V1.0
 ********************************************************
*/

#include "beep.h"


/**
 * @brief       ������ IO��ʼ��
 * @retval       None
 */
void Beep_Init(void)
{

    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOA_CLK_ENABLE();           	//����GPIOAʱ��

	
    GPIO_Initure.Pin=GPIO_PIN_5; 				//PB8
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//�������
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);

	
}

/**
 * @brief       ����������
 * @retval       None
 */
void BeepOn(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
}

/**
 * @brief       �رշ�����
 * @retval       None
 */
void BeepOff(void)
{

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
}


