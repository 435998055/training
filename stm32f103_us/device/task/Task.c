/**
 ********************************************************
 * @file           : Tak.c
 * @brief          : Project file name Task.c
 * @details        : This file describes the execution of threaded tasks.
 * @version        : V1.0
 ********************************************************
*/


#include "config.h"
#include "Task.h"

/*************************************************************************
*                            ��������
*************************************************************************
*/



static rt_thread_t dynamic_thread = RT_NULL;//��̬�߳̿��ƿ�ָ��
rt_sem_t usart2_recv_sem;//���崮��2�����ź������ƿ�ָ��
rt_mq_t  msg_mq;//������Ϣ���п��ƿ�
/*************************************************************************
*                             �߳���ں�������
*************************************************************************
*/
void led_thread_entry(void *parameter);
void usart2_recv_thread_entry(void *parameter);//����2���մ�����ں���
void msg_process_thread_entry(void *parameter);//�û���Ϣ������ں���
void adc_getVol_thread_entry(void *parameter);//ADC��ȡ��ѹֵ
void led1_thread_entry(void *parameter);
	
	
/*************************************************************************
*                             �߳̽ṹ������(�ʺ��̶߳�̬����)
*************************************************************************
*/
TaskStruct TaskThreads[] = {
			{"ledThread", led_thread_entry,  RT_NULL,  256,  5, 10},
			{"usart2_recv_thread", usart2_recv_thread_entry, 	RT_NULL, 512, 2, 	10 	},
			{"msg_process_thread",  msg_process_thread_entry, RT_NULL, 512, 2,   10 },
			{"adc_getVol_thread",adc_getVol_thread_entry, RT_NULL, 512, 2, 10},
			{"led1_thread", led1_thread_entry, RT_NULL, 256, 2, 10},  
			
			/*********************************************************/
			//�ı�ջ�Ĵ�С���ɸı��߳�������Ҳ��ͨ���ı� rtconfig.h �е� RT_MAIN_THREAD_STACK_SIZE ��С�����ı��߳�����
			//�û�����̲߳���
			//���磺{�߳�����,�߳���ں���,�߳���ں�������,�߳�ջ��С,�̵߳����ȼ�,�߳�ʱ��Ƭ},
			
			
			
			{"",RT_NULL, RT_NULL,RT_NULL,RT_NULL,RT_NULL}
	
};
	
	
/**
 *@brief      ��ʼ������
 *@retval     None
 */
void TaskInit(void)
{
	uint8_t TaskThreadIndex = 0;
	usart2_recv_sem = rt_sem_create("usart2_recv_sem",  //�ź�������
									0,                  //�ź�����ʼֵ
									RT_IPC_FLAG_FIFO    //�ź���ģʽ FIFO(0x00)
	                                );
	
	if(usart2_recv_sem != RT_NULL)
		rt_kprintf("�ź���usart2_recv_sem�����ɹ�\n\n");
	
	msg_mq = rt_mq_create("msg_mq",							//��Ϣ��������
						   32,  									//��Ϣ����󳤶�, bytes
						   10,										//��Ϣ���е��������(����)
						   RT_IPC_FLAG_FIFO			//����ģʽ FIFO
	                     );
	if(msg_mq != RT_NULL)
		rt_kprintf("��Ϣ����key_mq�����ɹ�\n\n");
	
	 while(1)
	 {
		 if(strcmp(TaskThreads[TaskThreadIndex].name,"") != 0)
		 {
			 
				dynamic_thread = rt_thread_create(TaskThreads[TaskThreadIndex].name,       // �߳����� 
                                TaskThreads[TaskThreadIndex].entry,  // �߳���ں��� 
                                TaskThreads[TaskThreadIndex].parameter,           // �߳���ں�������
                                TaskThreads[TaskThreadIndex].stack_size,               // �߳�ջ��С 
                                TaskThreads[TaskThreadIndex].priority,                 // �̵߳����ȼ� 
                                TaskThreads[TaskThreadIndex].tick                 // �߳�ʱ��Ƭ
                   	            );
				if(dynamic_thread != RT_NULL)
				{
					rt_thread_startup(dynamic_thread);
				}
			  TaskThreadIndex ++;
		 }
		 else
			 break;
		 
	 }
}


/*************************************************************************
*                             �߳���ں�������
*************************************************************************
*/

/**
 *@brief      ִ���̣߳���0�ı仯
 *@retval     None
 */
void led_thread_entry(void *parameter)
{//led�̣߳�ÿ��2s,�任PA.1״̬
	while(1)
	{
		HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_2);
	    rt_thread_mdelay(2000);	
		rt_kprintf("System Running Time:%d s \r\n",rt_tick_get()/RT_TICK_PER_SECOND);
	}
}

/**
 *@brief      ִ���̣߳�����2��Ϣ�Ľ���
 *@retval     None
 */
void usart2_recv_thread_entry(void *parameter)
{//����2���մ���
	rt_err_t uwRet = RT_EOK;
	while(1)
	{
		 uwRet =rt_sem_take(usart2_recv_sem, RT_WAITING_FOREVER);//��ȡ����2����֡����ź���
		 if(RT_EOK == uwRet )
		 {
			 rt_kprintf("Usart2 Receive Data:%s\n",USART_RX_BUF);
			 
			if(strstr((char *)USART_RX_BUF,"Beep On")!= NULL)
			{
		//		BeepOn();
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
			}
			else if(strstr((char *)USART_RX_BUF,"Beep Off")!= NULL)
			{
		//		BeepOff();
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
			}
			 memset(USART_RX_BUF,0,USART_REC_LEN);//����
			 USART_RX_STA = 0;
		 }
	}
}

/**
 *@brief      ִ���̣߳������Ķ�ȡ
 *@retval     None
 */
void msg_process_thread_entry(void *parameter)
{
	rt_err_t uwRet = RT_EOK;
	uint8_t r_queue;//���ڽ���msg_mq��Ϣ������Ϣ
	
	while(1)
	{  //��ȡ������Ϣ

		 uwRet = rt_mq_recv(msg_mq,
							&r_queue,
							sizeof(r_queue),
							RT_WAITING_FOREVER
							);
		 if(RT_EOK == uwRet )
		 {
			 switch(r_queue)//���ݽ��յ�����Ϣ���ݷֱ���д���
			 {
#if 0
				 case MSG_KEY1_PRESS:rt_kprintf("Receive message:KEY1(PE.3) is press\n\n");break;
				 case MSG_KEY2_PRESS:rt_kprintf("Receive message:KEY2(PE.2) is press\n\n");break;
				 default: rt_kprintf("No Message!\n\n");break;
#endif				 
			 }
			
		 }
		 else
		 {
			 rt_kprintf("���ݽ��մ��󣬴�����룺0x%lx\n\n",uwRet);
		 }
	}				
	
}

/**
 *@brief      ִ���̣߳���ȡ��ѹֵ
 *@retval     None
 */
void adc_getVol_thread_entry(void *parameter)
{
	uint16_t val;
	
	while(1)
	{
		val = Get_Adc();
		rt_kprintf("the vol is %d mv\r\n", val);
		rt_thread_mdelay(1000);
	}
}


/**
 *@brief      ִ���̣߳���1�ı仯
 *@retval     None
 */
void led1_thread_entry(void *parameter)
{

	while(1)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
		rt_thread_mdelay(1000);

	}
}


