/**
 ********************************************************
 * @file           : usart.h
 * @brief          : Header for usart.c file.
 * @author         : weijianx
 * @email          : 3176184465@qq.com
 * @date           : 2020-05-19
 * @version        : V1.0
 ********************************************************
 */


#ifndef __USART_H
#define __USART_H
#include "stdio.h"	


#include "sys.h" 


#define USART_REC_LEN  			200  		//�����������ֽ��� 200
#define EN_USART1_RX 			1			//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern uint8_t  USART_RX_BUF[USART_REC_LEN]; 	//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uint16_t USART_RX_STA;         			//����״̬���	
extern UART_HandleTypeDef UART1_Handler; 	//UART���

#define RXBUFFERSIZE   1 					//�����С
extern uint8_t aRxBuffer[RXBUFFERSIZE];			//HAL��USART����Buffer

//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(uint32_t bound);

void uart_tx(void);
unsigned char Usart1_RecProcess(void);
#endif


