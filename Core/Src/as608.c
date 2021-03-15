
#include <string.h>
#include "as608.h"


u32 AS608Addr = 0XFFFFFFFF; //默认
extern __IO uint8_t readSta;
uint8_t RxBuff[100];
uint8_t TxBuff[__MAXLEN] = {0};

void setFrame(void)
{
	TxBuff[0] = 0xEF;
	TxBuff[1] = 0x01;
	TxBuff[2] = AS608Addr >> 24;
	TxBuff[3] = AS608Addr >> 16;
	TxBuff[4] = AS608Addr >> 8;
	TxBuff[5] = AS608Addr;
	for(int i = 6;i<__MAXLEN;++i)
	{
		TxBuff[i] = 0;
	}
}
uint8_t PS_GetImage(void)
{
	HAL_StatusTypeDef xx;
	uint8_t packType = 0x01;		
	uint16_t packLen = 0x03;
	uint8_t cmd = 0x01;				//pack command
	uint16_t sum=0;					//frame sum
	
	uint16_t ansLen = 3;				// answer pack length
	
	uint8_t temp = 6;				//....just temp
	
	setFrame();
	
	sum = packType + packLen + cmd;
	
	TxBuff[temp++] = packType;
	
	TxBuff[temp++] = packLen>>8;
	TxBuff[temp++] = packLen;
	
	TxBuff[temp++] = cmd;
	
	TxBuff[temp++] = sum>>8;
	TxBuff[temp++] = sum;
	
	HAL_UART_Transmit(&huart2,TxBuff,temp,200);

	sum=0;
	temp = ansLen+9;
	xx = HAL_UART_Receive(&huart2,RxBuff,temp,5000);
	if (xx!= HAL_OK)
	{
			return RxBuff[9];
	}
	else
		return 0xff;
}

uint8_t PS_GenChar(uint8_t BufferID)
{
	uint8_t packType = 0x01;		
	uint16_t packLen = 0x04;
	uint8_t cmd = 0x02;				//pack command
	uint16_t sum=0;					//frame sum
	
	uint16_t ansLen = 3;				// answer pack length
	
	uint8_t temp = 6;				//....just temp
	
	setFrame();
	//////////////
	sum = packType + packLen + cmd + BufferID;
	//////////////
	TxBuff[temp++] = packType;
	
	TxBuff[temp++] = packLen>>8;
	TxBuff[temp++] = packLen;
	
	TxBuff[temp++] = cmd;
	TxBuff[temp++] = BufferID;
	TxBuff[temp++] = sum>>8;
	TxBuff[temp++] = sum;
	
	HAL_UART_Transmit(&huart2,TxBuff,temp,200);
	
	sum=0;
	
	temp = ansLen+9;
	if (HAL_UART_Receive(&huart2,RxBuff,temp,2000) == HAL_OK)
	{
		for(int i = 6; i<temp-2; i++)
		{
			sum+=RxBuff[i];
		}
		if(sum == RxBuff[temp-1]+(RxBuff[temp-2]<<8))
			return RxBuff[9];
		return 0xff;
	}
	else
		return 0xff;
}

uint16_t PS_Match(uint16_t *score)
{
	uint8_t packType = 0x01;		
	uint16_t packLen = 0x03;
	uint8_t cmd = 0x03;				//pack command
	uint16_t ansLen = 5;				// answer pack length
	uint16_t sum=0;					//frame sum
	uint8_t temp = 6;				//....just temp
	setFrame();
	sum = packType + packLen + cmd;
	
	TxBuff[temp++] = packType;
	TxBuff[temp++] = packLen>>8;
	TxBuff[temp++] = packLen;
	TxBuff[temp++] = cmd;
	///--->Here
	
	
	TxBuff[temp++] = sum>>8;
	TxBuff[temp++] = sum;
	
	HAL_UART_Transmit(&huart2,TxBuff,temp,200);
	
	sum=0;
	
	temp = ansLen+9;
	if (HAL_UART_Receive(&huart2,RxBuff,temp,2000) == HAL_OK)
	{
		sum = RxBuff[6]+((RxBuff[7]<<8)+RxBuff[8])+RxBuff[9]+((RxBuff[10]<<8)+RxBuff[11]);
		if(sum == RxBuff[temp-1]+(RxBuff[temp-2]<<8))
		{
			*score = (RxBuff[10]<<8)+RxBuff[11];
			return RxBuff[9];
		}
		return 0xff;
	}
	else
		return 0xff;
}

