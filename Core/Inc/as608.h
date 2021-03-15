#ifndef __AS608_H
#define __AS608_H
#include <stdio.h>
#include "stm32f1xx_hal.h"
#include "usart.h"

#define CharBuffer1 0x01
#define CharBuffer2 0x02
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

#define CMD_TYPE 	0x01
//#define PS_GetImage 0x01
//#define PS_GenChar	0x02
//#define PS_Match	0x03
//#define PS_Search	0x04
//#define PS_RegModel	0x05
//#define PS_StorChar	0x06
//#define PS_LoadChar	0x07
//#define PS_UpChar	0x08
//#define PS_DownChar	0x09
//#define PS_UpImage	0x0A
//#define PS_DownImg	0x0B
//#define PS_DletChar	0x0C
//#define PS_Empty	0x0D
//#define PS_WritReg	0x0E
//#define PS_ReadSys	0x0F
//#define PS_Enroll	0x10
//#define PS_Identify	0x11
//#define PS_SetPwd	0x12
//#define PS_VfyPwd	0x13
//#define PS_GetRand	0x14
//#define PS_SetAddr	0x15
//#define PS_RdInfPag	0x16
//#define PS_PortCtrl	0x17
//#define PS_WrNote	0x18
//#define PS_RdNote	0x19
//#define PS_HiSearch	0x1B
//#define PS_GeBinImg	0x1C


#define __MAXLEN 50

extern u32 AS608Addr;//模块地址

extern uint8_t RxBuff[100];
extern uint8_t TxBuff[__MAXLEN];
typedef struct  
{
	u16 pageID;//指纹ID
	u16 mathscore;//匹配得分
}SearchResult;

typedef struct
{
	u16 PS_max;//指纹最大容量
	u8  PS_level;//安全等级
	u32 PS_addr;
	u8  PS_size;//通讯数据包大小
	u8  PS_N;//波特率基数N
}SysPara;

void setFrame(void);
uint8_t PS_GetImage(void);
uint8_t PS_GenChar(uint8_t BufferID);
uint16_t PS_Match(uint16_t *score);
u8 PS_Search(u8 BufferID, u16 StartPage, u16 PageNum,uint16_t* pageID,uint16_t* score);
u8 PS_RegModel(void);
u8 PS_StoreChar(u8 BufferID, u16 PageID);
u8 PS_DeletChar(u16 PageID, u16 cnt);
u8 PS_Empty(void);
u8 PS_WriteReg(u8 RegNum, u8 DATA);
u8 PS_ReadSysPara(SysPara *p);
u8 PS_Enroll(u16 *pageID);
u8 PS_Identify(u16 *pageID,u16* score);
u8 PS_HighSpeedSearch(u8 BufferID, u16 StartPage, u16 PageNum, uint16_t* pageID,uint16_t* score);
void PS_BetterEnroll(uint8_t times);
const char *EnsureMessage(u8 ensure);


#endif

