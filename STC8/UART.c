#include <STC8H.h>
#include "stdio.h"	
#include "string.h"

char usercmd[32]="AT";

void Delay1000ms(void)	//@11.0592MHz
{
	unsigned char data i, j, k;

	i = 57;
	j = 27;
	k = 112;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


// 初始化串口
void UART_Init() {
    SCON = 0x50;  // 8位数据，可变波特率，允许接收
    TMOD &= 0x0F;
    TMOD |= 0x20;  // 定时器1工作在模式2（自动重装载）
    TH1 = 0xFD;    // 波特率9600，晶振11.0592MHz
    TL1 = 0xFD;
    TR1 = 1;       // 启动定时器1
    ES = 1;        // 使能串口中断
    EA = 1;        // 使能总中断
}

// 串口发送一个字符
void UART_SendChar(char ch) {
    SBUF = ch;     // 将字符写入发送缓冲区
    while (!TI);   // 等待发送完成
    TI = 0;        // 清除发送完成标志
}

// 串口发送字符串
void UART_SendString(char *str) {
    while (*str) {
        UART_SendChar(*str++);
    }
}

// 串口中断服务函数
void UART_ISR() interrupt 4 {
    if (RI) {
        char receivedChar = SBUF;  // 读取接收到的字符
        UART_SendChar(receivedChar); // 将接收到的字符原样返回
        RI = 0;  // 清除接收完成标志
    }
}

void main() {
    UART_Init();  // 初始化串口
    UART_SendString("Hello, STC8H UART!\r\n");  // 发送欢迎信息
    while (1) {
        // 主循环可以处理其他任务
		if(strcmp(usercmd,"AT")==0)		  //测试匹配函数
		{
		     UART_SendString("ATOK!\r\n");			
		}
		Delay1000ms();
    }
}


//下面是参考代码
//
//int fputc(int ch,FILE *p)  //函数默认的，在使用printf函数时自动调用
//{
//	USART_SendData(USART2,(u8)ch);	
//	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
//	return ch;
//}
//
//
//
//char * user_after(char *instr,char *markstr)
//{
//	char *p;
//	int i;
//	if(strstr(instr,markstr)!=NULL)
//  {
//    p=strstr(instr,markstr); //找到位置
//		i=strlen(markstr);       //找到尺寸
//		p+=i;                    
//		return p;
//	}
//	else
//  {
//		p=NULL;
//	}
//	return 0;
//}
//
//
//
//
//char * user_before(char *instr,char *markstr)
//{
//	int len_max,i;
//	static char tmp[128];
//	tmp[0]='\0';
//	if(strstr(instr,markstr)!=NULL)
//  {
//		len_max=strlen(instr);
//		i=strlen( strstr(instr,markstr) );
//		strncpy(tmp,instr,len_max-i);
//		tmp[len_max-i]='\0';
//	}
//	return tmp;
//}

//
//
//应用方法
//		if(strcmp(USART2_RX_BUF,"AT+LCDFactory")==0)LCD_ChangePage(LCD_PageAddress_Factory);//切换页面
//	
//		if(strstr(USART2_RX_BUF,"AT+SetDate")!=NULL)    //设置日期  格式：AT+SetDate=230510   表示23年5月10日
//		{
//			 strbuf = user_after(USART2_RX_BUF,"AT+SetDate=");
//			 i0buf=atoi(strbuf);
//			 DDD_D;
//			 DS1302_Read_Time();        //获取当前时间
//			 timeNow.year  = (i0buf/100000)     *16+(i0buf/10000)%10;  //计算年出的写入数值     获取第4第5位  然后再转为芯片数据
//			 timeNow.month =((i0buf%10000)/1000)*16+(i0buf/100)  %10;  //计算月出的写入数值
//			 timeNow.day   =((i0buf%100)/10)    *16+(i0buf/1)    %10;  //计算日出的写入数值
//			 DS1302_Set_Time(timeNow.year,0x00,timeNow.month,timeNow.day,timeNow.hour,timeNow.minute,timeNow.second);  
//			 printf("AT+%d\r\n",i0buf);
//		}	
//
//

