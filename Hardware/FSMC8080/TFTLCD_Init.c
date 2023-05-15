#include "TFTLCD_Init.h"
#include "stdio.h"
#include "lcd9841.h"


///////////////////////////
uint8_t  lcd_id[12]; //存放LCD ID字符串
uint8_t  lcd_dot[24]; //存放LCD 点阵字符串
/*************以下初始化与相关设置代码*******************/
void NT35510_HY35_Initial_Code3(void);   ///黄仕周代码20200803
/*************************************/

void LCD_Init(void)
{
  /********对ＬＣＤ初始化前进行复位**********/
  LCD_RESET();   //对显示屏务必上电复位	
  /*************对ＬＣＤ复位*****************/
  /**********对ＬＣＤ屏参数设置**************/	
  LCD_Set();     //设置显示屏参数	 需要设置，显示功能都用到设置的．	
	//printf("GWDBG[%s %d]\r\n", __FUNCTION__, __LINE__);
  /*************以下开始初始化*******************/
  /*************以下开始初始化*******************/
	//ILI9481_HY35_Initial_Code3(); ///20200803
	NT35510_HY35_Initial_Code3();

	

}


/*************以下开始初始化*******************/	
/*************以下开始初始化*******************/	
#define LCD_ILI9481_CMD  LCD_WR_8REG
#define LCD_ILI9481_INDEX LCD_WR_8DATA
#define Delayms  delay_1ms

void ILI9481_HY35_Initial_Code3(void)   ///黄仕周代码20200803
{
//亮晶晶ILI9481_HYDIS35_GS22—TO黑屏   better20210809
LCD_ILI9481_CMD(0x11);
Delayms(200);
LCD_ILI9481_CMD(0x11);
Delayms(200);
//2 ok20200803
LCD_ILI9481_CMD(0xD0);
LCD_ILI9481_INDEX(0x07);
LCD_ILI9481_INDEX(0x42);
LCD_ILI9481_INDEX(0x1b);

LCD_ILI9481_CMD(0xD1);
LCD_ILI9481_INDEX(0x00);
LCD_ILI9481_INDEX(0x05);
LCD_ILI9481_INDEX(0x0c);

LCD_ILI9481_CMD(0xD2);
LCD_ILI9481_INDEX(0x01);
LCD_ILI9481_INDEX(0x11);

LCD_ILI9481_CMD(0xC0);
LCD_ILI9481_INDEX(0x00);
LCD_ILI9481_INDEX(0x3B);
LCD_ILI9481_INDEX(0x00);
LCD_ILI9481_INDEX(0x02);
LCD_ILI9481_INDEX(0x11);

LCD_ILI9481_CMD(0xC5);
LCD_ILI9481_INDEX(0x02);

LCD_ILI9481_CMD(0xC8);
LCD_ILI9481_INDEX(0x00);
LCD_ILI9481_INDEX(0x22);
LCD_ILI9481_INDEX(0x37);
LCD_ILI9481_INDEX(0x20);
LCD_ILI9481_INDEX(0x04);
LCD_ILI9481_INDEX(0x00);
LCD_ILI9481_INDEX(0x04);
LCD_ILI9481_INDEX(0x55);
LCD_ILI9481_INDEX(0x77);
LCD_ILI9481_INDEX(0x02);
LCD_ILI9481_INDEX(0x04);
LCD_ILI9481_INDEX(0x00);

///20210809
///1
LCD_ILI9481_CMD(0xF0);
LCD_ILI9481_INDEX(0x00);
LCD_ILI9481_CMD(0xF6);
LCD_ILI9481_INDEX(0x84);  ///0x80
LCD_ILI9481_CMD(0xF3);
LCD_ILI9481_INDEX(0x02);
LCD_ILI9481_INDEX(0x1A);
LCD_ILI9481_CMD(0xF7);
LCD_ILI9481_INDEX(0x80);

///2
//LCD_ILI9481_CMD(0xF0); 
//LCD_ILI9481_INDEX(0x01); 
//LCD_ILI9481_CMD(0xF6);
//LCD_ILI9481_INDEX(0x80);
//LCD_ILI9481_CMD(0xF3); 
//LCD_ILI9481_INDEX(0x40); 
//LCD_ILI9481_INDEX(0x0A); 
//LCD_ILI9481_CMD(0xF7); 
//LCD_ILI9481_INDEX(0x80); 

LCD_ILI9481_CMD(0x36);
LCD_ILI9481_INDEX(0x0A);

LCD_ILI9481_CMD(0x3A);
LCD_ILI9481_INDEX(0x55);

Delayms(120);
LCD_ILI9481_CMD(0x29);
}	



