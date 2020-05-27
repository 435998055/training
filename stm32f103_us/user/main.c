/*USER CODE BEGIN Header*/
/**
 ********************************************************
 * @file           : main.c
 * @brief          : Project file name main.c
 * @details        : frame of Project main function enter
 * @author         : weijianx
 * @email          : 3176184465@qq.com
 * @date           : 2020-05-19
 * @version        : V1.0
 ********************************************************
 * @attention
 * @brief hardware platefrom:STM32F103
 ********************************************************
*/

#include "config.h"
#include "system.h"


u8 time_tick = 10;  //Ĭ���˲���ʽ  Ϊƽ���˲�

/**
 * @brief        ������
 * @brief        ��������
 * @retval       None
 */
int main(void)
{ 

//	SysInit();        //ϵͳ��ʼ��       
    TaskInit();

    while(1)
    {	

	
		rt_thread_mdelay(1000);
		
    }
	
}

