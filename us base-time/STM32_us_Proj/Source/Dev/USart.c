#include "config.h"
#include "USart.h"

#if USART1_EN == 1
	uint8   g_USART1_RxBuf[USART1_RX_BUF_SIZE];//���ڽ��ջ�����
	uint16 	g_USART1_RecPos = 0;//��ŵ�ǰ���ڽ������ݴ�ŵ�λ��
#endif 

#if USART2_EN == 1
	uint8   g_USART2_RxBuf[USART2_RX_BUF_SIZE];//���ڽ��ջ�����
	uint16 	g_USART2_RecPos = 0;//��ŵ�ǰ���ڽ������ݴ�ŵ�λ��
#endif 

#if USART3_EN == 1
	uint8   g_USART3_RxBuf[USART3_RX_BUF_SIZE];//���ڽ��ջ�����
	uint16 	g_USART3_RecPos = 0;//��ŵ�ǰ���ڽ������ݴ�ŵ�λ��
#endif 

void UsartInit(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef  USART_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
#if USART1_EN == 1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9 ;  //TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10 ;  //RX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	USART_InitStructure.USART_BaudRate = USART1_BAUD;  
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity =  USART_Parity_No;
  USART_InitStructure.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &USART_InitStructure);//��ʼ������1
#if 0 //����FinSH��rt_hw_console_getcharʹ�ò�ѯ��ʽʵ�֣��ʴ���1�жϳ�ʼ����ע�͵�
	//����1�жϳ�ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
#endif
	USART_Cmd(USART1, ENABLE);//ʹ�ܴ���1
	
  USART_ClearFlag(USART1, USART_FLAG_TC  );//�巢����ɱ�־λ
#endif


#if USART2_EN == 1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2 ;  //TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3 ;  //RX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	USART_InitStructure.USART_BaudRate = USART2_BAUD;  
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity =  USART_Parity_No;
  USART_InitStructure.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART2, &USART_InitStructure);//��ʼ������2
	
	//����2�жϳ�ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//��Ӵ��ڿ����ж�ʹ�ܣ��벻Ҫʹ��USART_IT_RXNE|USART_IT_IDLE����ס�ֿ�д�������

	USART_Cmd(USART2, ENABLE);//ʹ�ܴ���2
	
  USART_ClearFlag(USART2, USART_FLAG_TC  );//�巢����ɱ�־λ
#endif

#if USART3_EN == 1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10 ;  //TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11 ;  //RX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	USART_InitStructure.USART_BaudRate = USART3_BAUD;  
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity =  USART_Parity_No;
  USART_InitStructure.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART3, &USART_InitStructure);//��ʼ������3
	
	//����3�жϳ�ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART3, ENABLE);//ʹ�ܴ���3
	
  USART_ClearFlag(USART3, USART_FLAG_TC  );//�巢����ɱ�־λ
#endif

}


//Ϊ�ܹ�����printf������ѡ���Ĵ��ڴ�ӡ������ض���fputc������������ʹ�ô���1
int fputc(int ch,FILE *f)
{
	
	USART_SendData(USART1, (uint8)ch);
	
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)== RESET);
	
	return (ch);

}

static void UsartSendByte(USART_TypeDef* USARTx,uint8 ch)
{
	
	USART_SendData(USARTx, (uint8)ch);
	
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)== RESET);
}


void UsartSendString(USART_TypeDef* USARTx,uint8 *str)
{
	uint32 pos = 0;
	while(*(str+pos)!='\0')
	{
		UsartSendByte(USARTx,*(str+pos));
		pos ++;
		
	}

}



#if 0 //����FinSH��rt_hw_console_getcharʹ�ò�ѯ��ʽʵ�֣��ʴ���1�жϻص���ע�͵�
//USART1_IRQHandler,����1�жϻص�����
void USART1_IRQHandler(void)
{
#if USART1_EN == 1 

#endif
	if( USART_GetFlagStatus(USART1,USART_FLAG_RXNE)!=RESET )				// ���ڽ������� 
	{		
#if USART1_EN == 1 

#endif
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
	}
	if( USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET ) 				// �����������
	{
			USART_ClearFlag(USART1, USART_FLAG_ORE);
	}
}
#endif




