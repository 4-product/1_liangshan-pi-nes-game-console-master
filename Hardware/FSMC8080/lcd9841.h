#ifndef __LCD9841_H
#define __LCD9841_H	

#include "gd32f4xx.h"
#include "systick.h"
#include "lcd9841.h"
#include "stdio.h"



/********务必择数据位端口与并口串口选择,按键更新选择*********/
/************************按三步选择**************************/
/******按一步选择*********/
//并口，串口选择：
//1：并口:低8位，16位, 高8位
//0：串口:3线,4线
#define  PS 1

/*******按二步选择********
//LCD接口判读选择：读写功能函数  
//MCU=0,00: L8BIT 0K
//MCU=1,01: 16BIT	OK
//MCU=2,10: W4SPI	OK
//MCU=3,11: W3SPI	OK
//MCU=4,--: H8BIT	OK
//MCU=5,--: PC0-7用来作数据口：DB0-7	OK
***************/
#define  MCU  1

/********按三步选择*******/
//并口8/16位选择：ＦＳＭＣ配置
//1：16位
//0：L8位
//REG16,高8位,串口不用理会
#define  MCu8or16BIT  1

/*******按四步选择********/
//SD卡自动更新，按键更新选择20171125//
//1：按键更新画好面
//0：自动更新画好面
#define keypress 	1

/*********************ＥＤＮ*********************/
/*********************ＥＤＮ*********************/


/************************************************/
//LCD重要参数集
typedef struct  
{										    
	uint16_t  width;		//LCD 宽度
	uint16_t  height;		//LCD 高度
	uint16_t  id;				//LCD ID
	uint8_t   dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	uint16_t 	wramcmd;	//开始写gram指令
	uint16_t   setxcmd;	//设置x坐标指令
	uint16_t   setycmd;	//设置y坐标指令  
}_lcd_dev; 	  

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
//LCD的画笔颜色和背景色	   
extern uint16_t   POINT_COLOR;//默认红色    
extern uint16_t   BACK_COLOR; //背景颜色.默认为白色

////////////////////////////////////////////////
//-----------------LCD端口定义----------------//

/**************************************************************/
/*                STM32F4内部FSCM介绍                       */
/***************************************************************
STM32F4 的 FSMC 存储块 1（ Bank1）被分为 4 个区，每个区管理 64M 字节空间，
每个区都有独立的寄存器对所连接的存储器进行配置。
Bank1 的 256M 字节空间由 28 根地址线（ HADDR[27:0]）寻址。
这里 HADDR 是内部 AHB 地址 总线，其中 HADDR[25:0] 来自外部存储器地址FSMC_A[25:0]，
而 HADDR[26:27]对 4 个区进行寻址。FSMC_A[25:0]，2^26 =0X0400 0000 = 64MB,
每个BANK 有4*64MB = 256MB,而 HADDR[26:27]对 4 个区进行寻址。如下所示：
Bank1 所选区 片选信号 地址范围 [25:0]	 HADDR[27:26]
第 1 区 FSMC_NE1 0X6000,0000~63FF,FFFF 00
第 2 区 FSMC_NE2 0X6400,0000~67FF,FFFF 01    
第 3 区 FSMC_NE3 0X6800,0000~6BFF,FFFF 10
第 4 区 FSMC_NE4 0X6C00,0000~6FFF,FFFF 11

LCD_CS接的PD7(FSCM_NE1)所以选的NORSRAM1
选择BANK1-BORSRAM1 连接TFT,地址范围0X6000 0000 ~ 0X3FFF FFFF  
FSMC_A11接LCD的DC(寄存器数据/地址选择脚)
16位:A0无效,此时A1相当于真正A0,注意设置时STM32内部会右移一位对其!
RS:A11  0x3FF    加上无效的A0位得到0x3FF<<1=0x7FE 
寄存器地址 = 0X6000 0000
RAM基地址 =  0X6001 FFFE = 0X6000 0000+2^11（A11所以就是11次方）*2 = 0X6000 0000 + 0X001 FFFE = 0X6001 FFFE
地址线不同，要重新计算RAM基地址
*****************************************************************/
/****************************************************************/
/****************************************************************/

