#include "config.h"
#include "system.h"


void CpuInit(void)
{
	//CPU�������
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//ѡ���2���ж����ȼ�����2λ�����ȼ���2λ�����ȼ�
	
}

void SysInit(void)
{
	
	CpuInit();//����ϵͳ��Ϣ
	
	
	//����������ʼ��
	SysTickInit();//ϵͳ�δ�ʱ����ʼ��
	LedGpioInit();//LED��ʼ��
	BeepGpioInit();//Beep��������ʼ��
	UsartInit();//���ڳ�ʼ��
	TaskInit();//�����ʼ��
	

	
}

#define  ATaskPeriod 200
#define  BTaskPeriod 300
#define  CTaskPeriod 2000
#define  Usart1ControlBeepPeriod 20



void ATask(void);//����A������
void BTask(void);//����B������
void CTask(void);
void Usart1ControlBeepTask(void);



void  TaskInit(void)
{
	ATaskTimer = ATaskPeriod;//A����ÿ��ATaskPeriod ms����һ��
	BTaskTimer = BTaskPeriod;//B����ÿ��BTaskPeriod ms����һ��
	CTaskTimer = CTaskPeriod;//C����ÿ��CTaskPeriod ms����һ��
  Usart1ControlBeepTimer = Usart1ControlBeepPeriod ;
	
}
void  TaskRun(void)
{
	ATask();//PA.0 ����
	BTask();//PA.1 ����
	CTask();
	Usart1ControlBeepTask();

}

void ATask(void)
{
	static uint8  cnt;
	
	if(ATaskTimer) return;
	ATaskTimer = ATaskPeriod;
	
	//���²����������
	if(++cnt % 2 == 0)
	{	
		GPIO_SetBits(GPIOA,GPIO_Pin_0);
		cnt = 0;
	}
  else
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);
	
	
}

void BTask(void)
{
	static uint8  cnt;
	if(BTaskTimer) return;
	BTaskTimer = BTaskPeriod;
	
	//���²����������
	
	if(++cnt % 2 == 0)
	{	
		GPIO_SetBits(GPIOA,GPIO_Pin_1);
		cnt = 0;
	}
  else
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

void CTask(void)
{

	if(CTaskTimer) return;
	CTaskTimer = CTaskPeriod;
	
	//���²����������
  printf("System Total Run time is %f s\n",GetRunTime()/1000.0);
	UsartSendString(USART1,"CTask is Running.\n--------------------------------\n");
	UsartSendString(USART2,"USART2 is Sending.\n--------------------------------\n");
}

void Usart1ControlBeepTask(void)
{
	if(Usart1ControlBeepTimer) return;
	Usart1ControlBeepTimer = Usart1ControlBeepPeriod ;
	//���²����������
	Usart1_RecProcess();
}



