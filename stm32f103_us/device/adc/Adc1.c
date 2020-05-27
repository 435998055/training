#include "config.h"
#include "Adc1.h"

#if ADC_EN == 1

__IO uint16 ADCConvertedValue[NbrOfChannel] = {0};

ADC_HandleTypeDef hadc1;         //ADC1���
DMA_HandleTypeDef hdma_adc1;     //ADC1_DMA���

void AdcGpioInit(void);
//void AdcDMAInit(void);
void AdcModeInit(void);

void AdcInit(void)
{
	//	AdcDMAInit();
	AdcModeInit();
//	HAL_ADC_MspInit(&hadc1);

	

}	

void AdcModeInit(void)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	RCC_PeriphCLKInitTypeDef ADC_CLKInit;
	
	ADC_CLKInit.PeriphClockSelection=RCC_PERIPHCLK_ADC;			//ADC����ʱ��
	ADC_CLKInit.AdcClockSelection=RCC_ADCPCLK2_DIV6;			//��Ƶ����6ʱ��Ϊ72M/6=12MHz
	HAL_RCCEx_PeriphCLKConfig(&ADC_CLKInit);					//����ADCʱ��
	
    hadc1.Instance=ADC1;
    hadc1.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //�Ҷ���
    hadc1.Init.ScanConvMode=DISABLE;                      //��ɨ��ģʽ
    hadc1.Init.ContinuousConvMode=DISABLE;                //�ر�����ת��
    hadc1.Init.NbrOfConversion=1;                         //1��ת���ڹ��������� Ҳ����ֻת����������1 
    hadc1.Init.DiscontinuousConvMode=DISABLE;             //��ֹ����������ģʽ
    hadc1.Init.NbrOfDiscConversion=0;                     //����������ͨ����Ϊ0
    hadc1.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //�������
    HAL_ADC_Init(&hadc1);                                 //��ʼ�� 
	
	HAL_ADCEx_Calibration_Start(&hadc1);					 //У׼ADC
	
	sConfig.Channel = ADC_CHANNEL_10;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();
  
    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**ADC1 GPIO Configuration    
    PC0     ------> ADC1_IN10
    PC1     ------> ADC1_IN11
    PC2     ------> ADC1_IN12
    PC3     ------> ADC1_IN13 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* ADC1 DMA Init */
    /* ADC1 Init */
    hdma_adc1.Instance = DMA1_Channel1;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
//    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
//    {
//      Error_Handler();
//    }

    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc1);

  /* USER CODE BEGIN ADC1_MspInit 1 */
  	//����ADC��DMAģʽ
	
	HAL_ADC_Start_DMA(adcHandle, (uint32_t*)ADCConvertedValue, 4);

  /* USER CODE END ADC1_MspInit 1 */
  }
}


/*************************************************************************
*                            ��������
*************************************************************************
*/


/*************************************************************************
*                             �����ʱ���ص���������
*************************************************************************
*/
static void ADCProcessSoftTimer_callback(void* parameter)
{
  printf("\r\n ADC1 CH10(PC0) value = %.2f V \r\n",(float)ADCConvertedValue[0]/4096 * 3.3);

}

int ADCProcessInit()
{
	AdcInit();//ADC��ʼ��
	ADCProcessSoftTimer = rt_timer_create("ADCProcessSoftTimer", /* �����ʱ�������� */
                        ADCProcessSoftTimer_callback,/* �����ʱ���Ļص����� */
                        0,			/* ��ʱ����ʱ��������ڲ��� */
                        2*RT_TICK_PER_SECOND,   /* �����ʱ���ĳ�ʱʱ��(���ڻص�ʱ��) */
                        RT_TIMER_FLAG_PERIODIC );
                        /* �����ʱ��HARD_TIMERģʽ ����ģʽ */
 
	
	return 0;
}
	
INIT_APP_EXPORT(ADCProcessInit);


#endif


