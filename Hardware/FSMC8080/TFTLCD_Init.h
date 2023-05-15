#ifndef __TFTLCD_Init_H
#define __TFTLCD_Init_H	

#include "gd32f4xx.h"
#include "systick.h"

/****************************/
#define SubQCIF	96RGB128
#define SQQVGA  128RGB128
#define QQVGA  	128RGB160
#define QCIF  	176RGB220
#define QVGA  	240RGB320
#define WQVGA  	240RGB400
#define HVGA  	320RGB480
#define	VGA		480RGB640
#define WVGA  	480RGB800
#define FWVGA  	480RGB854
#define QHD  	540RGB960
#define HD720  	720RGB1280
#define WSVGA   1024RGB600
#define	XGA		1024RGB768
#define	WXGA	1280RGB800
#define	Full_HD	1920RGB1080
/******************************/


extern	uint8_t  lcd_id[12];

void LCD_Init(void);	//初始化
void LCD_Set(void);
void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend);
void LCD_SetCursor(uint16_t  Xpos, uint16_t  Ypos); 	//设置光标位置 
void TFT_Scan_Dir(uint8_t  dir);	//设置LCD的自动扫描方向
void LCD_ReadID(void);	//IC ID 

void TFT_WriteRAM(void); //开始写GRAM
void LCD_DisplayOn(void);	 //开显示
void LCD_DisplayOff(void);	//关显示

/*************以下开始初始化*******************/
//1.44-1.77
void ILI9163_CTC177_Initial(void);
void ILI9163C_CPT144_Initial(void);
void INI_ST7735S_CPT144(void);
void INI_ST7735S_CTC177(void);
void INI_ST7735S_IVO177(void);
void INI_ST7735S_HSD177(void);
void INI_HX8353C_BOE177(void);
void INI_HX8353E_TM177(void);
void INI_HX8353D_TM177(void);
void INI_GC9102D_BOE177(void);
void INI_GC9106_BOE177(void);

//2.0-2.2
void ILI9225G_TM_20_Initial(void);
void ILI9338_TM20_Initial(void);
void ILI9225B_CPT20_Initial(void);

//2.3-2.4,2.8,3.2
void ST7789S_IVO24_Initial(void);
void ST7789S_HSD154IPS_Initial(void);
void ST7789V_HSD13IPS_Initial(void);
void ST7789h2_BOE154panelinitialcode(void);
void ILI9341_CMI24_Initial(void);
void ILI9341_LB200_Initial(void);
void ILI9340_LG24IPS_Initial(void);
void ILI9341_IVO24_Initial(void);
void ILI9341_HSD26_Initial(void);
void GC9312_HSD26_Initial(void);
void GC9304C_HSD26_Initial(void);

void ILI9328_AUO24_Initial(void);
void ILI9325D_AUO24_Initial(void);
void ILI9325_ILI9328_CPT26_Initial(void);
void LCD_Initial_R61580(void);
void LCD_Initial_LG4535(void);
void LCD_Initial_SPFD5408B(void);
void RM68090_BOE24_Initial(void);

//3.0
void ILI9327_CPT30_Initial(void);
void S6D04D1_JDI28_Initial(void);
void R61509V_HSD36_Initial(void);

//3.5
void ILI9488_CMI35IPS(void);
void ILI9488_CTC35(void);

void ILI9486L_CMI35TN(void);
void ILI9486L_IVO35TN(void);

void HX8357D_TM350IPS_Initial(void);
void HX8357C_CPT350TN_Initial(void);

void HX8357C_TM350TN_Initial(void);

void HX8357C_HSD350TN_Initial(void);
void HX8357B_HSD350TN_Initial(void);

void HX8357B_CMO3509_3511TN_Initial(void);

void ILI9341_CPT28_Initial(void);
void ILI9341_TM24_Initial(void);

void ILI9481_BOE35_init(void);
void ILI9481_CPT35_Initial_Code(void);
void ILI9481_HY35_Initial_Code1(void);
void ILI9481_HY35_Initial_Code2(void);
void ILI9481_HY35_Initial_Code3(void);
void ILI9481_TM35_Initial_Code(void);
void ILI9481_CMI350IPS_Initial_Code(void);

void ILI9486_HY35_Initial_Code(void);
void ILI9486_HY35_Initial2_Code(void);

void LcdILI9481ReadID(void);

void NT35310_HSD35_Initial_Code(void);

