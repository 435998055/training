/**
 *****************************************************
 * @file           : key.c
 * @brief          : The file provides code for the 
                     configuration of the keys.
 * @author         : weijianx
 * @email          : 3176184465@qq.com
 * @date           : 2020-05-19
 * @version        : V1.0
 *****************************************************
*/

#include "key.h"
#include "delay.h"

/** 
 * @brief      ������ʼ������
 * @retval     None
 */
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();           //����GPIOAʱ��
    __HAL_RCC_GPIOC_CLK_ENABLE();           //����GPIOCʱ��

    
    GPIO_Initure.Pin=GPIO_PIN_0;            //PA0
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
	GPIO_Initure.Pin=GPIO_PIN_15; 			//PA15
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
	GPIO_Initure.Pin=GPIO_PIN_5; 			//PC5
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
}

/** 
 * @brief     ����������
 * @return    ���ذ���ֵ
 * @para      mode:0,��֧��������;1,֧��������;
 * @return     �Ƿ��а�������
 *       @retval  0 �ް�������
 *       @retval  1 WKUP���� WK_UP
 * @attention     ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>WK_UP!!
 */
u8 KEY_Scan(u8 mode)
{
    static u8 key_up=1;     //�����ɿ���־
    if(mode==1)key_up=1;    //֧������
    if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
    {
        delay_ms(10);
        key_up=0;
        if(KEY0==0)       return KEY0_PRES;
        else if(KEY1==0)  return KEY1_PRES;
        else if(WK_UP==1) return WKUP_PRES;          
    }else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1;
    return 0;   //�ް�������
}