void NT35510_HY35_Initial_Code3(void)   ///黄仕周代码20200803
{
	  LCD_WriteReg(0xF000,0x55);
		LCD_WriteReg(0xF001,0xAA);
		LCD_WriteReg(0xF002,0x52);
		LCD_WriteReg(0xF003,0x08);
		LCD_WriteReg(0xF004,0x01);//page1
		//AVDD Set AVDD 5.2V
		LCD_WriteReg(0xB000,0x0D);
		LCD_WriteReg(0xB001,0x0D);
		LCD_WriteReg(0xB002,0x0D);
		//AVDD ratio
		LCD_WriteReg(0xB600,0x34);
		LCD_WriteReg(0xB601,0x34);
		LCD_WriteReg(0xB602,0x34);
		//AVEE -5.2V
		LCD_WriteReg(0xB100,0x0D);
		LCD_WriteReg(0xB101,0x0D);
		LCD_WriteReg(0xB102,0x0D);
		//AVEE ratio
		LCD_WriteReg(0xB700,0x34);
		LCD_WriteReg(0xB701,0x34);
		LCD_WriteReg(0xB702,0x34);
		//VCL -2.5V
		LCD_WriteReg(0xB200,0x00);
		LCD_WriteReg(0xB201,0x00);
		LCD_WriteReg(0xB202,0x00);
		//VCL ratio
		LCD_WriteReg(0xB800,0x24);
		LCD_WriteReg(0xB801,0x24);
		LCD_WriteReg(0xB802,0x24);
		//VGH 15V (Free pump)
		LCD_WriteReg(0xBF00,0x01);
		LCD_WriteReg(0xB300,0x0F);
		LCD_WriteReg(0xB301,0x0F);
		LCD_WriteReg(0xB302,0x0F);
		//VGH ratio
		LCD_WriteReg(0xB900,0x34);
		LCD_WriteReg(0xB901,0x34);
		LCD_WriteReg(0xB902,0x34);
		//VGL_REG -10V
		LCD_WriteReg(0xB500,0x08);
		LCD_WriteReg(0xB501,0x08);
		LCD_WriteReg(0xB502,0x08);
		LCD_WriteReg(0xC200,0x03);
		//VGLX ratio
		LCD_WriteReg(0xBA00,0x24);
		LCD_WriteReg(0xBA01,0x24);
		LCD_WriteReg(0xBA02,0x24);
		//VGMP/VGSP 4.5V/0V
		LCD_WriteReg(0xBC00,0x00);
		LCD_WriteReg(0xBC01,0x78);
		LCD_WriteReg(0xBC02,0x00);
		//VGMN/VGSN -4.5V/0V
		LCD_WriteReg(0xBD00,0x00);
		LCD_WriteReg(0xBD01,0x78);
		LCD_WriteReg(0xBD02,0x00);
		//VCOM
		LCD_WriteReg(0xBE00,0x00);
		LCD_WriteReg(0xBE01,0x64);
		//Gamma Setting
		LCD_WriteReg(0xD100,0x00);
		LCD_WriteReg(0xD101,0x33);
		LCD_WriteReg(0xD102,0x00);
		LCD_WriteReg(0xD103,0x34);
		LCD_WriteReg(0xD104,0x00);
		LCD_WriteReg(0xD105,0x3A);
		LCD_WriteReg(0xD106,0x00);
		LCD_WriteReg(0xD107,0x4A);
		LCD_WriteReg(0xD108,0x00);
		LCD_WriteReg(0xD109,0x5C);
		LCD_WriteReg(0xD10A,0x00);
		LCD_WriteReg(0xD10B,0x81);
		LCD_WriteReg(0xD10C,0x00);
		LCD_WriteReg(0xD10D,0xA6);
		LCD_WriteReg(0xD10E,0x00);
		LCD_WriteReg(0xD10F,0xE5);
		LCD_WriteReg(0xD110,0x01);
		LCD_WriteReg(0xD111,0x13);
		LCD_WriteReg(0xD112,0x01);
		LCD_WriteReg(0xD113,0x54);
		LCD_WriteReg(0xD114,0x01);
		LCD_WriteReg(0xD115,0x82);
		LCD_WriteReg(0xD116,0x01);
		LCD_WriteReg(0xD117,0xCA);
		LCD_WriteReg(0xD118,0x02);
		LCD_WriteReg(0xD119,0x00);
		LCD_WriteReg(0xD11A,0x02);
		LCD_WriteReg(0xD11B,0x01);
		LCD_WriteReg(0xD11C,0x02);
		LCD_WriteReg(0xD11D,0x34);
		LCD_WriteReg(0xD11E,0x02);
		LCD_WriteReg(0xD11F,0x67);
		LCD_WriteReg(0xD120,0x02);
		LCD_WriteReg(0xD121,0x84);
		LCD_WriteReg(0xD122,0x02);
		LCD_WriteReg(0xD123,0xA4);
		LCD_WriteReg(0xD124,0x02);
		LCD_WriteReg(0xD125,0xB7);
		LCD_WriteReg(0xD126,0x02);
		LCD_WriteReg(0xD127,0xCF);
		LCD_WriteReg(0xD128,0x02);
		LCD_WriteReg(0xD129,0xDE);
		LCD_WriteReg(0xD12A,0x02);
		LCD_WriteReg(0xD12B,0xF2);
		LCD_WriteReg(0xD12C,0x02);
		LCD_WriteReg(0xD12D,0xFE);
		LCD_WriteReg(0xD12E,0x03);
		LCD_WriteReg(0xD12F,0x10);
		LCD_WriteReg(0xD130,0x03);
		LCD_WriteReg(0xD131,0x33);
		LCD_WriteReg(0xD132,0x03);
		LCD_WriteReg(0xD133,0x6D);
		LCD_WriteReg(0xD200,0x00);
		LCD_WriteReg(0xD201,0x33);
		LCD_WriteReg(0xD202,0x00);
		LCD_WriteReg(0xD203,0x34);
		LCD_WriteReg(0xD204,0x00);
		LCD_WriteReg(0xD205,0x3A);
		LCD_WriteReg(0xD206,0x00);
		LCD_WriteReg(0xD207,0x4A);
		LCD_WriteReg(0xD208,0x00);
		LCD_WriteReg(0xD209,0x5C);
		LCD_WriteReg(0xD20A,0x00);

		LCD_WriteReg(0xD20B,0x81);
		LCD_WriteReg(0xD20C,0x00);
		LCD_WriteReg(0xD20D,0xA6);
		LCD_WriteReg(0xD20E,0x00);
		LCD_WriteReg(0xD20F,0xE5);
		LCD_WriteReg(0xD210,0x01);
		LCD_WriteReg(0xD211,0x13);
		LCD_WriteReg(0xD212,0x01);
		LCD_WriteReg(0xD213,0x54);
		LCD_WriteReg(0xD214,0x01);
		LCD_WriteReg(0xD215,0x82);
		LCD_WriteReg(0xD216,0x01);
		LCD_WriteReg(0xD217,0xCA);
		LCD_WriteReg(0xD218,0x02);
		LCD_WriteReg(0xD219,0x00);
		LCD_WriteReg(0xD21A,0x02);
		LCD_WriteReg(0xD21B,0x01);
		LCD_WriteReg(0xD21C,0x02);
		LCD_WriteReg(0xD21D,0x34);
		LCD_WriteReg(0xD21E,0x02);
		LCD_WriteReg(0xD21F,0x67);
		LCD_WriteReg(0xD220,0x02);
		LCD_WriteReg(0xD221,0x84);
		LCD_WriteReg(0xD222,0x02);
		LCD_WriteReg(0xD223,0xA4);
		LCD_WriteReg(0xD224,0x02);
		LCD_WriteReg(0xD225,0xB7);
		LCD_WriteReg(0xD226,0x02);
		LCD_WriteReg(0xD227,0xCF);
		LCD_WriteReg(0xD228,0x02);
		LCD_WriteReg(0xD229,0xDE);
		LCD_WriteReg(0xD22A,0x02);
		LCD_WriteReg(0xD22B,0xF2);
		LCD_WriteReg(0xD22C,0x02);
		LCD_WriteReg(0xD22D,0xFE);
		LCD_WriteReg(0xD22E,0x03);
		LCD_WriteReg(0xD22F,0x10);
		LCD_WriteReg(0xD230,0x03);
		LCD_WriteReg(0xD231,0x33);
		LCD_WriteReg(0xD232,0x03);
		LCD_WriteReg(0xD233,0x6D);
		LCD_WriteReg(0xD300,0x00);
		LCD_WriteReg(0xD301,0x33);
		LCD_WriteReg(0xD302,0x00);
		LCD_WriteReg(0xD303,0x34);
		LCD_WriteReg(0xD304,0x00);
		LCD_WriteReg(0xD305,0x3A);
		LCD_WriteReg(0xD306,0x00);
		LCD_WriteReg(0xD307,0x4A);
		LCD_WriteReg(0xD308,0x00);
		LCD_WriteReg(0xD309,0x5C);
		LCD_WriteReg(0xD30A,0x00);

		LCD_WriteReg(0xD30B,0x81);
		LCD_WriteReg(0xD30C,0x00);
		LCD_WriteReg(0xD30D,0xA6);
		LCD_WriteReg(0xD30E,0x00);
		LCD_WriteReg(0xD30F,0xE5);
		LCD_WriteReg(0xD310,0x01);
		LCD_WriteReg(0xD311,0x13);
		LCD_WriteReg(0xD312,0x01);
		LCD_WriteReg(0xD313,0x54);
		LCD_WriteReg(0xD314,0x01);
		LCD_WriteReg(0xD315,0x82);
		LCD_WriteReg(0xD316,0x01);
		LCD_WriteReg(0xD317,0xCA);
		LCD_WriteReg(0xD318,0x02);
		LCD_WriteReg(0xD319,0x00);
		LCD_WriteReg(0xD31A,0x02);
		LCD_WriteReg(0xD31B,0x01);
		LCD_WriteReg(0xD31C,0x02);
		LCD_WriteReg(0xD31D,0x34);
		LCD_WriteReg(0xD31E,0x02);
		LCD_WriteReg(0xD31F,0x67);
		LCD_WriteReg(0xD320,0x02);
		LCD_WriteReg(0xD321,0x84);
		LCD_WriteReg(0xD322,0x02);
		LCD_WriteReg(0xD323,0xA4);
		LCD_WriteReg(0xD324,0x02);
		LCD_WriteReg(0xD325,0xB7);
		LCD_WriteReg(0xD326,0x02);
		LCD_WriteReg(0xD327,0xCF);
		LCD_WriteReg(0xD328,0x02);
		LCD_WriteReg(0xD329,0xDE);
		LCD_WriteReg(0xD32A,0x02);
		LCD_WriteReg(0xD32B,0xF2);
		LCD_WriteReg(0xD32C,0x02);
		LCD_WriteReg(0xD32D,0xFE);
		LCD_WriteReg(0xD32E,0x03);
		LCD_WriteReg(0xD32F,0x10);
		LCD_WriteReg(0xD330,0x03);
		LCD_WriteReg(0xD331,0x33);
		LCD_WriteReg(0xD332,0x03);
		LCD_WriteReg(0xD333,0x6D);
		LCD_WriteReg(0xD400,0x00);
		LCD_WriteReg(0xD401,0x33);
		LCD_WriteReg(0xD402,0x00);
		LCD_WriteReg(0xD403,0x34);
		LCD_WriteReg(0xD404,0x00);
		LCD_WriteReg(0xD405,0x3A);
		LCD_WriteReg(0xD406,0x00);
		LCD_WriteReg(0xD407,0x4A);
		LCD_WriteReg(0xD408,0x00);
		LCD_WriteReg(0xD409,0x5C);
		LCD_WriteReg(0xD40A,0x00);
		LCD_WriteReg(0xD40B,0x81);

		LCD_WriteReg(0xD40C,0x00);
		LCD_WriteReg(0xD40D,0xA6);
		LCD_WriteReg(0xD40E,0x00);
		LCD_WriteReg(0xD40F,0xE5);
		LCD_WriteReg(0xD410,0x01);
		LCD_WriteReg(0xD411,0x13);
		LCD_WriteReg(0xD412,0x01);
		LCD_WriteReg(0xD413,0x54);
		LCD_WriteReg(0xD414,0x01);
		LCD_WriteReg(0xD415,0x82);
		LCD_WriteReg(0xD416,0x01);
		LCD_WriteReg(0xD417,0xCA);
		LCD_WriteReg(0xD418,0x02);
		LCD_WriteReg(0xD419,0x00);
		LCD_WriteReg(0xD41A,0x02);
		LCD_WriteReg(0xD41B,0x01);
		LCD_WriteReg(0xD41C,0x02);
		LCD_WriteReg(0xD41D,0x34);
		LCD_WriteReg(0xD41E,0x02);
		LCD_WriteReg(0xD41F,0x67);
		LCD_WriteReg(0xD420,0x02);
		LCD_WriteReg(0xD421,0x84);
		LCD_WriteReg(0xD422,0x02);
		LCD_WriteReg(0xD423,0xA4);
		LCD_WriteReg(0xD424,0x02);
		LCD_WriteReg(0xD425,0xB7);
		LCD_WriteReg(0xD426,0x02);
		LCD_WriteReg(0xD427,0xCF);
		LCD_WriteReg(0xD428,0x02);
		LCD_WriteReg(0xD429,0xDE);
		LCD_WriteReg(0xD42A,0x02);
		LCD_WriteReg(0xD42B,0xF2);
		LCD_WriteReg(0xD42C,0x02);
		LCD_WriteReg(0xD42D,0xFE);
		LCD_WriteReg(0xD42E,0x03);
		LCD_WriteReg(0xD42F,0x10);
		LCD_WriteReg(0xD430,0x03);
		LCD_WriteReg(0xD431,0x33);
		LCD_WriteReg(0xD432,0x03);
		LCD_WriteReg(0xD433,0x6D);
		LCD_WriteReg(0xD500,0x00);
		LCD_WriteReg(0xD501,0x33);
		LCD_WriteReg(0xD502,0x00);
		LCD_WriteReg(0xD503,0x34);
		LCD_WriteReg(0xD504,0x00);
		LCD_WriteReg(0xD505,0x3A);
		LCD_WriteReg(0xD506,0x00);
		LCD_WriteReg(0xD507,0x4A);
		LCD_WriteReg(0xD508,0x00);
		LCD_WriteReg(0xD509,0x5C);
		LCD_WriteReg(0xD50A,0x00);
		LCD_WriteReg(0xD50B,0x81);

		LCD_WriteReg(0xD50C,0x00);
		LCD_WriteReg(0xD50D,0xA6);
		LCD_WriteReg(0xD50E,0x00);
		LCD_WriteReg(0xD50F,0xE5);
		LCD_WriteReg(0xD510,0x01);
		LCD_WriteReg(0xD511,0x13);
		LCD_WriteReg(0xD512,0x01);
		LCD_WriteReg(0xD513,0x54);
		LCD_WriteReg(0xD514,0x01);
		LCD_WriteReg(0xD515,0x82);
		LCD_WriteReg(0xD516,0x01);
		LCD_WriteReg(0xD517,0xCA);
		LCD_WriteReg(0xD518,0x02);
		LCD_WriteReg(0xD519,0x00);
		LCD_WriteReg(0xD51A,0x02);
		LCD_WriteReg(0xD51B,0x01);
		LCD_WriteReg(0xD51C,0x02);
		LCD_WriteReg(0xD51D,0x34);
		LCD_WriteReg(0xD51E,0x02);
		LCD_WriteReg(0xD51F,0x67);
		LCD_WriteReg(0xD520,0x02);
		LCD_WriteReg(0xD521,0x84);
		LCD_WriteReg(0xD522,0x02);
		LCD_WriteReg(0xD523,0xA4);
		LCD_WriteReg(0xD524,0x02);
		LCD_WriteReg(0xD525,0xB7);
		LCD_WriteReg(0xD526,0x02);
		LCD_WriteReg(0xD527,0xCF);
		LCD_WriteReg(0xD528,0x02);
		LCD_WriteReg(0xD529,0xDE);
		LCD_WriteReg(0xD52A,0x02);
		LCD_WriteReg(0xD52B,0xF2);
		LCD_WriteReg(0xD52C,0x02);
		LCD_WriteReg(0xD52D,0xFE);
		LCD_WriteReg(0xD52E,0x03);
		LCD_WriteReg(0xD52F,0x10);
		LCD_WriteReg(0xD530,0x03);
		LCD_WriteReg(0xD531,0x33);
		LCD_WriteReg(0xD532,0x03);
		LCD_WriteReg(0xD533,0x6D);
		LCD_WriteReg(0xD600,0x00);
		LCD_WriteReg(0xD601,0x33);
		LCD_WriteReg(0xD602,0x00);
		LCD_WriteReg(0xD603,0x34);
		LCD_WriteReg(0xD604,0x00);
		LCD_WriteReg(0xD605,0x3A);
		LCD_WriteReg(0xD606,0x00);
		LCD_WriteReg(0xD607,0x4A);
		LCD_WriteReg(0xD608,0x00);
		LCD_WriteReg(0xD609,0x5C);
		LCD_WriteReg(0xD60A,0x00);
		LCD_WriteReg(0xD60B,0x81);

		LCD_WriteReg(0xD60C,0x00);
		LCD_WriteReg(0xD60D,0xA6);
		LCD_WriteReg(0xD60E,0x00);
		LCD_WriteReg(0xD60F,0xE5);
		LCD_WriteReg(0xD610,0x01);
		LCD_WriteReg(0xD611,0x13);
		LCD_WriteReg(0xD612,0x01);
		LCD_WriteReg(0xD613,0x54);
		LCD_WriteReg(0xD614,0x01);
		LCD_WriteReg(0xD615,0x82);
		LCD_WriteReg(0xD616,0x01);
		LCD_WriteReg(0xD617,0xCA);
		LCD_WriteReg(0xD618,0x02);
		LCD_WriteReg(0xD619,0x00);
		LCD_WriteReg(0xD61A,0x02);
		LCD_WriteReg(0xD61B,0x01);
		LCD_WriteReg(0xD61C,0x02);
		LCD_WriteReg(0xD61D,0x34);
		LCD_WriteReg(0xD61E,0x02);
		LCD_WriteReg(0xD61F,0x67);
		LCD_WriteReg(0xD620,0x02);
		LCD_WriteReg(0xD621,0x84);
		LCD_WriteReg(0xD622,0x02);
		LCD_WriteReg(0xD623,0xA4);
		LCD_WriteReg(0xD624,0x02);
		LCD_WriteReg(0xD625,0xB7);
		LCD_WriteReg(0xD626,0x02);
		LCD_WriteReg(0xD627,0xCF);
		LCD_WriteReg(0xD628,0x02);
		LCD_WriteReg(0xD629,0xDE);
		LCD_WriteReg(0xD62A,0x02);
		LCD_WriteReg(0xD62B,0xF2);
		LCD_WriteReg(0xD62C,0x02);
		LCD_WriteReg(0xD62D,0xFE);
		LCD_WriteReg(0xD62E,0x03);
		LCD_WriteReg(0xD62F,0x10);
		LCD_WriteReg(0xD630,0x03);
		LCD_WriteReg(0xD631,0x33);
		LCD_WriteReg(0xD632,0x03);
		LCD_WriteReg(0xD633,0x6D);
		//LV2 Page 0 enable
		LCD_WriteReg(0xF000,0x55);
		LCD_WriteReg(0xF001,0xAA);
		LCD_WriteReg(0xF002,0x52);
		LCD_WriteReg(0xF003,0x08);
		LCD_WriteReg(0xF004,0x00);//page 0
		//Display control
		LCD_WriteReg(0xB100, 0xCC);
		LCD_WriteReg(0xB101, 0x00);
		//Source hold time
		LCD_WriteReg(0xB600,0x05);
		//Gate EQ control
		LCD_WriteReg(0xB700,0x70);
		LCD_WriteReg(0xB701,0x70);
		//Source EQ control (Mode 2)
		LCD_WriteReg(0xB800,0x01);
		LCD_WriteReg(0xB801,0x03);
		LCD_WriteReg(0xB802,0x03);
		LCD_WriteReg(0xB803,0x03);
		//Inversion mode (2-dot)
		LCD_WriteReg(0xBC00,0x02);
		LCD_WriteReg(0xBC01,0x00);
		LCD_WriteReg(0xBC02,0x00);
		//Timing control 4H w/ 4-delay
		LCD_WriteReg(0xC900,0xD0);
		LCD_WriteReg(0xC901,0x02);
		LCD_WriteReg(0xC902,0x50);
		LCD_WriteReg(0xC903,0x50);
		LCD_WriteReg(0xC904,0x50);
		
		LCD_WriteReg(0x3500,0x00);
		//LCD_WriteReg(0x3600,0x20);  //MY MX MV ML RGB MH RSMX RSMY
		LCD_WriteReg(0x3A00,0x55);  //16-bit/pixel
				
		LCD_WR_REG(0x1100);
		delay_1ms(120);
		LCD_WR_REG(0x2900);
}

