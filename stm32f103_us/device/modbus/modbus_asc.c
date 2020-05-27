#include "usart.h"
#include "modbus_asc.h"
#include "modbus_ascii.h"
#include "type.h"
#include "eeprom1.h"
#include "para.h"
#include "string.h"

uint8_t StartFlag = 0;
uint8_t StartCountFlag = 0;
uint8_t SendLen;
uint8_t SendBuf[100];
extern uint8_t USART_RX_BUF[USART_MAX_DATALEN];
extern UserTypeDef UserPara;
//extern uint8_t u8SendNum;
extern BitAction UartRecvFrameOK;
extern BitAction StartFillBufFlag;


extern uint8_t Cur_Param[USER_DEFAULT_LEN];
extern uint8_t const  User_Default_Param[USER_DEFAULT_LEN];
extern uint16_t Current_PositiveTime ;  //��ǰ��ת������
extern uint16_t Current_NegativeTime ;  //��ǰ��ת������

uint8_t k, i;

u8 const SensorSoftVersion[8] = {0x07, 'S', 'V', '1', '.', '0', '.', '4'};      //����汾  20200413
//**************************************************************************************************
// ����         	: MBASCII_GetSlaveAddr()
// ��������   	        : 2015-10-29
// ����         	: wang
// ����         	:����豸��ַ
// �������     	:���յĻ�������(u8 *u8Msg)
// �������     	: �豸��ַ
// ���ؽ��     	: ��
// ע���˵��   	:
// �޸�����     	:
//**************************************************************************************************

u32 MBASC_GetSlaveAddr(u8 *u8Msg)
{
    return(u8Msg[0]);
}


//******************************************************************************
// ����         : MBASC_SendMsg()
// ��������     : 2018-06-08
// ����         : MMX
// ����         : ��ASCII��ʽ������Ϣ
// �������     : ֡������(u8 *u8Msg),֡��(u8 u8MsgLen)
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : ��
// �޸�����     : ��
//******************************************************************************
void MBASC_SendMsg(uint8_t *u8Msg, uint8_t u8MsgLen)
{
    if(MB_ADDRESS_BROADCAST != u8Msg[0])
    {
      Delay_Ms(50);
        MODBUS_ASCII_SendData(u8Msg, u8MsgLen);
    }
}

//******************************************************************************
// ����         : MBASC_SendMsg_NoLimit()
// ��������     : 2018-06-08
// ����         : MMX
// ����         : �������Ե���ASCII��ʽ������Ϣ
// �������     : ֡������(u8 *u8Msg),֡��(u8 u8MsgLen)
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : ��
// �޸�����     : ��
//******************************************************************************
void MBASC_SendMsg_NoLimit(uint8_t *u8Msg, uint8_t u8MsgLen)
{
   Delay_Ms(50);
    MODBUS_ASCII_SendData(u8Msg, u8MsgLen);
}

//******************************************************************************
// ����         : MBASC_SendErr()
// ��������     : 2016-06-08
// ����         : MMX
// ����         : ���ʹ��������Ӧ֡
// �������     : ������(u8 ErrCode)
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : ��
// �޸�����     : ��
//******************************************************************************
void MBASC_SendErr(uint8_t ErrCode)
{
    SendLen = 1;
    SendBuf[SendLen++] |= 0x80;
    SendBuf[SendLen++] = ErrCode;
    MBASC_SendMsg(SendBuf, SendLen);
}

