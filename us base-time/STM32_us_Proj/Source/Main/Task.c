#include "config.h"
#include "Task.h"

/*************************************************************************
*                            ��������
*************************************************************************
*/

static rt_thread_t dynamic_thread = RT_NULL;//��̬�߳̿��ƿ�ָ��

/*************************************************************************
*                             �߳���ں�������
*************************************************************************
*/
void led_thread_entry(void *parameter);
void usart2_recv_thread_entry(void *parameter);//����2���մ�����ں���
void msg_process_thread_entry(void *parameter);//�û���Ϣ������ں���
	
	
/*************************************************************************
*                             �߳̽ṹ������(�ʺ��̶߳�̬����)
*************************************************************************
*/
TaskStruct TaskThreads[] = {
			{"ledThread", led_thread_entry,  RT_NULL,  256,  5, 10},
			{"usart2_recv_thread", usart2_recv_thread_entry, 	RT_NULL, 512, 2, 	10 	},
			{"msg_process_thread",  msg_process_thread_entry, RT_NULL, 512, 2,   10 },
			
			/*********************************************************/
			//�û�����̲߳���
			//���磺{�߳�����,�߳���ں���,�߳���ں�������,�߳�ջ��С,�̵߳����ȼ�,�߳�ʱ��Ƭ},
			
			
			
			{"",RT_NULL, RT_NULL,RT_NULL,RT_NULL,RT_NULL}
	
};
	
	
	
	

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
void led_thread_entry(void *parameter)
{//led�̣߳�ÿ��2s,�任PA.1״̬
	while(1)
	{
		LedToggle(GPIOA,GPIO_Pin_1);
	  rt_thread_mdelay(2000);	
//		rt_kprintf("System Running Time:%d s \n",rt_tick_get()/RT_TICK_PER_SECOND);
	}
}

void usart2_recv_thread_entry(void *parameter)
{//����2���մ���
	rt_err_t uwRet = RT_EOK;
	while(1)
	{
		 uwRet =rt_sem_take(usart2_recv_sem, RT_WAITING_FOREVER);//��ȡ����2����֡����ź���
		 if(RT_EOK == uwRet )
		 {
			 rt_kprintf("Usart2 Receive Data:%s\n",g_USART2_RxBuf);
			 
			 if(strstr((char*)g_USART2_RxBuf,"BeepOn")!=NULL)
			 {
				 BeepOn();
			 }
			 if(strstr((char*)g_USART2_RxBuf,"BeepOff")!=NULL)
			 {
				 BeepOff();
				 
			 }
			 memset(g_USART2_RxBuf,0,USART2_RX_BUF_SIZE);//����
			 g_USART2_RecPos = 0;
		 }
	}
}

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