//===========END INITIAL=========//
void LCD_HVGA_NT35510(void)	;
/**********************************************
函数名：LCD_Set
功能：Lcd设置相关寄存器参数集， 选择设置三种中一种就可以
返回值：无
***********************************************/
void LCD_Set(void)
{
	//LCD_HVGA_ILI9481();
	LCD_HVGA_NT35510();
}


///////////////各种点阵////////////////
void LCD_HVGA_ILI9481(void)	 
{
//9481
lcddev.width=320;    //LCD 宽度
lcddev.height=480;   //LCD 高度
//	//横向设置
//lcddev.width=480;    //LCD 宽度
//lcddev.height=320;   //LCD 高度	
lcddev.setxcmd=0X2A;  //设置x坐标指令2A
lcddev.setycmd=0X2B;  //设置y坐标指令2B
lcddev.wramcmd=0X2C;  //开始写gram指令
}	

///////////////各种点阵////////////////
void LCD_HVGA_NT35510(void)	 
{
//9481
lcddev.width=480;    //LCD 宽度
lcddev.height=800;   //LCD 高度
//	//横向设置
//lcddev.width=480;    //LCD 宽度
//lcddev.height=320;   //LCD 高度	
lcddev.setxcmd=0X2A00;  //设置x坐标指令2A
lcddev.setycmd=0X2B00;  //设置y坐标指令2B
lcddev.wramcmd=0X2C00;  //开始写gram指令
}	