//******************************************************************************
// ����         : MBASC_Fun03()
// ��������     : 2018-06-08
// ����         : MMX
// ����         : 03�����룬�������Ĵ�������
// �������     : ��
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : ��
// �޸�����     : ��
//******************************************************************************
void MBASC_Fun03(void)
{
    uint8_t i;
    uint16_t Data_Buf;
   // uint32_t Data_Buf1;
    uint16_t Register_Num = 0;
    uint8_t ReadAdrH, ReadAdrL;
    
    ReadAdrH = USART_RX_BUF[2];
    ReadAdrL = USART_RX_BUF[3];

    Register_Num = ((uint16_t)USART_RX_BUF[4] << 8) + USART_RX_BUF[5];
     
    SendLen = 0;
    SendBuf[SendLen++] = USART_RX_BUF[0] ? UserPara.SlaveAddr : 0x00;
    SendBuf[SendLen++] = 0x03;	                        //������
    SendBuf[SendLen++] = Register_Num * 2;		                        //���ݳ���
                                                                                //�����ȡ��Χ���
    if ((!(((ReadAdrL >= MBASC_HOLDING_REG_REGION_BGEIN) && (ReadAdrL <= MBASC_HOLDING_REG_REGION_END)
        && (ReadAdrL + Register_Num <= (MBASC_HOLDING_REG_REGION_END + 1)))&& (0 != Register_Num)))
        || ((ReadAdrH != UserPara.SlaveAddr) && (ReadAdrH != MB_ADDRESS_BROADCAST)))//�㲥��ַ�ſ�
    {
        MBASC_SendErr(MB_EX_ILLEGAL_DATA_ADDRESS);
        return;
    }

    for (k = 0; k < Register_Num; ReadAdrL++, k++)
    {
        switch (ReadAdrL)
        {
          case 0x30:
            Data_Buf = UserPara.SlaveAddr;      //�豸��ַ
            break;

          case 0x31:
            Data_Buf = UserPara.Baudrate;	//��ѯ�����ʣ�			        
            break;

          case 0x32:
            Data_Buf = UserPara.Parity;		//��ѯ��żУ�飺	                        
            break;

          case 0x33:
            Data_Buf = 0;						        
            break;

          case 0x34:
            Data_Buf = 0;  //����ʹ��	                        
            break;

          case 0x35:                                                          
            Data_Buf = 0;	//�˲���ʽ	  		                                             
            break;

          case 0x36:
            Data_Buf = 0;	//�Զ��ϱ���ʽ	               
            break;

          case 0x37:            //�������ϵ��K
            Data_Buf = 0;                       
            break;

          case 0x38:             //�������ϵ��B
            Data_Buf = 0;                        
            break;

          case 0x39:
            Data_Buf = 0;	                                                                 
            break;

          case 0x3A:
            Data_Buf = 0;	                                                             
            break;  
            
          case 0x3B:
            Data_Buf = 0;	                                                             
            break;             
            
          case 0x3C:
            Data_Buf = 0;	                                                             
            break;
            
          case 0x3D:
            //Data_Buf = UserPara.HeightUnit;	
            break;             
            
          case 0x3E:
            Data_Buf = 0;	                                                             
            break;
            
          case 0x3F:
           // Data_Buf = UserPara.SensorRange;//;��������������                                                  
            break;
            
          case 0x40:
            Data_Buf = 0;//UserPara.DetectThr;		        
            break;

          case 0x41:
            Data_Buf = 0;
            //Data_Buf = UserPara.StandbyAlarmThr;
            break;
            
          case 0x42:
            Data_Buf = 0;//UserPara.StaVaryDuration;                                 
            break;  
            
          case 0x43:
            Data_Buf = 0;//(uint16_t)(UserPara.WorkTimeBase >> 16);                 //�ܹ���ʱ����ֵ��λ                       
            break;

          case 0x44:
            Data_Buf = 0;//(uint16_t)UserPara.WorkTimeBase;                         //�ܹ���ʱ����ֵ��λ                       
            break;

          case 0x45:
            Data_Buf = 0;//(uint16_t)(UserPara.StandbyTimeBase >> 16);              //�ܴ���ʱ����ֵ��λ                                                            
            break;

          case 0x46:
            Data_Buf =0;//(uint16_t)UserPara.StandbyTimeBase;                       //�ܴ���ʱ����ֵ��λ                                                              
            break;  
            
          case 0x47:
            Data_Buf = 0;//(uint16_t)(UserPara.StopTimeBase >> 16);                 //��ͣ��ʱ����ֵ��λ  	                                                             
            break;             
            
          case 0x48:
            Data_Buf = 0;//(uint16_t)UserPara.StopTimeBase;	                        //��ͣ��ʱ����ֵ��λ                                                            
            break;

          case 0x49:
            Data_Buf = 0;
            break; 

          case 0x4A:
            Data_Buf = 0;
            break;

          case 0x4B: 
            Data_Buf = 0;        
            break;   
            
          case 0x4C:
            Data_Buf = 0;  
            break; 

          case 0x4D:
            Data_Buf = 0;  
            break;

          case 0x4E: 
            Data_Buf = 0;    
            break;   
                
          case 0x4F:
            Data_Buf = 0;  
            break;            
            
          default:
            Data_Buf = 0;
            break;
        }
        
       
          for (i = 2; i > 0; i--)
          {
              SendBuf[SendLen++] = (uint8_t)(Data_Buf >> ((i - 1) * 8));
          }
        
      
    }
    MBASC_SendMsg_NoLimit(SendBuf, SendLen);                                      
}