//====８位，１６位选择20171125====//
#if MCu8or16BIT   //16位

//LCD地址结构体 16BIT
typedef struct
{
	uint16_t  LCD_REG;
	uint16_t  LCD_RAM;
} LCD_TypeDef;

//使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A10作为数据命令区分线 
//注意设置时STM32内部会右移一位对其! 			    
//LCD>>16BIT FSMC   OK
#define LCD_BASE    ((uint32_t )(0x6c000000 |  0X000007FE))  //    111 1111 1111
#define LCD         ((LCD_TypeDef *) LCD_BASE)

#else	//8位

//LCD地址结构体8BIT
typedef struct
{
	uint8_t  LCD_REG;
	uint8_t  LCD_RAM;
} LCD_TypeDef;

//LCD>>8BIT FSMC  OK
#define LCD_BASE        ((uint32_t )(0x60000000 | 0X0FFFF))
#define LCD             ((LCD_TypeDef *) LCD_BASE)

#endif


//#define LCD_BLK PBout(1)	// DS0 
#define LCD_BLK_ON  gpio_bit_set(GPIOD,GPIO_PIN_13)	// PD13
#define LCD_BLK_OFF gpio_bit_reset(GPIOD,GPIO_PIN_13)	// PD13

//===SPI===//
#define SPI_CS(a)	\
						if (a)	\
						gpio_bit_set(GPIOD,GPIO_PIN_13)	\
						else		\
						gpio_bit_reset(GPIOD,GPIO_PIN_13)
						
#define SPI_SCL(a)	\
						if (a)	\
						gpio_bit_set(GPIOD,GPIO_PIN_13)	\
						else		\
						gpio_bit_reset(GPIOD,GPIO_PIN_13)
						
#define SPI_SDA(a)	\
						if (a)	\
						gpio_bit_set(GPIOD,GPIO_PIN_13)	\
						else		\
						gpio_bit_reset(GPIOD,GPIO_PIN_13)
						
#define SPI_A0(a)	\
						if (a)	\
						gpio_bit_set(GPIOD,GPIO_PIN_13)	\
						else		\
						gpio_bit_reset(GPIOD,GPIO_PIN_13)

//#define SPI_SCLK PDout(5)
//						
//#define SPI_SDIN PDout(4)	

//#define SPI_A0IN PDout(11)					
//						
/*
//参考写法
//如果使用快速IO，则定义下句，如果不使用，则去掉即可！
//使用快速IO，刷屏速率可以达到28帧每秒！
//普通IO，只能14帧每秒！   		
*/
						
//#ifdef LCD_FAST_IO //快速IO

#define	LCD_CS_SET  GPIOD->BSRRH=1<<7    //片选端口  	 PD7
#define	LCD_RS_SET	GPIOD->BSRRH=1<<11   //数据/命令  	 PD11	   
#define	LCD_WR_SET	GPIOD->BSRRH=1<<5    //写数据			 PD5
#define	LCD_RD_SET	GPIOD->BSRRH=1<<4    //读数据			 PD4
#define	LCD_RES_SET	GPIOD->BSRRH=1<<13   //读数据		   PD13
								    
#define	LCD_CS_CLR  GPIOD->BSRRL=1<<7     //片选端口  	 PD7
#define	LCD_RS_CLR	GPIOD->BSRRL=1<<11    //数据/命令		 PD11	   
#define	LCD_WR_CLR	GPIOD->BSRRL=1<<5     //写数据			 PD5
#define	LCD_RD_CLR	GPIOD->BSRRL=1<<4     //读数据			 PD4
#define	LCD_RES_CLR	GPIOD->BSRRL=1<<13    //读数据		   PD13

#define DATAOUTD(x) GPIOD->ODR=x; //数据输出    　
#define DATAIND     GPIOD->IDR;   //数据输入	 	