/*******************************************************************************
* Function Name  : LCD_WriteRAM_Prepare  开始写GRAM
* Description    : 开始写IC-GRAM寄存器函数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TFT_WriteRAM(void)
{
  LCD_WR_REG(lcddev.wramcmd);
  //LCD_WR_8REG(lcddev.wramcmd);	
}

//LCD开启显示
void LCD_DisplayOn(void)
{					   
  //LCD_WR_REG(0X29);	//开启显示
  LCD_WR_8REG(0X29);
}	 

//LCD关闭显示
void LCD_DisplayOff(void)
{	   
  //LCD_WR_REG(0X28);	//关闭显示
  LCD_WR_8REG(0X28);
}

/**********************************************
函数名：Lcd块选函数
功能：选定Lcd上指定的矩形区域    选择设置三种中一种就可以
注意：xStart、yStart、Xend、Yend随着屏幕的旋转而改变，位置是矩形框的四个角
入口参数：xStart x方向的起始点
	  ySrart y方向的起始点
	  Xend   y方向的终止点
	  Yend   y方向的终止点
返回值：无
***********************************************/
void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend) 
{	
	
  //LCDBlockILI9481( Xstart,Xend,Ystart,Yend);
	
  //LCDBlock2A2B( Xstart,Xend,Ystart,Yend);

	if(lcddev.id==0x9481) {	LCDBlockILI9481( Xstart,Xend,Ystart,Yend); 	}
	else if(lcddev.id==0X9486) {	LCDBlock2A2B( Xstart,Xend,Ystart,Yend); 	 }	
	else if(lcddev.id==0X8000){

		LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(Xstart>>8);  
		LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(Xstart&0XFF);	  
		LCD_WR_REG(lcddev.setxcmd+2);LCD_WR_DATA(Xend>>8);   
		LCD_WR_REG(lcddev.setxcmd+3);LCD_WR_DATA(Xend&0XFF);   
		LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(Ystart>>8);   
		LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(Ystart&0XFF);  
		LCD_WR_REG(lcddev.setycmd+2);LCD_WR_DATA(Yend>>8);   
		LCD_WR_REG(lcddev.setycmd+3);LCD_WR_DATA(Yend&0XFF); 
		
	  //LCD_WR_8REG(0x2C);
	}
}