u8 PS_Search(u8 BufferID, u16 StartPage, u16 PageNum,uint16_t* pageID,uint16_t* score)
{
	uint8_t packType = 0x01;		
	uint16_t packLen = 0x08;
	uint8_t cmd = 0x04;				//pack command
	uint16_t ansLen = 7;				// answer pack length
	
	uint16_t sum=0;					//frame sum
	uint8_t temp = 6;				//....just temp
	
	setFrame();
	sum = packType + packLen + cmd + BufferID + StartPage + PageNum;
	
	TxBuff[temp++] = packType;
	TxBuff[temp++] = packLen>>8;
	TxBuff[temp++] = packLen;
	TxBuff[temp++] = cmd;
	///--->Here
	TxBuff[temp++] = BufferID;
	TxBuff[temp++] = StartPage>>8;
	TxBuff[temp++] = StartPage;
	TxBuff[temp++] = PageNum>>8;
	TxBuff[temp++] = PageNum;
	TxBuff[temp++] = sum>>8;
	TxBuff[temp++] = sum;
	
	HAL_UART_Transmit(&huart2,TxBuff,temp,200);
	
	sum=0;
	
	temp = ansLen+9;
	if (HAL_UART_Receive(&huart2,RxBuff,temp,2000) == HAL_OK)
	{
		sum += RxBuff[6];			//anspack identify
		sum += (RxBuff[7]<<8)+RxBuff[8];		//ansLen
		sum += RxBuff[9];					//yes code	
		sum += (RxBuff[10]<<8)+RxBuff[11];	//page id 
		sum += (RxBuff[12]<<8)+RxBuff[13];	//score
		if(sum == RxBuff[temp-1]+(RxBuff[temp-2]<<8))
		{
			*pageID = (RxBuff[10]<<8)+RxBuff[11];	//page id 
			*score  = (RxBuff[12]<<8)+RxBuff[13];	//score
			return RxBuff[9];
		}
		return 0xff;
	}
	else
		return 0xff;
}

u8 PS_RegModel(void)
{
	uint8_t packType = 0x01;		
	uint16_t packLen = 0x03;
	uint8_t cmd = 0x05;				//pack command
	uint16_t ansLen = 3;				// answer pack length
	
	uint16_t sum=0;					//frame sum
	uint8_t temp = 6;				//....just temp
	
	setFrame();
	sum = packType + packLen + cmd;
	
	TxBuff[temp++] = packType;
	TxBuff[temp++] = packLen>>8;
	TxBuff[temp++] = packLen;
	TxBuff[temp++] = cmd;
	///--->Here
	TxBuff[temp++] = sum>>8;
	TxBuff[temp++] = sum;
	
	HAL_UART_Transmit(&huart2,TxBuff,temp,200);
	
	sum=0;
	
	temp = ansLen+9;
	if (HAL_UART_Receive(&huart2,RxBuff,temp,2000) == HAL_OK)
	{
		sum += RxBuff[6];			//anspack identify
		sum += (RxBuff[7]<<8)+RxBuff[8];		//ansLen
		sum += RxBuff[9];					//yes code	
		if(sum == RxBuff[temp-1]+(RxBuff[temp-2]<<8))
		{
			return RxBuff[9];
		}
		return 0xff;
	}
	else
		return 0xff;
}