//USART2_IRQHandler,����2�жϻص�����
void USART2_IRQHandler(void)
{
	#if USART2_EN == 1 
		//�û�����
	 uint8 RecCh;
	#endif
	if( USART_GetFlagStatus(USART2,USART_FLAG_RXNE)!=RESET )				// ���ڽ������� 
	{		
		#if USART2_EN == 1 
					 //�û�����
		RecCh = (uint8)USART_ReceiveData(USART2);
		g_USART2_RxBuf[g_USART2_RecPos++] = RecCh;
		
		#endif
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}
	if( USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET ) 				// �����������
	{
			USART_ClearFlag(USART2, USART_FLAG_ORE);
	}
	if( USART_GetFlagStatus(USART2,USART_FLAG_IDLE)==SET ) 				// �����������
	{
		#if USART2_EN == 1 
					 //�û�����
		
		g_USART2_RxBuf[g_USART2_RecPos] = '\0';
		rt_sem_release(usart2_recv_sem);//�ͷ�һ���ź�������ʾ�����ѽ���;������ֵ�ź��� �����ͽ��յ�������֡��־����ǰ̨�̲߳�ѯ
		
		#endif
		USART_ReceiveData(USART2);//ʹ�ø������������жϱ�־λ���벻Ҫʹ��USART_ClearITPendingBit(USART2, USART_IT_IDLE);������޷��ﵽЧ��
	}
}




//USART3_IRQHandler,����3�жϻص�����
void USART3_IRQHandler(void)
{
	#if USART3_EN == 1 
		//�û�����
	#endif

	if( USART_GetFlagStatus(USART3,USART_FLAG_RXNE)!=RESET )				// ���ڽ������� 
	{		
		#if USART3_EN == 1 
			//�û�����
		#endif
		
		USART_ClearFlag(USART3, USART_FLAG_RXNE);
	}
	if( USART_GetFlagStatus(USART3,USART_FLAG_ORE)==SET ) 				// �����������
	{
			USART_ClearFlag(USART3, USART_FLAG_ORE);
	}
}


void rt_hw_console_output(const char *str)  //ʵ�ָú���������ʹ��rt_kprintf
{

	 /* �����ٽ�� */
  rt_enter_critical();
	
	while(*str!='\0')
	{
		 /* ���� */
    if (*str == '\n')//RT-Thread ϵͳ�����еĴ�ӡ���� \n ��β�������� \r\n���������ַ����ʱ����Ҫ����� \n ֮ǰ��� \r����ɻس��뻻�У�����ϵͳ��ӡ��������Ϣ��ֻ�л���
    {
       USART_SendData(USART1, '\r');
	
	     while(USART_GetFlagStatus(USART1, USART_FLAG_TC)== RESET);
    }

		USART_SendData(USART1, *(str++));
	
	  while(USART_GetFlagStatus(USART1, USART_FLAG_TC)== RESET);
		
	}
	 /* �˳��ٽ�� */
  rt_exit_critical();  //ע�⣺ʹ�ý����ٽ�����rt_enter_critical(); һ��Ҫʹ���˳��ٽ����� rt_exit_critical();�����������ס���޷����е���
	
}

char rt_hw_console_getchar(void)//ʹ��Finsh��������裺1.ʵ�ָú�����rt_hw_console_output������2.rtconfig.h�п���RT_USING_FINSH��;3.���Finsh���(cmd.c��msh.c��shell.c)��
{  //��ѯ��ʽʵ�֣��ǵý�Usart1��ʼ���е��жϽ���������ش���ע�͵�
	   int ch = -1;
	   /*�ȴ�����1��������*/
     if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
		 {
         ch = (int)USART_ReceiveData(USART1);
			   USART_ClearFlag(USART1, USART_FLAG_RXNE);
		 }
		 else
		 {
			 
         if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
         {
             USART_ClearFlag(USART1, USART_FLAG_ORE);
         }
         rt_thread_mdelay(10);
			 
		 }

     return ch;
}