/****************************************************/

///////////////各种ＩＣ窗口////////////////
void LCDBlock2A2B(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend) 
{
	
 //9163,9331,9340,9341,7735,7789,8357D,9327,9488,HX8357C//OK 已验证20171127

  LCD_WR_8REG(0x2A);   
  LCD_WR_8DATA(Xstart>>8);
  LCD_WR_8DATA(Xstart&0xff);
  LCD_WR_8DATA(Xend>>8);
  LCD_WR_8DATA(Xend&0xff);

  LCD_WR_8REG(0x2B);   
  LCD_WR_8DATA(Ystart>>8);
  LCD_WR_8DATA(Ystart&0xff);
  LCD_WR_8DATA(Yend>>8);
  LCD_WR_8DATA(Yend&0xff);

  LCD_WR_8REG(0x2C);
}

void LCDBlockILI9481(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend)
{
  //////ili9481  20180728  OK
//	Xend-=1;
//	Yend-=1;	
  //////ili9481  
  LCD_WR_8REG(lcddev.setxcmd);   
  LCD_WR_8DATA(Xstart>>8);
  LCD_WR_8DATA(Xstart&0xff);
  LCD_WR_8DATA(Xend>>8);
  LCD_WR_8DATA(Xend&0xff);

  LCD_WR_8REG(lcddev.setycmd);   
  LCD_WR_8DATA(Ystart>>8);
  LCD_WR_8DATA(Ystart&0xff);
  LCD_WR_8DATA(Yend>>8);
  LCD_WR_8DATA(Yend&0xff);	
	
	
  LCD_WR_8REG(0x2C);	
}

/*******************************************************************************
//从FSMC_8080.C移放置在TFTLCD_Init.c    20171120
//设置光标位置    选择设置二种中一种就可以
//Xpos:横坐标
//Ypos:纵坐标
*******************************************************************************/
void LCD_SetCursor(uint16_t  Xpos, uint16_t  Ypos)
{	 
  //LCD_ILI9481SetCursor(Xpos,Ypos); //ILI9481  OK  20180328

  //LCDHVG35SetCursor(Xpos,Ypos); 
	
	if(lcddev.id==0x9481) {	LCD_ILI9481SetCursor(Xpos,Ypos); }
	else if(lcddev.id==0X9486) {	LCDHVG35SetCursor(Xpos,Ypos); }	
	else if(lcddev.id == 0X8000){
		LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(Xpos>>8); 		
		LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(Xpos&0XFF);			 
		LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(Ypos>>8);  		
		LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(Ypos&0XFF);	}
	
}

/***************************************************************/


///////////////光标////////////////

void LCDHVG35SetCursor(uint16_t  Xpos, uint16_t  Ypos)  //ILI9488 ,HX8357C,TEST 20171101_OK要加后面的013F,01DF
{
	
 //ILI9488 ,HX8357C,TEST 20171101_OK

  //LCD_WR_8REG(0x36);
  //LCD_WR_8DATA(0x08); ///08(正常用),C8（倒装用），A8(横用FPC@IC在右)，68(横用FPC@IC在左)
	
  //LCD_WR_8REG(lcddev.setxcmd);  //20180430
  LCD_WR_8REG(0X2A);	
  LCD_WR_8DATA(Xpos>>8);
  LCD_WR_8DATA(Xpos&0xff);
  LCD_WR_8DATA(0x01);
  LCD_WR_8DATA(0x3F);

  //LCD_WR_8REG(lcddev.setycmd);  //20180430
  LCD_WR_8REG(0X2B);	
  LCD_WR_8DATA(Ypos>>8);
  LCD_WR_8DATA(Ypos&0xff);
  LCD_WR_8DATA(0x01);
  LCD_WR_8DATA(0xDF);
	
  LCD_WR_8REG(0x2C);
}

