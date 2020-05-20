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
	TaskInit();                             //�����ʼ��
	

	
}

/***************�궨������ʱ�Ӽ��*************/
#define  ATaskPeriod 1000
#define  BTaskPeriod 1000
#define  CTaskPeriod 2000
#define  Usart1ControlBeepPeriod 20
/**********************************************/

/*****************��������*********************/
void ATask(void);//����A������
void BTask(void);//����B������
void CTask(void);//����C������
void Usart1ControlBeepTask(void);
/**********************************************/

/** 
 * @brief        ÿ�������ʱ������
 * @retval       None
 */
void  TaskInit(void)
{
	ATaskTimer = ATaskPeriod;//A����ÿ��ATaskPeriod ms����һ��
	BTaskTimer = BTaskPeriod;//B����ÿ��BTaskPeriod ms����һ��
	CTaskTimer = CTaskPeriod;//C����ÿ��CTaskPeriod ms����һ��
    Usart1ControlBeepTimer = Usart1ControlBeepPeriod ;
	
}

/** 
 * @brief       ����ִ��
 * @retval      None
 */
void  TaskRun(void)
{
	ATask();//PA.8 ����
	BTask();//PD.2 ����
	CTask();
	Usart1ControlBeepTask();

}

/** 
 * @brief         ִ������A��PA.8�Ƶ���˸
 * @retval        None
 */
void ATask(void)
{
	static uint8  cnt;
	
	if(ATaskTimer) return;
	ATaskTimer = ATaskPeriod;
	
	//���²����������
	if(++cnt % 2 == 0)
	{	
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
		cnt = 0;
	}
  else
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
	
	
}

/** 
 * @brief         ִ������B��PD.2�Ƶ���˸
 * @retval        None
 */
void BTask(void)
{
	static uint8  cnt;
	if(BTaskTimer) return;
	BTaskTimer = BTaskPeriod;
	
	//���²����������
	
	if(++cnt % 2 == 0)
	{	
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
		cnt = 0;
	}
    else
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
}

/** 
 * @brief         ִ������C������1�Ĵ���
 * @retval        None
 */
void CTask(void)
{
	uint8_t tx_buf1[]="CTask is Running.\r\n";
	uint8_t tx_buf2[]="USART2 is Sending.\r\n";

	if(CTaskTimer) return;
	CTaskTimer = CTaskPeriod;
	
	//���²����������
    printf("System Total Run time is %f s\r\n",GetRunTime()/1000.0);
	HAL_UART_Transmit(&UART1_Handler, (uint8_t*)tx_buf1, sizeof(tx_buf1), 1000);
	HAL_UART_Transmit(&UART1_Handler, (uint8_t*)tx_buf2, sizeof(tx_buf2), 1000);

}

/** 
 * @brief           ִ�д��ڵ�����
 * @retval          None
 */
void Usart1ControlBeepTask(void)
{
	if(Usart1ControlBeepTimer) return;
	Usart1ControlBeepTimer = Usart1ControlBeepPeriod ;
	//���²����������
	Usart1_RecProcess();
}