//******************************************************************************
// ����         : MBASC_Fun04()
// ��������     : 2018-06-08
// ����         : MMX
// ����         : 04�����룬��˫���Ĵ�������
// �������     : ��
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : ��
// �޸�����     : �� 
//******************************************************************************
void MBASC_Fun04(void)	                                                        //��˫���Ĵ���
{
    uint8_t i;
    uint32_t Data_Buf;
    uint16_t Register_Num ;
    uint8_t ReadAdrH;
    uint8_t ReadAdrL;
    
    ReadAdrH = USART_RX_BUF[2];
    ReadAdrL = USART_RX_BUF[3];
    
    Register_Num = ((uint16_t)USART_RX_BUF[4] <<8) + USART_RX_BUF[5];
  
    SendLen = 0;
    SendBuf[SendLen++] = USART_RX_BUF[0] ? UserPara.SlaveAddr : 0x00;
    SendBuf[SendLen++] = 0x04;
    SendBuf[SendLen++] = Register_Num * 2;		                        //���ݳ���

    if(  (!((   ( (ReadAdrL <= MBASC_INPUT_REG_REGION_END) && ((ReadAdrL + Register_Num) <= (MBASC_INPUT_REG_REGION_END + 2)))  )&& ((0 != Register_Num) && (0 == (Register_Num & 0x01)) && (0 == (ReadAdrL & 0x01)))))   ||   (ReadAdrH != UserPara.SlaveAddr))   
                         //�����ַС�ڵ�04������Ĵ������ֵ       �����ַ���� +2 <= 04������Ĵ������ֵ+2                              ���Ȳ�Ϊ0                 ����Ϊ˫��                     ����Ϊ˫��                          �ӻ���ַ���
    {
        MBASC_SendErr(MB_EX_ILLEGAL_DATA_ADDRESS);
        return;
    }

    for(k = 0; k < Register_Num; ReadAdrL += 2, k += 2)
    {
        switch (ReadAdrL)
        {
          case 0x00:
             
              Data_Buf = UserPara.RotateSta;//��ת״̬

              break;
          case 0x02:  
              Data_Buf = UserPara.DirSta;//��ת����                                      
            break;
            
          case 0x04: 
              Data_Buf = UserPara.RotateSpeed;//��ת�ٶ�	                                                                     
            break;
            
          case 0x06: 
              Data_Buf = UserPara.WorkTime;//  �ۼ�����ʱ��                                      
            break;

          case 0x08: 
              Data_Buf = UserPara.TotalPulse;//  �ۼ���������                                    
            break;            

          case 0x0A: 
              
              Data_Buf = UserPara.Duration;
                                 
            break; 

          case 0x0C: 
              Data_Buf = 0;//                                      
            break; 
            
          case 0x0E: 
              Data_Buf =  0;//                                   
            break;
            
          default:
            Data_Buf = 0; 
            break;
        }
        for(i = 4; i > 0; i--)
        {
            SendBuf[SendLen++] = (uint8_t)(Data_Buf >> ((i - 1) * 8));
        }
    }
    MBASC_SendMsg(SendBuf, SendLen);
}