#define DATAOUTE(x) GPIOE->ODR=x; //数据输出    
#define DATAINE     GPIOE->IDR;   //数据输入	 		

//#else //慢速IO

#define	LCD_CS	PDout(7)  //片选端口  	    
#define	LCD_RS	PDout(11)  //数据/命令      	   
#define	LCD_WR	PDout(5)  //写数据			
#define	LCD_RD	PDout(4)  //读数据			
							    
//#endif

/*
//PB0~15,作为数据线
#define DATAOUT(x) GPIOB->ODR=x; //数据输出    　未调试
#define DATAIN     GPIOB->IDR;   //数据输入	 	未调试				

//仅参考放在这
//写8位数据函数
//用宏定义,提高速度.

#ifdef LCD_FAST_IO //快速IO
  #define LCD_WR_DATA8(DH,DL){\
      LCD_RS_SET;\
      LCD_CS_CLR;\
      DATAOUT(DH);\
      LCD_WR_CLR;\
      LCD_WR_SET;\
      DATAOUT(DL);\
      LCD_WR_CLR;\
      LCD_WR_SET;\
      LCD_CS_SET;\
      }

  #define LCD_WR_DATA(data){\
      unsigned char m,n;	\
      m=data>>8;\
      n=data;\
      LCD_WR_DATA8(m,n);\
     }  
#else  //正常IO
  #define LCD_WR_DATA8(DH,DL){\
      LCD_RS=1;\
      LCD_CS=0;\
      DATAOUT(DH);\
      LCD_WR=0;\
      LCD_WR=1;\
      DATAOUT(DL);\
      LCD_WR=0;\
      LCD_WR=1;\
      LCD_CS=1;\
      } 
  #define LCD_WR_DATA(data){\
       unsigned char m,n;	\
       m=data>>8;\
       n=data;\
       LCD_WR_DATA8(m,n);\
      }  	
#endif		
*/

///////////////////////////////////////////						
//扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左	 

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向

//画笔颜色
#define WHITE        0xFFFF
#define BLACK        0x0000	  
#define BLUE         0x001F  
#define BRED         0XF81F
#define GRED 			   0XFFE0
#define GBLUE			   0X07FF
#define RED          0xF800
#define MAGENTA      0xF81F
#define GREEN        0x07E0
#define CYAN         0x7FFF
#define YELLOW       0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			     0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

//*******************************************************************************//
void LCD_CtrlLinesConfig(void);
void LCD_FSMCConfig(void);

void LCD_Config(void);//ＬＣＤ配置操作

void LCD_SPICtrlLinesConfig(void); //针对ＳＰＩ单独配置
void LCD_H8BitCtrlLinesConfig(void); //针对高8位：单独配置20171204

void LCD_PC0_7CtrlLinesConfig(void); //针对PC0_7：单独配置20180314

void LCD_RESET(void);  //ＬＣＤ进行复位操作

void LCD_WriteByteSPI(unsigned char byte);

/*****4W-SPI******/
void SPI4W_WriteRegIndex(uint16_t  Index);
void SPI4W_WriteData(uint16_t  dat);
void SPI4W_WriteDisplayData(uint16_t  dat);

/*****3W-SPI******/
void SPI3W_WriteRegIndex(uint16_t  Index);
void SPI3W_WriteData(uint16_t  dat);
void SPI3W_WriteDisplayData(uint16_t  dat);

/*****高8BIT***20171203***/
void LCD_WR_REG8(uint16_t  Index);
void LCD_WR_DATA8(uint16_t  Data);

/*****8/16BIT******/
void LCD_WR_REG(uint16_t  regval); //16BIT REG
void LCD_WR_8REG(uint16_t  regval);//8BIT REG
void LCD_WR_DATA(uint16_t  data);
void LCD_WR_8DATA(uint16_t  data);

void LCD_WriteReg(uint16_t  LCD_Reg, uint16_t  LCD_RegValue);

uint16_t  LCD_RD_DATA(void);
uint16_t  LCD_RD_DATA8(void);