void LCD_ILI9481SetCursor(uint16_t  Xpos, uint16_t  Ypos)  //ILI9481 20180511_OK
{
  //LCD_WR_8REG(lcddev.setxcmd);  //20180430
  LCD_WR_8REG(0X2A);	
  LCD_WR_8DATA(Xpos>>8);
  LCD_WR_8DATA(Xpos&0xff);
  LCD_WR_8DATA(0x01);
  LCD_WR_8DATA(0x3F);

  //LCD_WR_8REG(lcddev.setycmd);  //20180430
  LCD_WR_8REG(0X2B);	
  LCD_WR_8DATA(Ypos>>8);
  LCD_WR_8DATA(Ypos&0xff);
  LCD_WR_8DATA(0x01);
  LCD_WR_8DATA(0xDF);
	
  LCD_WR_8REG(0x2C);	
	
	/*
  ///20180511
	uint16_t  dir;
	dir=0;
	switch(dir)   
	{	
			case 0:{	
						//0-正装ok20180510
						///正装设置OK 20180510  36H:0A 
						lcddev.width=320;    //LCD 宽度
						lcddev.height=480;   //LCD 高度
						lcddev.setxcmd=0X2A;  //设置x坐标指令2A
						lcddev.setycmd=0X2B;  //设置y坐标指令2B
						lcddev.wramcmd=0X2C;  //开始写gram指令
							
						LCD_WR_8REG(0x36);   
						LCD_WR_8DATA(0x0A); //0A
							
						LCD_WR_8REG(lcddev.setxcmd);   
						LCD_WR_8DATA(Xpos>>8);
						LCD_WR_8DATA(Xpos&0xff);
						LCD_WR_8DATA(0x01);
						LCD_WR_8DATA(0x3F);	
							
						LCD_WR_8REG(lcddev.setycmd);   
						LCD_WR_8DATA(Ypos>>8);
						LCD_WR_8DATA(Ypos&0xff);
						LCD_WR_8DATA(0x01);
						LCD_WR_8DATA(0xDF);

						LCD_WR_8REG(lcddev.wramcmd);
					}break;

			case 1:{
					//1-20180511 FPC在左边: 36H:2F  OK
						lcddev.width=480;    //LCD 宽度
						lcddev.height=320;   //LCD 高度
						lcddev.setxcmd=0X2A;  //设置x坐标指令2A
						lcddev.setycmd=0X2B;  //设置y坐标指令2B
						lcddev.wramcmd=0X2C;  //开始写gram指令

						LCD_WR_8REG(0x36);
						LCD_WR_8DATA(0x2F);  //正常看排线与ＩＣ在左方：起点在左下角，>>90    2E
							// MY MX MV ML BGR MH X X
							//Bit B7 – Page Address Order, ‘0’ = Top to Bottom,‘1’ = Bottom to Top
							//Bit B6 – Column Address Order,‘0’ = Left to Right,1’ = Right to Left  
							//Bit B5 – Page/Column Order   Row / Column Exchange ,‘0’ = Normal Mode,‘1’ = Reverse Mode
							//Bit B4 –Line Address Order, Vertical Refresh Order,‘0’ = LCD Refresh Top to Bottom,1’ = LCD Refresh Bottom to Top
							// BGR(B3)(0=RGB color filter panel, 1=BGR color filter panel)  Bit B3 – RGB/BGR Order
							// SS(B2) his bit is not applicable for this project, so it is set to ‘0’. (Not supported)
							// -Bit B1 – Horizontal Flip
							// -Bit B0 – Vertical Flip
						LCD_WR_8REG(lcddev.setxcmd);   
						LCD_WR_8DATA(Xpos>>8);
						LCD_WR_8DATA(Xpos&0xff);
						LCD_WR_8DATA(0x01);
						LCD_WR_8DATA(0xDF);	
						
						LCD_WR_8REG(lcddev.setycmd);   
						LCD_WR_8DATA(Ypos>>8);
						LCD_WR_8DATA(Ypos&0xff);
						LCD_WR_8DATA(0x01);
						LCD_WR_8DATA(0x3F);
						
						LCD_WR_8REG(lcddev.wramcmd);

					}break;

			case 2:{	
						//2-倒装ok20180510
						///倒装设置OK 20180510  36H:1D 
						lcddev.width=320;    //LCD 宽度
						lcddev.height=480;   //LCD 高度
						lcddev.setxcmd=0X2A;  //设置x坐标指令2A
						lcddev.setycmd=0X2B;  //设置y坐标指令2B
						lcddev.wramcmd=0X2C;  //开始写gram指令
							
						LCD_WR_8REG(0x36);   
						LCD_WR_8DATA(0x1D); //1d,0d
							
						LCD_WR_8REG(lcddev.setxcmd);   
						LCD_WR_8DATA(Xpos>>8);
						LCD_WR_8DATA(Xpos&0xff);
						LCD_WR_8DATA(0x01);
						LCD_WR_8DATA(0x3F);	
							
						LCD_WR_8REG(lcddev.setycmd);   
						LCD_WR_8DATA(Ypos>>8);
						LCD_WR_8DATA(Ypos&0xff);
						LCD_WR_8DATA(0x01);
						LCD_WR_8DATA(0xDF);

						LCD_WR_8REG(lcddev.wramcmd);
					}break; 

			case 3:{
					//3-20180511 FPC在右边: 36H:2C  OK
						lcddev.width=480;    //LCD 宽度
						lcddev.height=320;   //LCD 高度
						lcddev.setxcmd=0X2A;  //设置x坐标指令2A
						lcddev.setycmd=0X2B;  //设置y坐标指令2B
						lcddev.wramcmd=0X2C;  //开始写gram指令

						LCD_WR_8REG(0x36);
						LCD_WR_8DATA(0x2C);  //正常看排线与ＩＣ在右方：起点在右上角，2C>>270
							// MY MX MV ML BGR MH X X
							//Bit B7 – Page Address Order, ‘0’ = Top to Bottom,‘1’ = Bottom to Top
							//Bit B6 – Column Address Order,‘0’ = Left to Right,1’ = Right to Left  
							//Bit B5 – Page/Column Order   Row / Column Exchange ,‘0’ = Normal Mode,‘1’ = Reverse Mode
							//Bit B4 –Line Address Order, Vertical Refresh Order,‘0’ = LCD Refresh Top to Bottom,1’ = LCD Refresh Bottom to Top
							// BGR(B3)(0=RGB color filter panel, 1=BGR color filter panel)  Bit B3 – RGB/BGR Order
							// SS(B2) his bit is not applicable for this project, so it is set to ‘0’. (Not supported)
							// -Bit B1 – Horizontal Flip
							// -Bit B0 – Vertical Flip
						LCD_WR_8REG(lcddev.setxcmd);   
						LCD_WR_8DATA(Xpos>>8);
						LCD_WR_8DATA(Xpos&0xff);
						LCD_WR_8DATA(0x01);
						LCD_WR_8DATA(0xDF);	
						
						LCD_WR_8REG(lcddev.setycmd);   
						LCD_WR_8DATA(Ypos>>8);
						LCD_WR_8DATA(Ypos&0xff);
						LCD_WR_8DATA(0x01);
						LCD_WR_8DATA(0x3F);
						
						LCD_WR_8REG(lcddev.wramcmd);	
					}break; 
			
			default:{	LCD_WR_8REG(0x36);   
						LCD_WR_8DATA(0x0A); 		}	break;							
	}	
  */	
}