//******************************************************************************
// ����         : MBASC_Fun05()
// ��������     : 2019-12-17
// ����         : LHL
// ����         : 05�����룬//д������Ȧ�����ڱ궨Һλ��ֵ����ֵ
// �������     : ��
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : ��
// �޸�����     : ��
//******************************************************************************
void MBASC_Fun05()   //д������Ȧ
{
   // uint32_t index = 0;
    //uint16_t ReadAdr = 0;
  //  uint16_t Register_Num = 0;
    //uint16_t u16Temp;
    uint8_t ReadAdrH, ReadAdrL;
    //uint8_t uTemp[2];

   // uint8_t uArray[12];
    BitAction bWriteParaFlag = Bit_RESET; 
    
    ReadAdrH = USART_RX_BUF[2];
    ReadAdrL = USART_RX_BUF[3];
        
  //  Register_Num = ((uint16_t)USART_RX_BUF[4] << 8) + USART_RX_BUF[5]; 
    
    SendLen = 0;
    SendBuf[SendLen++] = USART_RX_BUF[0] ? UserPara.SlaveAddr : 0x00;
    SendBuf[SendLen++] = 0x05;
   // SendBuf[SendLen++] = Register_Num * 2;
                                                                                //�����ȡ��Χ���
    if ((!((((ReadAdrL >= MBASC_SINGLE_COIL_ADDR_BGEIN) && (ReadAdrL <= MBASC_SINGLE_COIL_ADDR_END)))))|| (ReadAdrH != UserPara.SlaveAddr))
    {
        MBASC_SendErr(MB_EX_ILLEGAL_DATA_ADDRESS);
        return;
    }
 
    if(!(((USART_RX_BUF[4] == 0xFF) && (USART_RX_BUF[5] == 0x00))
              || ((USART_RX_BUF[4] == 0x00) && (USART_RX_BUF[5] == 0x00))))
    {
      MBASC_SendErr(MB_EX_ILLEGAL_DATA_VALUE);
      return;
    }
    
        switch (ReadAdrL)
        {
            case 0x50:   //�궨��Һλ/��Һλ	
             if((USART_RX_BUF[4] == 0x00) && (USART_RX_BUF[5] == 0x00))//�궨��Һλ
             {
             //  UserPara.LiquidAdMin = UserPara.AD_Value_Filter;
               // Unshort2Array(UserPara.LiquidAdMin, uTemp);
               //Eeprom_WriteNBytes(LIQUIDADMIN, uTemp, 2);                
             }
             else if((USART_RX_BUF[4] == 0xFF) && (USART_RX_BUF[5] == 0x00))//�궨��Һλ
              {
             //  UserPara.LiquidAdMax = UserPara.AD_Value_Filter;
              // Unshort2Array(UserPara.LiquidAdMax, uTemp);
              // Eeprom_WriteNBytes(LIQUIDADMAX, uTemp, 2);  
              } 
               break;
         
            case 0x51:	
               if ((USART_RX_BUF[4] == 0x00) && (USART_RX_BUF[5] == 0x00))
                  {
                    Eeprom_WriteByte(RUN_ADDR_BASE, "\x02", 1);  //�ָ���������
                    ReadPara(); 
                    Eeprom_WriteByte(RUN_ADDR_BASE, "\x01", 1);  
                  }
               break;
                
            case 0x52:  //�����豸/�ⶳ�豸
                           
                break;   
                
            case 0x53: //flash дʹ��/ũ���ֽ�ֹ              
                break; 
                
            default:
                break;
        }
   
                                                                          //�����·��ĸ���ֵ����ǰ����ֵ����
    if(bWriteParaFlag == Bit_SET)
    {
        bWriteParaFlag = Bit_RESET;
        //UserPara.Duration = 0;
        //UserPara.WorkTime = 0;
       // UserPara.StandbyTime = 0;
        //UserPara.StopTime = 0;
        //memset(uArray, 0x00, sizeof(uArray));
       // Eeprom_WriteNBytes(WORK_TIME, uArray, sizeof(uArray));
    }
    MBASC_SendMsg(USART_RX_BUF, 6);
}