uint16_t  LCD_SPI_RD_DATA(void);
	
uint16_t  LCD_ReadReg(uint16_t  LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(uint16_t  RGB_Code);
uint16_t  LCD_BGR2RGB(uint16_t  c);
void opt_delay(uint8_t  i);
uint16_t   LCD_ReadPoint(uint16_t  x,uint16_t  y); 	//读点 

void LCD_Clear(uint16_t  Color);	//清屏
void LCD_Display(uint16_t  color);
void LCD_DisplayJiuGongGe(void);
void LCD_Frame(uint16_t  RGB_Data,uint16_t  f_data);
void Show_Gray(void);
void GRAY_TEST16(void);

void Check_Crosstalk(void);
void Check_Flicker(void);

void LCD_Fill(uint16_t  sx,uint16_t  sy,uint16_t  ex,uint16_t  ey,uint16_t  color);		   				//填充单色
void LCD_Color_Fill(uint16_t  sx,uint16_t  sy,uint16_t  ex,uint16_t  ey,uint16_t  *color);				//填充指定颜色

void LCD_Fill_Area( uint16_t  Xpos_S, uint16_t  Ypos_S, uint16_t  Xpos_E, uint16_t  Ypos_E, uint16_t  color );

void DisplayPicture(void);
void DisplayPicture1(void);

//void LCD_SetCursor(uint16_t  Xpos, uint16_t  Ypos);	//移放置在TFTLCD_Init.c    20171120//设置光标

void LCD_DrawPoint(uint16_t  x,uint16_t  y);											//画点
void LCD_Fast_DrawPoint(uint16_t  x,uint16_t  y,uint16_t  color);								//快速画点
void LCD_DrawLine(uint16_t  x1, uint16_t  y1, uint16_t  x2, uint16_t  y2);							//画线
void LCD_DrawRectangle(uint16_t  x1, uint16_t  y1, uint16_t  x2, uint16_t  y2);		   				//画矩形
void LCD_Draw_Circle(uint16_t  x0,uint16_t  y0,uint8_t  r);						 			//画圆

void LCD_ShowChar(uint16_t  x,uint16_t  y,uint8_t  num,uint8_t  size,uint8_t  mode);						//显示一个字符
uint32_t  LCD_Pow(uint8_t  m,uint8_t  n);
void LCD_ShowNum(uint16_t  x,uint16_t  y,uint32_t  num,uint8_t  len,uint8_t  size);  						//显示一个数字
void LCD_ShowxNum(uint16_t  x,uint16_t  y,uint32_t  num,uint8_t  len,uint8_t  size,uint8_t  mode);				//显示 数字
void LCD_ShowString(uint16_t  x,uint16_t  y,uint16_t  width,uint16_t  height,uint8_t  size,uint8_t  *p);		//显示一个字符串,12/16字体

void LCD_SSD_BackLightSet(uint8_t  pwm);							//SSD1963 背光控制
void SSD1963_SetRGB(void);

//LCD分辨率设置
#define SSD_HOR_RESOLUTION		800		//LCD水平分辨率
#define SSD_VER_RESOLUTION		480		//LCD垂直分辨率
//LCD驱动参数设置
#define SSD_HOR_PULSE_WIDTH		1		//水平脉宽
#define SSD_HOR_BACK_PORCH		210		//水平前廊
#define SSD_HOR_FRONT_PORCH		45		//水平后廊

#define SSD_VER_PULSE_WIDTH		1		//垂直脉宽
#define SSD_VER_BACK_PORCH		34		//垂直前廊
#define SSD_VER_FRONT_PORCH		10		//垂直前廊
//如下几个参数，自动计算
#define SSD_HT	(SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS	(SSD_HOR_BACK_PORCH)
#define SSD_VT 	(SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)

///////////SD///////////
uint16_t  pic_get_tnum(uint8_t  *path);
void SD_DisPic(void);
void Bin_ShowPic(unsigned short x,unsigned short y,char * fname);//20171201
void ShowPicBin(void);


void display_colorbar_H(void);

#endif