void CLAN040LE21_OTM8009_MCU(void);

/*************END******************/

/************************************************/
/************************************************/
/************************************************/
///////////////各种点阵////////////////
void LCD_SQQVGA(void); 	//128RGB128
void LCD_QQVGA(void);	  //128RGB160
void LCD_QCIF(void)	;	  //176RGB220
void LCD_QVGA(void)	;	  //240RGB320
void LCD_QVGA1(void);	  //240RGB240
void LCD_ILI9328(void);	//240RGB320
void LCD_WQVGA(void);	  //240RGB400	
void LCD_HVGA(void);	  //320RGB480
void LCD_HVGA_ILI9481(void);
void LCD_WVGA8009(void);	//480RGB800
void LCD_FWVGA8009(void);	//480RGB854

/////////////各窗口////////////
void LCDBlock2A2B(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend); //9163,9331,9340,9341,7735,7789,8357D,9327,9488,HX8357C//OK
void LCDBlockST7735S_CPT144(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend); ////7735S-CPT144    OK
void LCDBlockST7735S_CTC177(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend); ////7735S-CTC177  20180102 OK
void LCDBlockGC9106_BOE177(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend); ////GC9106-BOE177  20180105 OK

void LCDBlockILI9225(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend);  //ILI9225系列

void LCDBlockILI9328(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend);  //ILI9325,RM68090系列 
void LCDBlockR61580(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend);

void LCDBlockSPFD5408(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend); 
void LCDBlockILI9481(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend);
void LCDBlockOTM8009(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend); //OTM8009
void LCDBlockHX(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend);   // HX8347,8340,8352 
void LCDBlockSSD2119(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend);  //SSD2119
void LCDBlockOTM4001(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend);  //OTM4001,R61509 
void LCDBlockOTM2001(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend);  //OTM2001

/*******************************************************************************/
///////////////光标////////////////
void LCD2A2B_SetCursor(uint16_t  Xpos, uint16_t  Ypos); //9163,9340,9341,7735,7789,8357D,9327,9488,HX8357C//OK
void LCD_ST7735S_CPT144_SetCursor(uint16_t  Xpos, uint16_t  Ypos);//20180103 OK
void LCD_ST7735S_CTC177_SetCursor(uint16_t  Xpos, uint16_t  Ypos);	//20180102  OK
void LCD_GC9106_BOE177_SetCursor(uint16_t  Xpos, uint16_t  Ypos);	//20180105  OK
void LCDHX8353D_SetCursor(uint16_t  Xpos, uint16_t  Ypos); //HX8353D   OK20171217
void LCDILI9225_SetCursor(uint16_t  Xpos, uint16_t  Ypos); //9225   OK 20171101

void LCDILI9328_SetCursor(uint16_t  Xpos, uint16_t  Ypos); //ILI9325,RM68090系列 

void LCDOTM8009SetCursor(uint16_t  Xpos, uint16_t  Ypos); //OTM8009  OK
void LCDHVG35SetCursor(uint16_t  Xpos, uint16_t  Ypos);  //ILI9488 ,HX8357C,TEST 20171101_OK要加后面的013F,01DF
void LCD_ILI9481SetCursor(uint16_t  Xpos, uint16_t  Ypos); //ILI9481 20180328 OK
void LCDR61509SetCursor(uint16_t  Xpos, uint16_t  Ypos); //

/*******************************************************************************/
///////////////LCD-ID////////////////
void LcdILI9163ReadID(void);  //OK
void LcdST7735ReadID(void);	  //OK
void LcdGC9106ReadID(void);
void LcdHX8353DReadID(void);
void LcdILI9225ReadID(void);  //OK
void LcdST7789ReadID(void);   //OK

void LcdGC9102ReadID(void);
void LcdGC9312ReadID(void);

void LcdILI9328ReadID(void);

void LcdILI9341ReadID(void);  //OK
void LcdILI9341_4SPIReadID(void); //20171201 TEST

void LcdHX8352BReadID(void);  //OK
void LcdILI9488ReadID(void);  //OK
void LcdOTM8009ReadID(void);  //OK
void LcdHX8357CReadID(void);  //OK
void LcdHX8357BReadID(void);  //OK

void LcdILI9327ReadID(void);  //OK
void LcdS6D04D1ReadID(void);

void LcdHX8357DReadID(void);  //NG
void LcdHX8357cReadID(void); 

#endif 