//******************************************************************************
// ����         : MBASC_Fun10()
// ��������     : 2018-06-08
// ����         : MMX
// ����         : 10�����룬д����Ĵ��������������޸Ĳ���
// �������     : ��
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : ��
// �޸�����     : ��
//******************************************************************************
void MBASC_Fun10()
{
    uint32_t index = 0;
    //uint16_t ReadAdr = 0;
    uint16_t Register_Num = 0;
    uint16_t u16Temp;
    uint8_t ReadAdrH, ReadAdrL;
    uint8_t uTemp[4];

   // uint8_t uArray[12];
    BitAction bWriteParaFlag = Bit_RESET; 
    
    ReadAdrH = USART_RX_BUF[2];
    ReadAdrL = USART_RX_BUF[3];
        
    Register_Num = ((uint16_t)USART_RX_BUF[4] << 8) + USART_RX_BUF[5]; 
    
    SendLen = 0;
    SendBuf[SendLen++] = USART_RX_BUF[0] ? UserPara.SlaveAddr : 0x00;
    SendBuf[SendLen++] = 0x10;
    SendBuf[SendLen++] = Register_Num * 2;
                                                                                //�����ȡ��Χ���
    if ((!((((ReadAdrL >= MBASC_MUL_REG_REGION_BGEIN) && (ReadAdrL <= MBASC_MUL_REG_REGION_END)
            && (ReadAdrL + Register_Num <= (MBASC_MUL_REG_REGION_END + 1))))
            && ((0 != Register_Num)) && ((Register_Num * 2) == USART_RX_BUF[6])))
            || (ReadAdrH != UserPara.SlaveAddr))
    {
        MBASC_SendErr(MB_EX_ILLEGAL_DATA_ADDRESS);
        return;
    }

    for (k = 0; k < Register_Num; ReadAdrL++, k++)
    {
        switch (ReadAdrL)
        {
            case 0x30:						                //�豸��ַ
                UserPara.SlaveAddr = USART_RX_BUF[7+index] * 256 + USART_RX_BUF[8+index];
                Eeprom_WriteByte(SLAVE_ADDR, &UserPara.SlaveAddr, 1);
                break;
            case 0x31:                                                          //������
                UserPara.Baudrate = USART_RX_BUF[7+index] * 256 + USART_RX_BUF[8+index];
                Eeprom_WriteByte(BAUDRATE, &UserPara.Baudrate, 1);
			    uart_init(115200);
//                Uart_Config_Init();
                break;
               
            case 0x32:	                                                        //��żУ��
                UserPara.Parity = USART_RX_BUF[7+index] * 256 + USART_RX_BUF[8+index];
                Eeprom_WriteByte(PARITY, &UserPara.Parity, 1);    
				uart_init(115200);
//                Uart_Config_Init();
                break;

            case 0x33: //����                                                             
                break;

            case 0x34:  //����ʹ��                                                           
                break;

            case 0x35:	//�˲�ϵ��
                //if((0x00 < USART_RX_BUF[8+index]) && (0x04 > USART_RX_BUF[8+index]))
                {
                 // UserPara.FilterLevel = USART_RX_BUF[8+index];
                 // Switch_Fiter(UserPara.FilterLevel);
                }
               // Eeprom_WriteNBytes(FILTER_LEVEL, &UserPara.FilterLevel, 1);
           					                
                break;

            case 0x36:   //�Զ��ϱ���ʽ                                                           
                break;

            case 0x37:   //�������ϵ��K                                                        
                break;

            case 0x38:   //�������ϵ��B
                break;

            case 0x39:	//�޸��ۼ�����ʱ��
                 u16Temp =  USART_RX_BUF[7 + index] * 256 + USART_RX_BUF[8 + index];
                if(u16Temp != 0xFFFF)                                           //��0xFFFFʱ��ִ��
                {
                    UserPara.WorkTime = u16Temp;
                    long32Array(UserPara.WorkTime, uTemp);
                    Eeprom_WriteByte(WORK_TIME_BASE, uTemp, 4);
                }        
                break;

            case 0x3A: //�޸��ۼ�������
                 u16Temp =  USART_RX_BUF[7 + index] * 256 + USART_RX_BUF[8 + index];
                if(u16Temp != 0xFFFF)                                           //��0xFFFFʱ��ִ��
                {
                    UserPara.TotalPulse = u16Temp;
                    long32Array(UserPara.TotalPulse, uTemp);
                    Eeprom_WriteByte(PULSE_TOTAL_BASE, uTemp, 4);
                }        
                break; 

            case 0x3B:						                
                break;

            case 0x3C:						                
                break;

            case 0x3D:						             
            
                break;

            case 0x3E:						                
                break; 
           
            case 0x40:
                            
                break; 
        
        
            case 0x70:	   //����            
                break;

            case 0x71:     //ACC״̬            
                break;   
                
            case 0x72:        //0x72 0x73 �����       
                break; 
                
            default:
                break;
        }
        index += 2;
    }                                                                           //�����·��ĸ���ֵ����ǰ����ֵ����
    if(bWriteParaFlag == Bit_SET)
    {
        bWriteParaFlag = Bit_RESET;
        //UserPara.Duration = 0;
        //UserPara.WorkTime = 0;
       // UserPara.StandbyTime = 0;
        //UserPara.StopTime = 0;
        //memset(uArray, 0x00, sizeof(uArray));
       // Eeprom_WriteNBytes(WORK_TIME, uArray, sizeof(uArray));
    }
    MBASC_SendMsg(USART_RX_BUF, 6);
}