void LcdNT35510ReadID(void); //20180510//OK20180510

/*****************************************************/

/**********************************************
函数名：LCD_ReadID
功能：LCD读取IC-ID  选择设置二种中一种就可以
返回值：无
***********************************************/
void LCD_ReadID(void)
{

  //	uint8_t  lcd_id[12];	//存放LCD ID字符串
  /***********************************************/	
	// LCD_RESET();//读取前需进行复位才能准确读到
  /***********************************************/
	 //LcdILI9481ReadID();  //ok

	LcdNT35510ReadID();
	
}



///////////////LCD-ID////////////////
void LcdILI9481ReadID(void) //20180510//OK20180510
{
  //unsigned char i;
  /***********************************************/
  //尝试9481 ID的并口读取	 OK20180510
//	printf("GWDBG[%s %d]\r\n", __FUNCTION__, __LINE__);
	LCD_WR_REG(0xB9); 
//	printf("GWDBG[%s %d]\r\n", __FUNCTION__, __LINE__);
	LCD_WR_DATA(0XFF);	
	LCD_WR_DATA(0X83);
	LCD_WR_DATA(0X69);	
	
	LCD_WR_REG(0XBF);				   
	lcddev.id=LCD_RD_DATA();	//1dummy read 
	printf("LCD ID1:%04X  ",lcddev.id);	
	lcddev.id=LCD_RD_DATA();	//2读到0X02
	printf("LCD ID2:%04X  ",lcddev.id);	
	lcddev.id=LCD_RD_DATA();   	//3读取04
	printf("LCD ID3:%04X  ",lcddev.id);	
	lcddev.id=LCD_RD_DATA();   	//4读取94
	printf("LCD ID4:%04X  ",lcddev.id);	
	lcddev.id<<=8;
	lcddev.id|=LCD_RD_DATA();   //5读取81
  //	lcddev.id=LCD_RD_DATA();   	//6读取FF
	
  sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组	
}

///////////////LCD-ID////////////////
void LcdNT35510ReadID(void) //20180510//OK20180510
{
	
  delay_1ms(50);
	LCD_WriteReg(0x0000,0x0001); 
	delay_1ms(50); // delay 50 ms 
	lcddev.id = LCD_ReadReg(0x0000); 
	
 	LCD_RESET();//读取前需进行复位才能准确读到
//	
//	LCD_WR_REG(0x00); 
//	LCD_WR_DATA(0x01);	
//	delay_1ms(50); // delay 50 ms 
//	
//	LCD_WR_REG(0x00);				   
//	lcddev.id=LCD_RD_DATA();	//1dummy read 
//  printf("LCD ID0:%04X\r\n",lcddev.id);	
	
	//LCD_RESET();//读取前需进行复位才能准确读到

//	//尝试9341 ID的读取		
//	LCD_WR_REG(0XD3);				   
//	lcddev.id=LCD_RD_DATA();	//dummy read 	
//	lcddev.id=LCD_RD_DATA();	//读到0X00
//	lcddev.id=LCD_RD_DATA();  //读取93								   
//	lcddev.id<<=8;
//	lcddev.id|=LCD_RD_DATA();//读取41 	   		
//	printf("9341 LCD ID:%04X\r\n",lcddev.id);	

//	//尝试6804 ID的读取		
//	LCD_WR_REG(0XBF);				   
//	lcddev.id=LCD_RD_DATA();	//dummy read 	
//	lcddev.id=LCD_RD_DATA();	//读到0X00
//	lcddev.id=LCD_RD_DATA();  //读取93								   
//	lcddev.id<<=8;
//	lcddev.id|=LCD_RD_DATA();//读取41 	   		
//	printf("6804 LCD ID:%04X\r\n",lcddev.id);	
////	
//	
//	/* NT35310 */
//	LCD_WR_REG(0XD4);				   
//	lcddev.id=LCD_RD_DATA();//dummy read  
//	lcddev.id=LCD_RD_DATA();//读回0X01	 
//	lcddev.id=LCD_RD_DATA();//读回0X53	
//	lcddev.id<<=8;	 
//	lcddev.id|=LCD_RD_DATA();	//这里读回0X10	 
//	printf("6804 LCD ID:%04X\r\n",lcddev.id);	
//	
//	
	/* NT35510 */
	
	LCD_WR_REG(0XDA00);	//read ID1
	lcddev.id=LCD_RD_DATA();		//读回0X00	 
	LCD_WR_REG(0XDB00);	//read ID2
	lcddev.id=LCD_RD_DATA();		//读回0X80
	lcddev.id<<=8;	
	LCD_WR_REG(0XDC00);	// read ID3
	lcddev.id|=LCD_RD_DATA();		//读回0X00		
	printf("NT35510 LCD ID:%04X\r\n",lcddev.id);	
	
  sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组	
}

void LcdILI9488ReadID(void)
{
  //unsigned char i;
  /***********************************************/
  //尝试9486,9488 ID的并口读取	 OK20171127
  LCD_WR_REG(0XD3);				   
  lcddev.id=LCD_RD_DATA();	//dummy read 	
  lcddev.id=LCD_RD_DATA();	//读到0X00
  lcddev.id=LCD_RD_DATA();   	//读取94	
  lcddev.id<<=8;
  lcddev.id|=LCD_RD_DATA();  	//读取86,88

  sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组	
}