u8 PS_StoreChar(u8 BufferID, u16 PageID)
{
	uint8_t packType = 0x01;		
	uint16_t packLen = 0x06;
	uint8_t cmd = 0x06;				//pack command
	uint16_t ansLen = 3;				// answer pack length
	
	uint16_t sum=0;					//frame sum
	uint8_t temp = 6;				//....just temp
	
	setFrame();
	sum = packType + packLen + cmd + BufferID + PageID;
	
	TxBuff[temp++] = packType;
	TxBuff[temp++] = packLen>>8;
	TxBuff[temp++] = packLen;
	TxBuff[temp++] = cmd;
	///--->Here
	TxBuff[temp++] = BufferID;
	TxBuff[temp++] = PageID>>8;
	TxBuff[temp++] = PageID;
	TxBuff[temp++] = sum>>8;
	TxBuff[temp++] = sum;
	
	HAL_UART_Transmit(&huart2,TxBuff,temp,200);
	
	sum=0;
	
	temp = ansLen+9;
	if (HAL_UART_Receive(&huart2,RxBuff,temp,2000) == HAL_OK)
	{
		sum += RxBuff[6];			//anspack identify
		sum += (RxBuff[7]<<8)+RxBuff[8];		//ansLen
		sum += RxBuff[9];					//yes code	
		if(sum == RxBuff[temp-1]+(RxBuff[temp-2]<<8))
		{
			return RxBuff[9];
		}
		return 0xff;
	}
	else
		return 0xff;
}

u8 PS_DeletChar(u16 PageID, u16 cnt)
{
	uint8_t packType = 0x01;		
	uint16_t packLen = 0x07;
	uint8_t cmd = 0x0c;				//pack command
	uint16_t ansLen = 3;				// answer pack length
	
	uint16_t sum=0;					//frame sum
	uint8_t temp = 6;				//....just temp
	
	setFrame();
	sum = packType + packLen + cmd + PageID + cnt;
	
	TxBuff[temp++] = packType;
	TxBuff[temp++] = packLen>>8;
	TxBuff[temp++] = packLen;
	TxBuff[temp++] = cmd;
	///--->Here
	TxBuff[temp++] = PageID>>8;
	TxBuff[temp++] = PageID;
	TxBuff[temp++] = cnt>>8;
	TxBuff[temp++] = cnt;
	TxBuff[temp++] = sum>>8;
	TxBuff[temp++] = sum;
	
	HAL_UART_Transmit(&huart2,TxBuff,temp,200);
	
	sum=0;
	
	temp = ansLen+9;
	if (HAL_UART_Receive(&huart2,RxBuff,temp,2000) == HAL_OK)
	{
		sum += RxBuff[6];			//anspack identify
		sum += (RxBuff[7]<<8)+RxBuff[8];		//ansLen
		sum += RxBuff[9];					//yes code	
		if(sum == RxBuff[temp-1]+(RxBuff[temp-2]<<8))
		{
			return RxBuff[9];
		}
		return 0xff;
	}
	else
		return 0xff;
}

u8 PS_Empty(void)
{
	uint8_t packType = 0x01;		
	uint16_t packLen = 0x03;
	uint8_t cmd = 0x0d;				//pack command
	uint16_t ansLen = 3;				// answer pack length
	
	uint16_t sum=0;					//frame sum
	uint8_t temp = 6;				//....just temp
	
	setFrame();
	sum = packType + packLen + cmd;
	
	TxBuff[temp++] = packType;
	TxBuff[temp++] = packLen>>8;
	TxBuff[temp++] = packLen;
	TxBuff[temp++] = cmd;
	///--->Here
	TxBuff[temp++] = sum>>8;
	TxBuff[temp++] = sum;
	
	HAL_UART_Transmit(&huart2,TxBuff,temp,200);
	
	sum=0;
	
	temp = ansLen+9;
	if (HAL_UART_Receive(&huart2,RxBuff,temp,2000) == HAL_OK)
	{
		sum += RxBuff[6];			//anspack identify
		sum += (RxBuff[7]<<8)+RxBuff[8];		//ansLen
		sum += RxBuff[9];					//yes code	
		if(sum == RxBuff[temp-1]+(RxBuff[temp-2]<<8))
		{
			return RxBuff[9];
		}
		return 0xff;
	}
	else
		return 0xff;
}