//**************************************************************************************************
// ����         : MBASC_Fun2B()
// ��������     : 2016-09-19
// ����         : ÷����
// ����         : ���ڶ�ȡ�豸���к���Ϣ
// �������     : ��
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : һ���5������ʱ�������ܴ�ע��SendBuf�ĳ���
// �޸�����     :
//**************************************************************************************************
void MBASC_Fun2B(void)
{
    u16 Object_Num = 0, ReadAdr = 0;
    u8 CompanyNameLen = 0;//ProductionCodeLen = 0, HardwareVersionLen = 0;
    u8 SoftwareVersionLen = 0;//DeviceIdLen = 0, CustomerCodeLen = 0;
    
    if(UserPara.SlaveAddr != USART_RX_BUF[2])                                   //���ӻ���ַ��Ϊ��0x45ʱ��Ҳ���Զ�д�Ĵ���
    {
        ReadAdr = USART_RX_BUF[2] * 256 + USART_RX_BUF[3];
    }
    else
    {
        ReadAdr = 0x51 * 256 + USART_RX_BUF[3];
    }
    Object_Num = USART_RX_BUF[4] * 256 + USART_RX_BUF[5];                       //���������   
    
    SendLen = 0;
    SendBuf[SendLen++] = (MBASC_GetSlaveAddr(USART_RX_BUF)) ? UserPara.SlaveAddr : 0x00;
    SendBuf[SendLen++] = 0x2B;
    SendBuf[SendLen++] = USART_RX_BUF[4];
    SendBuf[SendLen++] = USART_RX_BUF[5];                      
    
    if (!(((ReadAdr >= 0x51E0) && (ReadAdr <= 0x51E5) && (ReadAdr + Object_Num) <= (0x71E5 + 1))
        && (0 != Object_Num)))
    {
        MBASC_SendErr(MB_EX_ILLEGAL_DATA_ADDRESS);
        return;
    }   
    
    for (k = 0; k < Object_Num; ReadAdr++, k++)
    {
        switch (ReadAdr)
        {
        case 0x51E0:
            SendBuf[SendLen++] = 0xE0;
            //CompanyNameLen = I2C1_ReadByte(EEPROM_ADDRESS, COMPANY);            //��ȡ��һ��λ�õ����ݵĳ���
            if((0 == CompanyNameLen) || (0x32 < CompanyNameLen))                //���Ȳ���ȷ
            {
                SendBuf[SendLen++] = 0x01;                                      //Ӧ��һ���ֽ� 0x00                             
                SendBuf[SendLen++] = 0x00;
            }
            else
            {
                SendBuf[SendLen++] = CompanyNameLen;
//                for (u8 i = 0; i < CompanyNameLen; i++)
//                {                             
//                    SendBuf[SendLen++] = I2C1_ReadByte(EEPROM_ADDRESS, COMPANY + 1 + i);
//                }
            }
            break;
        
        case 0x51E1:
            SendBuf[SendLen++] = 0xE1;
           // ProductionCodeLen = I2C1_ReadByte(EEPROM_ADDRESS, DEV_ENCODING);
//            if((0 == ProductionCodeLen) || (0x32 < ProductionCodeLen))        
//            {
//                SendBuf[SendLen++] = 0x01;                                      //Ӧ��һ���ֽ� 0x00                             
//                SendBuf[SendLen++] = 0x00;
//            }
//            else
//            {
//                SendBuf[SendLen++] = ProductionCodeLen;
//                for (u8 i = 0; i < ProductionCodeLen; i++)
//                {                             
//                    SendBuf[SendLen++] = I2C1_ReadByte(EEPROM_ADDRESS, DEV_ENCODING + 1 + i);
//                }
//            }
            break;
            
        case 0x51E2:
            SendBuf[SendLen++] = 0xE2;
//            HardwareVersionLen = I2C1_ReadByte(EEPROM_ADDRESS, HWVERSION);
//            if((0 == HardwareVersionLen) || (0x32 < HardwareVersionLen))      
//            {
//                SendBuf[SendLen++] = 0x01;                                      //Ӧ��һ���ֽ� 0x00                             
//                SendBuf[SendLen++] = 0x00;
//            }
//            else
//            {
//                SendBuf[SendLen++] = HardwareVersionLen;
//                for (u8 i = 0; i < HardwareVersionLen; i++)
//                {                             
//                    SendBuf[SendLen++] = I2C1_ReadByte(EEPROM_ADDRESS, HWVERSION + 1 + i);
//                }
//            }
            break;
            
        case 0x51E3:
            SendBuf[SendLen++] = 0xE3;  //��ѯ����汾
            SoftwareVersionLen = 7;//I2C1_ReadByte(EEPROM_ADDRESS, SFVERSION);
            if((0 == SoftwareVersionLen) || (0x32 < SoftwareVersionLen))       
            {
                SendBuf[SendLen++] = 0x01;                                      //Ӧ��һ���ֽ� 0x00                             
                SendBuf[SendLen++] = 0x00;
            } 
            else
            {
                SendBuf[SendLen++] = SoftwareVersionLen;
                for (i = 0; i < SoftwareVersionLen; i++)
                {                             
                    SendBuf[SendLen++] = SensorSoftVersion[i+1];//(EEPROM_ADDRESS, SFVERSION + 1 + i);
                }
            }
            break;
            
        case 0x51E4:
            SendBuf[SendLen++] = 0xE4;
//            CustomerCodeLen = I2C1_ReadByte(EEPROM_ADDRESS, DEV_ID);
//            if((0 == CustomerCodeLen) || (0x32 < CustomerCodeLen))            //û��д����ȥ���򷵻ش�����
//            {
//                SendBuf[SendLen++] = 0x01;                                      //Ӧ��һ���ֽ� 0x00                             
//                SendBuf[SendLen++] = 0x00;
//            } 
//            else
//            {
//                SendBuf[SendLen++] = CustomerCodeLen;
//                for (u8 i = 0; i < CustomerCodeLen; i++)
//                {                             
//                    SendBuf[SendLen++] = I2C1_ReadByte(EEPROM_ADDRESS, DEV_ID + 1 + i);
//                }
//            }
            break;
            
        case 0x51E5:
            SendBuf[SendLen++] = 0xE5;
//            DeviceIdLen = I2C1_ReadByte(EEPROM_ADDRESS, CUSTOMERCODE);
//            if((0 == DeviceIdLen) || (0x32 < DeviceIdLen))                     //û��д����ȥ����ֱ�ӷ���
//            {
//                SendBuf[SendLen++] = 0x01;                                      //Ӧ��һ���ֽ� 0x00                             
//                SendBuf[SendLen++] = 0x00;
//            } 
//            else
//            {
//                SendBuf[SendLen++] = DeviceIdLen;
//                for (u8 i = 0; i < DeviceIdLen; i++)
//                {                             
//                    SendBuf[SendLen++] = I2C1_ReadByte(EEPROM_ADDRESS, CUSTOMERCODE + 1 + i);
//                }
//            }            
           break;            
            
        default:
            break; 
        }
    }
    MBASC_SendMsg(SendBuf, SendLen);
}


                
//******************************************************************************
// ����         : MBASC_Fun41()
// ��������     : 2018-06-08
// ����         : ÷����
// ����         : ����ϵͳ����boot��ʼִ�У�
// �������     : ��
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : ��
// �޸�����     : ��
//******************************************************************************
void MBASC_Fun41(void)
{
    uint16_t ReadAdr = 0;
    uint16_t DataLength = 0;
    uint8_t ReadData;
      
    ReadAdr = ((uint16_t)(USART_RX_BUF[2]<< 8)) + USART_RX_BUF[3];
    DataLength = ((uint16_t)(USART_RX_BUF[4]<< 8)) + USART_RX_BUF[5];
        
    SendLen = 0;
    SendBuf[SendLen++] = USART_RX_BUF[1] ? UserPara.SlaveAddr : 0x00;
    SendBuf[SendLen++] = 0x41;
    for(i = 2; i < 6; i++)
    {
        SendBuf[SendLen++] = USART_RX_BUF[i];
    }
    
    if((0x0001 != ReadAdr) || (0 != DataLength))
    {
        return;
    }
    else 
    {
        Eeprom_WriteByte(1023, "\x0C", 1);
        Eeprom_ReadByte(1023, &ReadData , 1);
        if(ReadData == 0x0C)
        {
            SendBuf[SendLen++] = 0x00;
            MBASC_SendMsg(SendBuf, SendLen);
//            while(0 != u8SendNum);                                              //�ȴ����ݷ�����ϣ��˾�Ҫ���ϣ�����ִ�д˹�����������Ӧ
            while(HAL_UART_Transmit(&UART1_Handler, SendBuf, SendLen, 100) == HAL_OK);
			NVIC_SystemReset();
        }
        else
        {
            SendBuf[SendLen++] = 0x01;
            MBASC_SendMsg(SendBuf, SendLen); 
        }
    } 
}     
     

