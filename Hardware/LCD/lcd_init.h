#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "gd32f4xx.h"
#include "systick.h"

#define USE_HORIZONTAL 0  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏
#define USE_HARDWARESPI   // 设置使用硬件SPI还是软件SPI  


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 280

#else
#define LCD_W 280
#define LCD_H 240
#endif

//-----------------LCD端口定义---------------- 

#ifndef USE_HARDWARESPI

#define LCD_SCLK_Clr() gpio_bit_reset(GPIOF,GPIO_PIN_7)//SCL=SCLK PF7
#define LCD_SCLK_Set() gpio_bit_set(GPIOF,GPIO_PIN_7)

#define LCD_MOSI_Clr() gpio_bit_reset(GPIOF,GPIO_PIN_9)//SDA=MOSI  PF9
#define LCD_MOSI_Set() gpio_bit_set(GPIOF,GPIO_PIN_9)

#endif

//#define LCD_RES_Clr()  gpio_bit_reset(GPIOB,GPIO_PIN_1)//RES  PB1
//#define LCD_RES_Set()  gpio_bit_set(GPIOB,GPIO_PIN_1)

//#define LCD_DC_Clr()   gpio_bit_reset(GPIOB,GPIO_PIN_15)//DC  PB15
//#define LCD_DC_Set()   gpio_bit_set(GPIOB,GPIO_PIN_15)
// 		     
//#define LCD_CS_Clr()   gpio_bit_reset(GPIOB,GPIO_PIN_13)//CS  PB13
//#define LCD_CS_Set()   gpio_bit_set(GPIOB,GPIO_PIN_13)

//#define LCD_BLK_Clr()  gpio_bit_reset(GPIOG,GPIO_PIN_13)//BLK  PG13
//#define LCD_BLK_Set()  gpio_bit_set(GPIOG,GPIO_PIN_13)

#define LCD_RST_RCU    RCU_GPIOF
#define LCD_RST_PORT   GPIOF
#define LCD_RST_PIN    GPIO_PIN_10

#define LCD_DC_RCU     RCU_GPIOA
#define LCD_DC_PORT    GPIOA
#define LCD_DC_PIN     GPIO_PIN_6

#define LCD_CS_RCU     RCU_GPIOA
#define LCD_CS_PORT    GPIOA
#define LCD_CS_PIN     GPIO_PIN_4

#define LCD_BLK_RCU    RCU_GPIOA
#define LCD_BLK_PORT   GPIOA
#define LCD_BLK_PIN    GPIO_PIN_7

#define LCD_RES_Clr()  gpio_bit_reset(LCD_RST_PORT,LCD_RST_PIN)//RST  PF10
#define LCD_RES_Set()  gpio_bit_set(LCD_RST_PORT,LCD_RST_PIN)

#define LCD_DC_Clr()   gpio_bit_reset(LCD_DC_PORT,LCD_DC_PIN)//DC  PA6
#define LCD_DC_Set()   gpio_bit_set(LCD_DC_PORT,LCD_DC_PIN)
 		     
#define LCD_CS_Clr()   gpio_bit_reset(LCD_CS_PORT,LCD_CS_PIN)//CS  PA4
#define LCD_CS_Set()   gpio_bit_set(LCD_CS_PORT,LCD_CS_PIN)

#define LCD_BLK_Clr()  gpio_bit_reset(LCD_BLK_PORT,LCD_BLK_PIN)//BLK  PA7
#define LCD_BLK_Set()  gpio_bit_set(LCD_BLK_PORT,LCD_BLK_PIN)


void LCD_GPIO_Init(void);//初始化GPIO
void LCD_Writ_Bus(uint8_t dat);//模拟SPI时序
void LCD_WR_DATA8(uint8_t dat);//写入一个字节
void LCD_WR_DATA(uint16_t dat);//写入两个字节
void LCD_WR_REG(uint8_t dat);//写入一个指令
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);//设置坐标函数
void LCD_Init(void);//LCD初始化


void LCD_timer_config(void);


#endif