u8 PS_WriteReg(u8 RegNum, u8 DATA)
{
	uint8_t packType = 0x01;		
	uint16_t packLen = 0x05;
	uint8_t cmd = 0x0e;				//pack command
	uint16_t ansLen = 3;				// answer pack length
	
	uint16_t sum=0;					//frame sum
	uint8_t temp = 6;				//....just temp
	
	setFrame();
	sum = packType + packLen + cmd + RegNum + DATA;
	
	TxBuff[temp++] = packType;
	TxBuff[temp++] = packLen>>8;
	TxBuff[temp++] = packLen;
	TxBuff[temp++] = cmd;
	///--->Here
	TxBuff[temp++] = RegNum;
	TxBuff[temp++] = DATA;
	TxBuff[temp++] = sum>>8;
	TxBuff[temp++] = sum;
	
	HAL_UART_Transmit(&huart2,TxBuff,temp,200);
	sum=0;
	temp = ansLen+9;
	if (HAL_UART_Receive(&huart2,RxBuff,temp,2000) == HAL_OK)
	{
		sum += RxBuff[6];			//anspack identify
		sum += (RxBuff[7]<<8)+RxBuff[8];		//ansLen
		sum += RxBuff[9];					//yes code	
		if(sum == RxBuff[temp-1]+(RxBuff[temp-2]<<8))
		{
			return RxBuff[9];
		}
		return 0xff;
	}
	else
		return 0xff;
}

u8 PS_ReadSysPara(SysPara *p)
{
	uint8_t packType = 0x01;		
	uint16_t packLen = 0x03;
	uint8_t cmd = 0x0f;				//pack command
	uint16_t ansLen = 19;				// answer pack length
	
	uint16_t sum=0;					//frame sum
	uint8_t temp = 6;				//....just temp
	setFrame();

	sum = packType + packLen + cmd;
	
	TxBuff[temp++] = packType;
	TxBuff[temp++] = packLen>>8;
	TxBuff[temp++] = packLen;
	TxBuff[temp++] = cmd;
	///--->Here
	TxBuff[temp++] = sum>>8;
	TxBuff[temp++] = sum;
	
	HAL_UART_Transmit(&huart2,TxBuff,temp,200);
	sum=0;
	temp = ansLen+9;		//28
	if (HAL_UART_Receive(&huart2,RxBuff,temp,2000) == HAL_OK)
	{
		sum += RxBuff[6];			//anspack identify
		sum += (RxBuff[7]<<8)+RxBuff[8];		//ansLen
		sum += RxBuff[9];					//yes code	
//		sum += (RxBuff[10]<<8)+RxBuff[11];	//status reg	
//		sum += (RxBuff[12]<<8)+RxBuff[13];	//sensor type
//		sum += (RxBuff[14]<<8)+RxBuff[15];	//libray size
//		sum += (RxBuff[16]<<8)+RxBuff[17];	//sercty level
//		sum += (RxBuff[20]<<8)+RxBuff[21];	//device address
//		sum += (RxBuff[22]<<8)+RxBuff[23];	//data pack size
		sum += (RxBuff[24]<<8)+RxBuff[25];	//baudrate
		printf("%d\n",sum);
//		if(sum == RxBuff[temp-1]+(RxBuff[temp-2]<<8))
		if(1)
		{
			p->PS_addr=(RxBuff[18]<<24)+(RxBuff[19]<<16)+(RxBuff[20]<<8)+RxBuff[21];
			p->PS_level= (RxBuff[16]<<8)+RxBuff[17];
			p->PS_max=(RxBuff[14]<<8)+RxBuff[15];
			p->PS_N=(RxBuff[24]<<8)+RxBuff[25];
			p->PS_size=(RxBuff[22]<<8)+RxBuff[23];
			return RxBuff[9];
		}
	}
	else
		return 0xff;
}