/*******************************************************************************/
//设置LCD扫描方向
//按顺时针转：　0:0度，1：90度，2：180度，3：270度
//9163,9331,9340,9341,7735,7789,8357D,9327,9488.HX8357C :  36h
// MY Row Address Order
// MX Column Address Order
// MV Row / Column Exchange
// ML Vertical Refresh Order
// (0=RGB color filter panel, 1=BGR color filter panel)
// MH Horizontal Refresh ORDER
// Horizontal Flip x
// Vertical Flip  y
/*******************************************************************************/
void TFT_Scan_Dir(uint8_t  dir)	 //20171123  OK
{
  /*				
	uint16_t  Xpos, uint16_t  Ypos;
	
	switch(dir)   
	{	
			case 0:{	
						//0-正装ok20180510
						///正装设置OK 20180510  36H:0A 
						lcddev.width=320;    //LCD 宽度
						lcddev.height=480;   //LCD 高度
						lcddev.setxcmd=0X2A;  //设置x坐标指令2A
						lcddev.setycmd=0X2B;  //设置y坐标指令2B
						lcddev.wramcmd=0X2C;  //开始写gram指令
							
						LCD_WR_8REG(0x36);   
						LCD_WR_8DATA(0x0A); //0A
							
						LCD_WR_8REG(lcddev.setxcmd);   
						LCD_WR_8DATA(Xpos>>8);
						LCD_WR_8DATA(Xpos&0xff);
						LCD_WR_8DATA(0x01);
						LCD_WR_8DATA(0x3F);	
							
						LCD_WR_8REG(lcddev.setycmd);   
						LCD_WR_8DATA(Ypos>>8);
						LCD_WR_8DATA(Ypos&0xff);
						LCD_WR_8DATA(0x01);
						LCD_WR_8DATA(0xDF);

						LCD_WR_8REG(lcddev.wramcmd);
					}break;

			case 1:{
					//1-20180511 FPC在左边: 36H:2F  OK
						lcddev.width=480;    //LCD 宽度
						lcddev.height=320;   //LCD 高度
						lcddev.setxcmd=0X2A;  //设置x坐标指令2A
						lcddev.setycmd=0X2B;  //设置y坐标指令2B
						lcddev.wramcmd=0X2C;  //开始写gram指令

						LCD_WR_8REG(0x36);
						LCD_WR_8DATA(0x2F);  //正常看排线与ＩＣ在左方：起点在左下角，>>90    2E
							// MY MX MV ML BGR MH X X
							//Bit B7 – Page Address Order, ‘0’ = Top to Bottom,‘1’ = Bottom to Top
							//Bit B6 – Column Address Order,‘0’ = Left to Right,1’ = Right to Left  
							//Bit B5 – Page/Column Order   Row / Column Exchange ,‘0’ = Normal Mode,‘1’ = Reverse Mode
							//Bit B4 –Line Address Order, Vertical Refresh Order,‘0’ = LCD Refresh Top to Bottom,1’ = LCD Refresh Bottom to Top
							// BGR(B3)(0=RGB color filter panel, 1=BGR color filter panel)  Bit B3 – RGB/BGR Order
							// SS(B2) his bit is not applicable for this project, so it is set to ‘0’. (Not supported)
							// -Bit B1 – Horizontal Flip
							// -Bit B0 – Vertical Flip
						LCD_WR_8REG(lcddev.setxcmd);   
						LCD_WR_8DATA(Xpos>>8);
						LCD_WR_8DATA(Xpos&0xff);
						LCD_WR_8DATA(0x01);
						LCD_WR_8DATA(0xDF);	
						
						LCD_WR_8REG(lcddev.setycmd);   
						LCD_WR_8DATA(Ypos>>8);
						LCD_WR_8DATA(Ypos&0xff);
						LCD_WR_8DATA(0x01);
						LCD_WR_8DATA(0x3F);
						
						LCD_WR_8REG(lcddev.wramcmd);

					}break;

			case 2:{	
						//2-倒装ok20180510
						///倒装设置OK 20180510  36H:1D 
						lcddev.width=320;    //LCD 宽度
						lcddev.height=480;   //LCD 高度
						lcddev.setxcmd=0X2A;  //设置x坐标指令2A
						lcddev.setycmd=0X2B;  //设置y坐标指令2B
						lcddev.wramcmd=0X2C;  //开始写gram指令
							
						LCD_WR_8REG(0x36);   
						LCD_WR_8DATA(0x1D); //1d,0d
							
						LCD_WR_8REG(lcddev.setxcmd);   
						LCD_WR_8DATA(Xpos>>8);
						LCD_WR_8DATA(Xpos&0xff);
						LCD_WR_8DATA(0x01);
						LCD_WR_8DATA(0x3F);	
							
						LCD_WR_8REG(lcddev.setycmd);   
						LCD_WR_8DATA(Ypos>>8);
						LCD_WR_8DATA(Ypos&0xff);
						LCD_WR_8DATA(0x01);
						LCD_WR_8DATA(0xDF);

						LCD_WR_8REG(lcddev.wramcmd);
					}break; 

			case 3:{
					//3-20180511 FPC在右边: 36H:2C  OK
						lcddev.width=480;    //LCD 宽度
						lcddev.height=320;   //LCD 高度
						lcddev.setxcmd=0X2A;  //设置x坐标指令2A
						lcddev.setycmd=0X2B;  //设置y坐标指令2B
						lcddev.wramcmd=0X2C;  //开始写gram指令

						LCD_WR_8REG(0x36);
						LCD_WR_8DATA(0x2C);  //正常看排线与ＩＣ在右方：起点在右上角，2C>>270
							// MY MX MV ML BGR MH X X
							//Bit B7 – Page Address Order, ‘0’ = Top to Bottom,‘1’ = Bottom to Top
							//Bit B6 – Column Address Order,‘0’ = Left to Right,1’ = Right to Left  
							//Bit B5 – Page/Column Order   Row / Column Exchange ,‘0’ = Normal Mode,‘1’ = Reverse Mode
							//Bit B4 –Line Address Order, Vertical Refresh Order,‘0’ = LCD Refresh Top to Bottom,1’ = LCD Refresh Bottom to Top
							// BGR(B3)(0=RGB color filter panel, 1=BGR color filter panel)  Bit B3 – RGB/BGR Order
							// SS(B2) his bit is not applicable for this project, so it is set to ‘0’. (Not supported)
							// -Bit B1 – Horizontal Flip
							// -Bit B0 – Vertical Flip
						LCD_WR_8REG(lcddev.setxcmd);   
						LCD_WR_8DATA(Xpos>>8);
						LCD_WR_8DATA(Xpos&0xff);
						LCD_WR_8DATA(0x01);
						LCD_WR_8DATA(0xDF);	
						
						LCD_WR_8REG(lcddev.setycmd);   
						LCD_WR_8DATA(Ypos>>8);
						LCD_WR_8DATA(Ypos&0xff);
						LCD_WR_8DATA(0x01);
						LCD_WR_8DATA(0x3F);
						
						LCD_WR_8REG(lcddev.wramcmd);	
					}break; 
			
			default:{	LCD_WR_8REG(0x36);   
						LCD_WR_8DATA(0x0A); 		}	break;						
	
	}	
	*/
}
///===========END ==============//