//******************************************************************************
// ����         : MBASC_Function()
// ��������     : 2018-06-08
// ����         : ׯ��Ⱥ
// ����         : modbus asciiͨ�Ŵ���
// �������     : ��
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : ��
// �޸�����     : ��                                         
//******************************************************************************
void MBASC_Function(void)
{
    uint16_t RecvLen = 0;
    if(UartRecvFrameOK == Bit_SET)
    {
        if(2 == MODBUS_ASCII_RecvData(USART_RX_BUF, &RecvLen))//У�����
        {
            return;
        }  
        if(RecvLen <= 0)
        {
            return;                                                            //û�н��ܵ����ݣ�����Ӧ
        }

        else if((UserPara.SlaveAddr != USART_RX_BUF[0]) && (MB_ADDRESS_BROADCAST != USART_RX_BUF[0]))
        {
            return;                                                            //���յĴӻ���ַ����Ӧ������Ӧ
        }

        else
        {
            switch (USART_RX_BUF[1])
            {
              case 0x03:
                MBASC_Fun03();	                                                //�������Ĵ��������ֽ����ݣ�
                break;

              case 0x04:
                MBASC_Fun04();	                                                //��˫���Ĵ������������ݣ�
                break;
                
              case 0x05:     //�궨0�����ֵ��
                MBASC_Fun05();	                                               //д������Ȧ
                break;
                
              case 0x10:
                MBASC_Fun10();                                                  //д����Ĵ���
                break; 
                
              case 0x2B:
                MBASC_Fun2B();                                                  //д����Ĵ���
                break; 
     
              case 0x41:
                MBASC_Fun41();                                                  //����ϵͳ����boot��ʼִ�У�
                break; 
                
              default:
                SendLen = 0;
                SendBuf[SendLen++] = USART_RX_BUF[0];//MBASC_GetSlaveAddr(USART_RX_BUF);
                SendBuf[SendLen++] = 0x80 | USART_RX_BUF[1];
                SendBuf[SendLen++] = MB_EX_ILLEGAL_FUNCTION;
                MBASC_SendMsg(SendBuf, SendLen);
                break;
             }
         }
    }
}