u8 PS_Enroll(u16 *pageID)
{
	uint8_t packType = 0x01;		
	uint16_t packLen = 0x03;
	uint8_t cmd = 0x10;				//pack command
	uint16_t ansLen = 5;				// answer pack length
	
	uint16_t sum=0;					//frame sum
	uint8_t temp = 6;				//....just temp
	
	setFrame();
	sum = packType + packLen + cmd;
	
	TxBuff[temp++] = packType;
	TxBuff[temp++] = packLen>>8;
	TxBuff[temp++] = packLen;
	TxBuff[temp++] = cmd;
	///--->Here	
	TxBuff[temp++] = sum>>8;
	TxBuff[temp++] = sum;
	
	HAL_UART_Transmit(&huart2,TxBuff,temp,200);
	sum=0;
	temp = ansLen+9;
	if (HAL_UART_Receive(&huart2,RxBuff,temp,2000) == HAL_OK)
	{
		sum += RxBuff[6];			//anspack identify
		sum += (RxBuff[7]<<8)+RxBuff[8];		//ansLen
		sum += RxBuff[9];					//yes code	
		sum += (RxBuff[10]<<8)+RxBuff[11]; 	//pageID
		if(sum == RxBuff[temp-1]+(RxBuff[temp-2]<<8))
		{
			*pageID = (RxBuff[10]<<8)+RxBuff[11];
			return RxBuff[9];
		}
		return 0xff;
	}
	else
		return 0xff;
}
u8 PS_Identify(u16 *pageID,u16* score)
{
	uint8_t packType = 0x01;		
	uint16_t packLen = 0x03;
	uint8_t cmd = 0x11;				//pack command
	uint16_t ansLen = 7;				// answer pack length
	
	uint16_t sum=0;					//frame sum
	uint8_t temp = 6;				//....just temp
	
	setFrame();
	sum = packType + packLen + cmd;
	
	TxBuff[temp++] = packType;
	TxBuff[temp++] = packLen>>8;
	TxBuff[temp++] = packLen;
	TxBuff[temp++] = cmd;
	///--->Here	
	TxBuff[temp++] = sum>>8;
	TxBuff[temp++] = sum;
	
	HAL_UART_Transmit(&huart2,TxBuff,temp,200);
	sum=0;
	temp = ansLen+9;
	if (HAL_UART_Receive(&huart2,RxBuff,temp,2000) == HAL_OK)
	{
		sum += RxBuff[6];			//anspack identify
		sum += (RxBuff[7]<<8)+RxBuff[8];		//ansLen
		sum += RxBuff[9];					//yes code	
		sum += (RxBuff[10]<<8)+RxBuff[11]; 	//pageID
		sum += (RxBuff[12]<<8)+RxBuff[13]; 	//score
		if(sum == RxBuff[temp-1]+(RxBuff[temp-2]<<8))
		{
			*pageID = (RxBuff[10]<<8)+RxBuff[11];
			*score = (RxBuff[12]<<8)+RxBuff[13];
			return RxBuff[9];
		}
		return 0xff;
	}
	else
		return 0xff;
}


u8 PS_HighSpeedSearch(u8 BufferID, u16 StartPage, u16 PageNum, uint16_t* pageID,uint16_t* score)
{
	uint8_t packType = 0x01;		
	uint16_t packLen = 0x08;
	uint8_t cmd = 0x04;				//pack command
	uint16_t ansLen = 7;				// answer pack length
	
	uint16_t sum=0;					//frame sum
	uint8_t temp = 6;				//....just temp
	
	setFrame();
	sum = packType + packLen + cmd + BufferID + StartPage + PageNum;
	
	TxBuff[temp++] = packType;
	TxBuff[temp++] = packLen>>8;
	TxBuff[temp++] = packLen;
	TxBuff[temp++] = cmd;
	///--->Here
	TxBuff[temp++] = BufferID;
	TxBuff[temp++] = StartPage>>8;
	TxBuff[temp++] = StartPage;
	TxBuff[temp++] = PageNum>>8;
	TxBuff[temp++] = PageNum;
	TxBuff[temp++] = sum>>8;
	TxBuff[temp++] = sum;
	
	HAL_UART_Transmit(&huart2,TxBuff,temp,200);
	
	sum=0;
	
	temp = ansLen+9;
	if (HAL_UART_Receive(&huart2,RxBuff,temp,2000) == HAL_OK)
	{
		sum += RxBuff[6];			//anspack identify
		sum += (RxBuff[7]<<8)+RxBuff[8];		//ansLen
		sum += RxBuff[9];					//yes code	
		sum += (RxBuff[10]<<8)+RxBuff[11];	//page id 
		sum += (RxBuff[12]<<8)+RxBuff[13];	//score
		if(sum == RxBuff[temp-1]+(RxBuff[temp-2]<<8))
		{
			*pageID = (RxBuff[10]<<8)+RxBuff[11];	//page id 
			*score  = (RxBuff[12]<<8)+RxBuff[13];	//score
			return RxBuff[9];
		}
			return 0xff;
	}
	else
		return 0xff;
}

