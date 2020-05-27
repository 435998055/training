/*USER CODE BEGIN Header*/
/**
 ********************************************************
 * @file           : adc11.c
 * @brief          : Project file name adc11.c
 * @details        : The file provides code for the configuration 
                      of the adc1.
 * @date           : 2020-05-27
 * @version        : V1.0
 ********************************************************
*/


#include "adc11.h"

#if ADC1_EN == 1

__IO uint16 ADCConvertedValue[NbrOfChannel] = {0};   //����adc���ݵ�����

ADC_HandleTypeDef hadc1;         //ADC1���
DMA_HandleTypeDef hdma_adc1;     //ADC1_DMA���

/**
 *@brief       adc1��ʼ��
 *@retval      None
 */
void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  /** Common config 
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 4;
  HAL_ADC_Init(&hadc1);
  
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_11;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
  
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_13;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADCConvertedValue, NbrOfChannel);
}


/**
  * ��������: ADC�����ʼ������
  * �������: hadc��AD����������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hadc->Instance==ADC1)
  {
    __HAL_RCC_ADC1_CLK_ENABLE();            //ʹ��ADC1ʱ��
    __HAL_RCC_GPIOC_CLK_ENABLE();			//����GPIOAʱ��
//	__HAL_RCC_DMA1_CLK_ENABLE();
	    /**ADC1 GPIO Configuration    
    PC0     ------> ADC1_IN10
    PC1     ------> ADC1_IN11
    PC2     ------> ADC1_IN12
    PC3     ------> ADC1_IN13 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//	GPIO_InitStruct.Pull=GPIO_NOPULL;          //����������
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* DMA�����ʼ������ */  
    hdma_adc1.Instance = DMA1_Channel1;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&hdma_adc1);
    /* ����DMA */
    __HAL_LINKDMA(hadc,DMA_Handle,hdma_adc1);
    
    /* �����ж����ȼ����ú�ʹ���ж� */
    HAL_NVIC_SetPriority(ADC1_2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
  }
}



/*************************************************************************
*                             �����ʱ���ص���������
*************************************************************************
*/
static void ADCProcessSoftTimer_callback(void* parameter)
{

	printf("\r\n ADC1 CH10(PC0) value = %.2f V \r\n",(float)ADCConvertedValue[0]/4096 * 3.3);
	printf("\r\n ADC1 CH11(PC1) value = %.2f V \r\n",(float)ADCConvertedValue[1]/4096 * 3.3);
	printf("\r\n ADC1 CH12(PC2) value = %.2f V \r\n",(float)ADCConvertedValue[2]/4096 * 3.3);
	printf("\r\n ADC1 CH13(PC3) value = %.2f V \r\n",(float)ADCConvertedValue[3]/4096 * 3.3);

}


int ADCProcessInit()
{
	MX_DMA_Init();
	MX_ADC1_Init();//ADC��ʼ��

	ADCProcessSoftTimer = rt_timer_create("ADC", /* �����ʱ�������� */
                        ADCProcessSoftTimer_callback,/* �����ʱ���Ļص����� */
                        0,			/* ��ʱ����ʱ��������ڲ��� */
                        2*RT_TICK_PER_SECOND,   /* �����ʱ���ĳ�ʱʱ��(���ڻص�ʱ��) */
                        RT_TIMER_FLAG_PERIODIC );
                        /* �����ʱ��HARD_TIMERģʽ ����ģʽ */
 
	
	return 0;
}
	
INIT_APP_EXPORT(ADCProcessInit);


#endif




