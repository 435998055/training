#ifndef __SYSTICK_H__
#define __SYSTICK_H__



#define MAX_TIMER 8 //����ϵͳ�δ�ʱ��ʵ�������ʱ���ĸ���

extern uint32 sysTimer[MAX_TIMER];
#define DelayTimer 							sysTimer[0]  //��ʱ��ʱ��
#define ATaskTimer 							sysTimer[1]  //A�������ж�ʱ��
#define BTaskTimer 							sysTimer[2]
#define CTaskTimer 							sysTimer[3]
#define Usart1RecTimer 					sysTimer[4] //����1���ն�ʱ���������ʱ����ʾһ֡�������
#define Usart2RecTimer 					sysTimer[5]
#define Usart3RecTimer 					sysTimer[6]
#define Usart1ControlBeepTimer  sysTimer[7] //Usart1ControlBeepTaskʹ��




void SysTickInit(void);//ϵͳ�δ�ʱ����ʼ��
void SysTickDelayMs(uint32 nms);//��ʱnms
uint32 GetRunTime(void);//��ȡϵͳ����ʱ��




#endif