void PS_BetterEnroll(uint8_t times)
{
	uint8_t read_result;
	while(!readSta) 
	{
		printf("Please place your finger on, %d more times is needed...\r\n",times);
		HAL_Delay(500);
	}
	HAL_Delay(100);
	read_result = PS_GetImage();
	printf("%d\n",read_result);
	while(readSta)
	{
		printf("Please lift your finger\r\n");
		HAL_Delay(500);
	}
	if(!PS_GenChar(2))
	{
			printf("Gen Template success\r\n");
	}	
	times--;
	while(times)
	{
		while(!readSta) 
		{
			printf("Please place your finger on, %d more times is needed...\r\n",times);
			HAL_Delay(500);
		}
		HAL_Delay(100);
		PS_GetImage();
		while(readSta)
		{
			printf("Please lift your finger\r\n");
			HAL_Delay(500);
		}
		if(!PS_GenChar(1))
		{
			printf("Gen Template success\r\n");
			if(!PS_RegModel())
			{
				times--;
				printf("Marge templates success\r\n");
			}
			else
				printf("Gen Template fail,plz retry\r\n");
		}
		else
			printf("Marge templates fail,plz retry\n");
	}
	if(!PS_StoreChar(2, 0))
		printf("----------------------------Store Finger in FLASH:0 SUCCESS----------------------------\r\n");
	else
		printf("----------------------------Store Finger in FLASH:0 [FAIL]----------------------------\r\n");
}
const char *EnsureMessage(u8 ensure)
{
	const char* p;
	switch (ensure)
	{
	case 0x00:
		p = "OK";
		break;
	case 0x01:
		p = "���ݰ����մ���";
		break;
	case 0x02:
		p = "��������û����ָ";
		break;
	case 0x03:
		p = "¼��ָ��ͼ��ʧ��";
		break;
	case 0x04:
		p = "ָ��ͼ��̫�ɡ�̫��������������";
		break;
	case 0x05:
		p = "ָ��ͼ��̫ʪ��̫��������������";
		break;
	case 0x06:
		p = "ָ��ͼ��̫�Ҷ�����������";
		break;
	case 0x07:
		p = "ָ��ͼ����������������̫�٣������̫С��������������";
		break;
	case 0x08:
		p = "ָ�Ʋ�ƥ��";
		break;
	case 0x09:
		p = "û������ָ��";
		break;
	case 0x0a:
		p = "�����ϲ�ʧ��";
		break;
	case 0x0b:
		p = "����ָ�ƿ�ʱ��ַ��ų���ָ�ƿⷶΧ";
	case 0x10:
		p = "ɾ��ģ��ʧ��";
		break;
	case 0x11:
		p = "���ָ�ƿ�ʧ��";
		break;
	case 0x15:
		p = "��������û����Чԭʼͼ��������ͼ��";
		break;
	case 0x18:
		p = "��д FLASH ����";
		break;
	case 0x19:
		p = "δ�������";
		break;
	case 0x1a:
		p = "��Ч�Ĵ�����";
		break;
	case 0x1b:
		p = "�Ĵ����趨���ݴ���";
		break;
	case 0x1c:
		p = "���±�ҳ��ָ������";
		break;
	case 0x1f:
		p = "ָ�ƿ���";
		break;
	case 0x20:
		p = "��ַ����";
		break;
	default:
		p = "ģ�鷵��ȷ��������";
		break;
	}
	return p;
}
