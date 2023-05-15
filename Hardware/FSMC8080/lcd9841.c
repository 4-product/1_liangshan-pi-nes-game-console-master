#include "lcd9841.h"
#include "stdlib.h" 

#include "font.h"
#include "TFTLCD_Init.h"
#include "gImage_128128.h"
#include "flowerPic.h" //花朵图片240*320图片

#define Pic  0    //对应打开上面点阵图片，1为打上开上图片320*320图片，要么240*320图片,挂载ＳＤ不能打开320320Pic：内存不足

//LCD的画笔颜色和背景色	   
uint16_t  POINT_COLOR=0x0000;	//画笔颜色
uint16_t  BACK_COLOR=0xFFFF;  //背景色 
  
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

//初始化lcd
//该初始化函数可以初始化各种ILI93XX液晶,但是其他函数是基于ILI9320的!!!
//在其他型号的驱动芯片上没有测试! 
void LCD_CtrlLinesConfig(void)
{ 	
		/* 背光 PD13 */
    rcu_periph_clock_enable(RCU_GPIOD);
    gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP,GPIO_PIN_13);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_13);

	 //rcu_periph_clock_enable(RCU_GPIOD);
	 rcu_periph_clock_enable(RCU_GPIOE);
	 rcu_periph_clock_enable(RCU_GPIOG);
	
	/* 引脚复用配置 */
	gpio_af_set(GPIOD, GPIO_AF_12, GPIO_PIN_0|GPIO_PIN_1
	|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15); // EXMC功能

	gpio_af_set(GPIOE, GPIO_AF_12, GPIO_PIN_7|GPIO_PIN_8
		|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15); // EXMC功能
	
	gpio_af_set(GPIOG, GPIO_AF_12, GPIO_PIN_0|GPIO_PIN_12); // EXMC功能
	
	//FSMC-D0--D15: 
  // D0   D1   D2   D3   D4   D5   D6   D7   D8   D9   D10   D11   D12   D13  D14   D15       
  //PD14 PD15  PD0  PD1  PE7  PE8  PE9 PE10 PE11 PE12  PE13  PE14  PE15  PD8  PD9   PD10 
  //FSMC_CMD:
  //FSMC_NOE      FSMC_NWE       FSMC_NE4     FSMC_A10
  //LCD_RD        LCD_WR         LCD_CS       LCD_RS  
  //PD4            PD5           PG12          PG0	

   gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_0|GPIO_PIN_1
		|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15);
   gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_0|GPIO_PIN_1
		|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15);
	
	

   gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_7|GPIO_PIN_8
		|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
   gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_7|GPIO_PIN_8
		|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
	 

		/*EXMC_NE3  PG12   EXMC_A10  PG0 */
	
   gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_0|GPIO_PIN_12);
   gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_0|GPIO_PIN_12);
	 

		LCD_BLK_ON;  // 点亮背光 
	//LCD_BLK_OFF; // 关闭背光
}

void LCD_FSMCConfig(void)
{
	exmc_norsram_parameter_struct nor_init_struct; // EXMC参数结构体
  exmc_norsram_timing_parameter_struct readWriteTiming; // EXMC时间参数结构体
	exmc_norsram_timing_parameter_struct writeTiming; // EXMC时间参数结构体
	
	 /* EXMC clock enable */
	rcu_periph_clock_enable(RCU_EXMC);
	
 /* configure timing parameter */
	/* 读写时间 */
	readWriteTiming.asyn_access_mode = EXMC_ACCESS_MODE_A; //模式A    
	readWriteTiming.syn_data_latency = EXMC_DATALAT_2_CLK;
	readWriteTiming.syn_clk_division = EXMC_SYN_CLOCK_RATIO_2_CLK;
	readWriteTiming.bus_latency = 0;
	readWriteTiming.asyn_data_setuptime = 72;			//需要360ns数据保存时间为72个HCLK	=5*72=360ns
	readWriteTiming.asyn_address_holdtime = 0x00; //地址保持时间（ADDHLD）模式A未用到	
	readWriteTiming.asyn_address_setuptime = 18; //需要96ns  地址建立时间（ADDSET）为19个HCLK 1/200M=5ns*19=96ns	

//	readWriteTiming.asyn_data_setuptime = 150;			//需要360ns数据保存时间为72个HCLK	=5*72=360ns
//	readWriteTiming.asyn_address_holdtime = 0x00; //地址保持时间（ADDHLD）模式A未用到	
//	readWriteTiming.asyn_address_setuptime = 50; //需要96ns  地址建立时间（ADDSET）为19个HCLK 1/200M=5ns*19=96ns	

	
	/* 读写时间 */
	writeTiming.asyn_access_mode = EXMC_ACCESS_MODE_A; //模式A    
	writeTiming.syn_data_latency = EXMC_DATALAT_2_CLK;
	writeTiming.syn_clk_division = EXMC_SYN_CLOCK_RATIO_2_CLK;
	writeTiming.bus_latency = 0;
//	writeTiming.asyn_data_setuptime = 11;			//需要54ns数据保存时间为11个HCLK	=5*11=54ns
//	writeTiming.asyn_address_holdtime = 0x00; //地址保持时间（ADDHLD）模式A未用到	
//	writeTiming.asyn_address_setuptime = 10; //需要54ns  地址建立时间（ADDSET）为11个HCLK 1/200M=5ns*11=54ns	


	writeTiming.asyn_data_setuptime = 4;			//需要18ns数据保存时间为11个HCLK	=5*4=18ns
	writeTiming.asyn_address_holdtime = 0x00; //地址保持时间（ADDHLD）模式A未用到	
	writeTiming.asyn_address_setuptime = 3; //需要18ns  地址建立时间（ADDSET）为11个HCLK 1/200M=5ns*4=18ns	


	/* configure EXMC bus parameters */
	nor_init_struct.norsram_region = EXMC_BANK0_NORSRAM_REGION3;
	// FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;// FSMC_Bank1_NORSRAM4;//  这里使用NE1 ，也就对应BTCR[6],[7]。
	nor_init_struct.address_data_mux = DISABLE; // 不复用数据地址
	nor_init_struct.memory_type = EXMC_MEMORY_TYPE_SRAM;  // FSMC_MemoryType_SRAM;  //SRAM 
	
	switch(MCU)   
	{	
			case 0:{nor_init_struct.databus_width = EXMC_NOR_DATABUS_WIDTH_8B; }  break; //存储器数据宽度为8bit  
			case 1:{nor_init_struct.databus_width = EXMC_NOR_DATABUS_WIDTH_16B;}  break; //存储器数据宽度为16bit 
			default:nor_init_struct.databus_width = EXMC_NOR_DATABUS_WIDTH_16B;; break;
	} 
		
	  nor_init_struct.burst_mode = DISABLE;
		nor_init_struct.nwait_polarity = EXMC_NWAIT_POLARITY_LOW;
		nor_init_struct.asyn_wait = DISABLE; 
		nor_init_struct.wrap_burst_mode = DISABLE;	
		nor_init_struct.nwait_config = EXMC_NWAIT_CONFIG_BEFORE;
		nor_init_struct.memory_write = ENABLE;//  存储器写使能
	  nor_init_struct.nwait_signal = DISABLE;	
	  nor_init_struct.extended_mode = ENABLE;  // 读写使用不同的时序
		nor_init_struct.write_mode = EXMC_ASYN_WRITE;
	
	nor_init_struct.read_write_timing = &readWriteTiming;//读写时序
	nor_init_struct.write_timing = &writeTiming;//写时序
	
	
   exmc_norsram_init(&nor_init_struct); //初始化FSMC配置
   /* enable the EXMC bank0 NORSRAM */
   exmc_norsram_enable(EXMC_BANK0_NORSRAM_REGION3);   // 使能BANK1 	
	
}

/*******************************************************************************
* Function Name  : void LCD_Config(void)   
* Description    : 对ＬＣＤ并口，串口配置选择，PS：1－并口，0－串口 , ＬＣＤ配置选择
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Config(void)
{
		//并口8/16BIT要打开下面
		LCD_CtrlLinesConfig();     //配置STM32口对应ＴＦＴ接口
		LCD_FSMCConfig();          //初始化FSMC对应ＴＦＴ	
//	printf("GWDBG[%s %d]\r\n", __FUNCTION__, __LINE__);
		
}

/*******************************************************************************
* Function Name  : LCD_Reset
* Description    : 对ＬＣＤ进行复位操作
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_RESET(void)
{   	
	/* 复位RST PD12 */
	rcu_periph_clock_enable(RCU_GPIOD);
	gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,GPIO_PIN_12);
	gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_12);
	
	gpio_bit_set(GPIOD, GPIO_PIN_12);	
	delay_1ms(10);
	gpio_bit_reset(GPIOD, GPIO_PIN_12);	  
	delay_1ms(50);					   
	gpio_bit_set(GPIOD, GPIO_PIN_12);			 
	delay_1ms(120);		 
}

/*******************************************************************************
* Function Name  : LCD_WR_REG　　
* Description    : 写寄存器函数,regval:寄存器值
* Input          : regval
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WR_REG(uint16_t  regval)			//初始化时对应：16BIT REG
{  
//	printf("GWDBG[%s %d]\r\n", __FUNCTION__, __LINE__);
	LCD->LCD_REG=regval;  
	//printf("GWDBG[%s %d]\r\n", __FUNCTION__, __LINE__);

}

void LCD_WR_8REG(uint16_t  regval)  //初始化时对应：8BIT  REG
{   	
		// printf("LCD->LCD_REG: %p\r\n",&LCD->LCD_REG);
		LCD->LCD_REG=regval; 		
}

/*******************************************************************************
* Function Name  : LCD_WR_DATA   
* Description    : 写LCD数据函数,data:要写入的值
* Input          : data
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WR_DATA(uint16_t  data)  //初始化时对应：16BIT DATA
{
  LCD->LCD_RAM=data;
}

void LCD_WR_8DATA(uint16_t  data)  //初始化时对应：8BIT DATA
{
  LCD->LCD_RAM=data;
}

/*******************************************************************************
* Function Name  : LCD_WriteReg    
* Description    : 写寄存器函数,LCD_Reg:寄存器地址；写LCD数据函数,LCD_RegValue:要写入的数据
* Input          : LCD_Reg,LCD_RegValue
* Output         : None
* Return         : None
*这一般用在１６位寄存器格式上的多，故用传两次．要是８位寄存器ＩＣ在改一下只传一次的．
*******************************************************************************/
void LCD_WriteReg(uint16_t  LCD_Reg,uint16_t  LCD_RegValue)  
{	
		LCD->LCD_REG = LCD_Reg;		   	 
		LCD->LCD_RAM = LCD_RegValue; 		
}

/*******************************************************************************
* Function Name  : LCD_RD_DATA     ＹＺ
* Description    : 读LCD数据函数
* Input          : None
* Output         : None
* Return         : 读到的值
*******************************************************************************/
uint16_t  LCD_RD_DATA(void)  //库函数读
{
	uint16_t  ram;			//防止被优化
	ram=LCD->LCD_RAM;	
	return ram;	 
}


void delay_us(uint16_t us)
{
	while(us--){
		__NOP;
		__NOP;
		__NOP;
		__NOP;
		__NOP;
		__NOP;
	}
}

/*******************************************************************************
* Function Name  : LCD_ReadReg     
* Description    : 读IC寄存器函数,读寄存器
* Input          : LCD_Reg:寄存器地址
* Output         : None
* Return         : 读到的值
*******************************************************************************/
uint16_t  LCD_ReadReg(uint16_t  LCD_Reg)
{		
  uint16_t  i = 0;	
	uint16_t  info;


		LCD_WR_REG(LCD_Reg);		//写入要读的寄存器序号
		delay_us(5);
		info=LCD_RD_DATA();
		info<<=8;
		info|=LCD_RD_DATA();
		i = 2;
	
  if(i == 1)
    return LCD_RD_DATA();	
	else if(i == 2)
		return info;
	else
	  return 0;
}   

/*******************************************************************************
* Function Name  : LCD_WriteRAM_Prepare  开始写GRAM   改ＹＺ
* Description    : 开始写IC-GRAM寄存器函数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteRAM_Prepare(void)
{
 	LCD->LCD_REG=lcddev.wramcmd;	
	//TFT_WriteRAM(); //在TFTLCD_Init.c设置
}

/*******************************************************************************
* Function Name  : LCD_WriteRAM    
* Description    : LCD写GRAM寄存器函数
* Input          : RGB_Code:颜色值
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteRAM(uint16_t  RGB_Code)
{
  LCD->LCD_RAM = RGB_Code;//写十六位GRAM
}

/*******************************************************************************
* Function Name  : LCD_BGR2RGB     ＹＺ
* Description    : GBR格式函数.从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式,通过该函数转换.
* Input          : c:GBR格式的颜色值
* Output         : None
* Return         : 返回值RGB格式的颜色值
*******************************************************************************/
uint16_t  LCD_BGR2RGB(uint16_t  c)
{
	uint16_t   r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
} 

/*******************************************************************************
//当mdk -O1时间优化时需要设置    ＹＺ
//延时i
*******************************************************************************/
void opt_delay(uint8_t  i)
{
	while(i--);
}

/*******************************************************************************
* Function Name  : LCD_ReadPoint    ＹＺ
* Description    :读取个某点的颜色值
* Input          : x,y:坐标
* Output         : None
* Return         : 返回值此点的颜色的颜色值
*******************************************************************************/
uint16_t  LCD_ReadPoint(uint16_t  x,uint16_t  y)
{
 	uint16_t  r=0,g=0,b=0;
	if(x>=lcddev.width||y>=lcddev.height)return 0;	//超过了范围,直接返回		   
	LCD_SetCursor(x,y);	    
	if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X1963)LCD_WR_REG(0X2E);//9341/6804/3510/1963 发送读GRAM指令
	else if(lcddev.id==0X5510)LCD_WR_REG(0X2E00);	//5510 发送读GRAM指令
	else LCD_WR_REG(0X22);      		 			//其他IC发送读GRAM指令
	if(lcddev.id==0X9320)opt_delay(2);				//FOR 9320,延时2us	    
 	r=LCD_RD_DATA();								//dummy Read	   
	if(lcddev.id==0X1963)return r;					//1963直接读就可以 
	opt_delay(2);	  
 	r=LCD_RD_DATA();  		  						//实际坐标颜色
 	if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X5510)		//9341/NT35310/NT35510要分2次读出
 	{
		opt_delay(2);	  
		b=LCD_RD_DATA(); 
		g=r&0XFF;		//对于9341/5310/5510,第一次读取的是RG的值,R在前,G在后,各占8位
		g<<=8;
	} 
	if(lcddev.id==0X9325||lcddev.id==0X4535||lcddev.id==0X4531||lcddev.id==0XB505||lcddev.id==0XC505)return r;	//这几种IC直接返回颜色值
	else if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X5510)return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));//ILI9341/NT35310/NT35510需要公式转换一下
	else return LCD_BGR2RGB(r);						//其他IC
}	
	

/*******************************************************************************
* Function Name  : LCD_Clear   modify YZ
* Description    ://清屏函数   
* Input          : color:要清屏的填充色
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Clear(uint16_t  color)
{
	//uint16_t  i,j;
	uint32_t index=0;     
	uint32_t totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			//得到总点数
	
	//LCD_SetCursor(0x00,0x0000);	//设置光标位置 
	BlockWrite(0,lcddev.width,0,lcddev.height);
	LCD_WriteRAM_Prepare();     		  //开始写入GRAM	 	
	
		for(index=0;index<totalpoint;index++)
		{
			LCD_WriteRAM(color);
		 // LCD->LCD_RAM=color;
		}		
} 

/*******************************************************************************
* Function Name  : LCD_Display
* Description    : 显示填充色
* Input          : color:要显示的填充色
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Display(uint16_t  color)
{
	uint16_t  i,j;
	
	//uint16_t  sx=0,sy=0;
	//uint16_t  twidth,theight;
	//twidth=sx+lcddev.width-1;
	//theight=sy+lcddev.height-1;

	BlockWrite(0,lcddev.width,0,lcddev.height);
  //BlockWrite(0,lcddev.width-1,0,lcddev.height-1);  //for SPFD5408  ILI9481 OK  20180329    已在BlockWrite更新写法20180727
	
	for(i=0;i<lcddev.height;i++)
	{
		for(j=0;j<lcddev.width;j++)
		{	
			LCD_WriteRAM(color);
      //LCD->LCD_RAM=color;			
		}
	}
}

/*******************************************************************************
* Function Name  : LCD_DisplayJiuGongGe
* Description    : 九宫格显示:RGB-BGR-RGB
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_DisplayJiuGongGe(void)
{
	uint16_t  Row,Column,aa;
	
	BlockWrite(0,lcddev.width,0,lcddev.height);
	
  //BlockWrite(0,lcddev.width-1,0,lcddev.height-1);  //for SPFD5408  ILI9481 OK  20180329    已在BlockWrite更新写法20180727
	
	aa=lcddev.height/3;

	for(Row=0;Row<aa;Row++)
	{
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0xf800);
      //LCD->LCD_RAM=0xf800;
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0x07e0);
      //LCD->LCD_RAM=0x07e0;
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0x001f);
      //LCD->LCD_RAM=0x001f;
		for(Column=0;Column<(lcddev.width%3);Column++) 	LCD_WriteRAM(0x001f);
      //LCD->LCD_RAM=0x001f;
	}

	for(Row=0;Row<aa;Row++)
	{			  
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0x001f);
    //LCD->LCD_RAM=0x001f;
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0xf800);		
    //LCD->LCD_RAM=0xf800;
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0x07e0);
    //LCD->LCD_RAM=0x07e0;
		for(Column=0;Column<(lcddev.width%3);Column++)	LCD_WriteRAM(0x07e0); 
    //LCD->LCD_RAM=0x07e0;
	}

	for(Row=0;Row<aa;Row++)
	{			  				   
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0x07e0); 
    //LCD->LCD_RAM=0x07e0;
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0x001f);  
    //LCD->LCD_RAM=0x001f;
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0xf800); 
    //LCD->LCD_RAM=0xf800;
		for(Column=0;Column<(lcddev.width%3);Column++)	LCD_WriteRAM(0xf800);  
    //LCD->LCD_RAM=0xf800;
	}

	for(Row=0;Row<(lcddev.height%3);Row++)
	{			  				   
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0x07e0); 
    //LCD->LCD_RAM=0x07e0;
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0x001f);  
    //LCD->LCD_RAM=0x001f;
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0xf800); 
    //LCD->LCD_RAM=0xf800;
		for(Column=0;Column<(lcddev.width%3);Column++)	LCD_WriteRAM(0xf800);  
    //LCD->LCD_RAM=0xf800;
	}
}

/*******************************************************************************
* Function Name  : LCD_Frame
* Description    : 四边线＋中间区域内填充单个颜色  
* Input          :RGB_Data为中间纯色， f_data为四边框线色
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Frame(uint16_t  RGB_Data,uint16_t  f_data)
{
	uint16_t  i,j;

	//BlockWrite(50,100,50,100);
	uint16_t  x = 100;
	uint16_t  y = 100;
	BlockWrite(0,x,0,y);
  //BlockWrite(0,lcddev.width-1,0,lcddev.height-1);  //for SPFD5408 ,ILI9481 OK 20180326     已在BlockWrite更新写法20180727
	
//	for(i=0;i<1;i++)  // 上面一行 
//	{
//		for(j=0;j<x;j++)	LCD_WriteRAM(f_data);	
//  //LCD->LCD_RAM=f_data;
//	}
	
	for (i=0;i<y;i++)
	{
	//	for (j=0;j<1;j++)	LCD_WriteRAM(f_data);  // 左边一列
    //LCD->LCD_RAM=f_data;
		for (j=0;j<x;j++)	
		LCD_WriteRAM(RGB_Data);	
    //LCD->LCD_RAM=RGB_Data;
//			
//		for (j=0;j<1;j++)	LCD_WriteRAM(f_data);  // 右边一列
//    //LCD->LCD_RAM=f_data;
	}

//	for(i=0;i<1;i++)  // 下面一行 
//	{
//		for(j=0;j<x;j++)	LCD_WriteRAM(f_data);
//    //LCD->LCD_RAM=f_data;
//	}	
}	

/*******************************************************************************
* Function Name  : Show_Gray
* Description    : 灰阶显示
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Show_Gray(void) 
{
	unsigned int i,j,k,c16h;
	
	BlockWrite(0,lcddev.width,0,lcddev.height);
  //BlockWrite(0,lcddev.width-1,0,lcddev.height-1);	//FOR 9481 TEST 20180328    已在BlockWrite更新写法20180727
	for(j=0;j<(lcddev.height);j++)  
		{
		c16h=0x0000;
		for(k=0;k<16;k++)
		{
		    for(i=0;i<(lcddev.width/16);i++)
				{
					LCD_WriteRAM(c16h);	
          //LCD->LCD_RAM=c16h;
				}
			c16h=c16h+0x1082;//32阶 0841 16阶 1082 8阶2104
		}
	}
}	

//--------Display GRAY------------//	
//屏上半部分为：右半边黑，左半边白,下半屏为灰价色
void GRAY_TEST16(void)
{
	unsigned int i,j,k,c16h;
	POINT_COLOR=RED;
	
	BlockWrite(0,lcddev.width,0,lcddev.height);	

  //BlockWrite(0,lcddev.width-1,0,lcddev.height-1);	//FOT 9481  20180328    已在BlockWrite更新写法20180727
	
	for(j=0;j<(lcddev.height/2);j++)
	{
		for(i=0;i<(lcddev.width/2);i++)
		{
			LCD_WriteRAM(0xffff);//	
		}
		for(i=0;i<(lcddev.width/2);i++)
		{
			LCD_WriteRAM(0x0000);//
		}
	}
				
	for(j=0;j<(lcddev.height/2);j++)
	{
		c16h=0x0000;
		for(k=0;k<16;k++)
		{
		  for(i=0;i<(lcddev.width/16);i++)
			{
				LCD_WriteRAM(c16h);
			}
			c16h=c16h+0x0841;//32阶 0841 16阶 1082 8阶2104
		}
	}
		
	LCD_ShowString((lcddev.width/6),(lcddev.height/4),210,16,16,"White");
	LCD_ShowString((lcddev.width*2/3),(lcddev.height/4),210,16,16,"Black");		
	LCD_ShowString((lcddev.width/6),(lcddev.height*2/3),210,16,16,"Gray");	
}
//--------END Display GRAY------------//

/*******************************************************************************
* Function Name  : Check_Crosstalk
* Description    : 检查是否有crosstalk现象   
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Check_Crosstalk(void)
{
	unsigned short m,n,i,j;
   
	BlockWrite(0,lcddev.width,0,lcddev.height);     
 
  for(m=0;m<lcddev.height/4;m++)
  {
       for(n=0;n<lcddev.width;n++)	LCD_WriteRAM(0x8410);
  }
	for(m=0;m<lcddev.height/2;m++)
  {
       for(n=0;n<lcddev.width/4;n++)	LCD_WriteRAM(0x8410);
       for(i=0;i<lcddev.width/2;i++)	LCD_WriteRAM(0x0000);
       for(j=0;j<lcddev.width/4;j++)	LCD_WriteRAM(0x8410);
	}
  for(m=0;m<lcddev.height/4;m++)
  {
       for(n=0;n<lcddev.width;n++)	LCD_WriteRAM(0x8410);
	}   
}

/*******************************************************************************
* Function Name  : Check_Flicker
* Description    : 测试LCD闪烁	flick
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Check_Flicker(void)
{
	unsigned short i,j;
	
	BlockWrite(0,lcddev.width,0,lcddev.height);     
 
	for(i=0;i<lcddev.height;i++) 
  { 
		for(j=0;j<lcddev.width;j++) LCD_WriteRAM(0x8410);
    i++;
    for(j=0;j<lcddev.width;j++) LCD_WriteRAM(0x0000);
  }      
}

/*******************************************************************************
* Function Name  : LCD_Fill   Modify YZ
* Description    : 在指定区域内填充单个颜色，(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)  
* Input          : color:要填充的颜色
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Fill(uint16_t  sx,uint16_t  sy,uint16_t  ex,uint16_t  ey,uint16_t  color)
{          
	uint16_t  i,j;
	uint16_t  xlen=0;
	//uint16_t  temp;

	xlen=ex-sx+1;
	
	for(i=sy;i<=ey;i++)
	{
		LCD_SetCursor(sx,i);      				//设置光标位置 
		LCD_WriteRAM_Prepare();     			//开始写入GRAM	  
		for(j=0;j<xlen;j++)	
		LCD_WriteRAM(color);
		
    //LCD->LCD_RAM=color;	//显示颜色 	    
	}	
} 

/*******************************************************************************
//在指定区域内填充指定颜色块		YZ	 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
*******************************************************************************/
void LCD_Color_Fill(uint16_t  sx,uint16_t  sy,uint16_t  ex,uint16_t  ey,uint16_t  *color)
{  
	uint16_t  height,width;
	uint16_t  i,j;
	
	
	width=ex-sx+1; 			//得到填充的宽度
	height=ey-sy+1;			//高度
	
// 	for(i=0;i<height;i++)
//	{
// 		LCD_SetCursor(sx,sy+i);   	//设置光标位置 
//		LCD_WriteRAM_Prepare();     //开始写入GRAM
//		for(j=0;j<width;j++)
//	//	LCD_WriteRAM(color[i*width+j]);
//    LCD->LCD_RAM = color[i*width+j];//写入数据 
//	}		 

//	width = ex-sx-2+1;
//	height = ey-sy-2+1;			//高度

	  BlockWrite(sx,ex,sy,ey);
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width ; j++)
        {
         // LCD->LCD_RAM = color;  //写入数据
					LCD_WriteRAM(color[i*width+j]);
					//LCD->LCD_RAM=RGB_Data;
					/* 使用DMA进行数据传输 */
        }
    }
			
} 

/*******************************************************************************
* Function Name  : LCD_Fill_Area   20180329
* Description    : 在指定区域内填充指定颜色									
* Input          : Xpos_S---x轴起始坐标
*				   Ypos_S---y轴起始坐标
*				   Xpos_E---x轴结束坐标
*				   Ypos_E---y轴结束坐标	
*                  color----填充颜色
* Output         : None
* Return         : None
*******************************************************************************/

#define LCD_Use_Horizontal 1
void LCD_Fill_Area( uint16_t  Xpos_S, uint16_t  Ypos_S, uint16_t  Xpos_E, uint16_t  Ypos_E, uint16_t  color )
{                    

} 

/*******************************************************************************
* Function Name  : DisplayPicture
* Description    : 图片数组在屏幕上显示 
* Input          : 
* Output         : None
* Return         : None
*一下TFT的彩色,一个点占2个字节RGB565,也就是一个点可以有2^16种色调,一个个点组成了图片
* 图片为：２４０－３２０ 在屏中间显示
*******************************************************************************/
void DisplayPicture(void)
{
	uint16_t  temp,pos,t;  

//	BlockWrite(0,lcddev.width,0,lcddev.height);
	
	#if Pic    //320-320图片
	
	BlockWrite((lcddev.width-320)/2,(lcddev.width+320)/2-1,(lcddev.height-320)/2,(lcddev.height+320)/2-1); //OK 20171101　320-320图片
	for (pos=0;pos<320;pos++) //(pos=0;pos<lcddev.height;pos++)
	{
		for (t=0;t<320*2;t++) //(t=0;t<lcddev.width*2;t++) //
		{		
			temp=gImage[pos*320*2+t]*256+gImage[pos*320*2+t+1];
		
			t++;                                
			LCD_WriteRAM(temp);                             
		}                                
	}         	
	
	#else   //240-320图片

//	BlockWrite((lcddev.width-240)/2,(lcddev.width+240)/2-1,(lcddev.height-320)/2,(lcddev.height+320)/2-1); //OK 20171021  240-320图片
	BlockWrite((lcddev.width-240)/2,(lcddev.width+240)/2,(lcddev.height-320)/2,(lcddev.height+320)/2);  //20180727对应ＩＬＩ９４８１已在BlockWrite更新写法20180727
	for (pos=0;pos<320;pos++) // (pos=0;pos<lcddev.height;pos++)//
	{
		for(t=0;t<240*2;t++) //(t=0;t<lcddev.width*2;t++) 
		{
			temp=gImage[pos*240*2+t]*256+gImage[pos*240*2+t+1];
	
			t++;                                
			LCD_WriteRAM(temp);          
		}
	}
	
	#endif  	
	
}	

//图片128-128数组在屏幕上正中间显示 
void DisplayPicture1(void)
{

	uint16_t  temp,pos,t;  

//	BlockWrite(113,207,193,287);//OK 20170927
	BlockWrite((lcddev.width-128)/2,(lcddev.width+128)/2-1,(lcddev.height-128)/2,(lcddev.height+128)/2-1); //OK 20171005	
//	BlockWrite((lcddev.width-128)/2,(lcddev.width+128)/2,(lcddev.height-128)/2,(lcddev.height+128)/2); //OK 20180105 for GC9106
	
	for(pos=0;pos<128;pos++)
	{
		for(t=0;t<128*2;t++)
		{
			temp=gImage_128128[pos*128*2+t]*256+gImage_128128[pos*128*2+t+1];  
			t++;                                
			LCD_WriteRAM(temp);                                 
		}                                
	}

}	
/*******************************************************************************
// 移放置在TFTLCD_Init.c    20171120
//设置光标位置    MODIFY YZ
//Xpos:横坐标
//Ypos:纵坐标    
*******************************************************************************/
/*
void LCD_SetCursor(uint16_t  Xpos, uint16_t  Ypos)
{	 
	
//	LCD_Set(); 

//	LCD_WR_REG(lcddev.setxcmd); 
//	LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF); 			 
//	LCD_WR_REG(lcddev.setycmd); 
//	LCD_WR_DATA(Ypos>>8);LCD_WR_DATA(Ypos&0XFF); //此写法第一个点一直显示出来,奇怪 20170824

//	LCD_WR_8REG(lcddev.setxcmd); 
//	LCD_WR_8DATA(Xpos>>8);LCD_WR_8DATA(Xpos&0XFF); 			 
//	LCD_WR_8REG(lcddev.setycmd); 
//	LCD_WR_8DATA(Ypos>>8);LCD_WR_8DATA(Ypos&0XFF);   //OK20171005
//	LCD_WR_8REG(0x2C);
	
	//ILI9488 ,HX8357C,TEST 20171101  OK   要加后面的013F,01DF
	LCD_WR_8REG(0x2a);   
	LCD_WR_8DATA(Xpos>>8);
	LCD_WR_8DATA(Xpos&0xff);
	LCD_WR_8DATA(0x01);
	LCD_WR_8DATA(0x3F);

	LCD_WR_8REG(0x2b);   
	LCD_WR_8DATA(Ypos>>8);
	LCD_WR_8DATA(Ypos&0xff);
	LCD_WR_8DATA(0x01);
	LCD_WR_8DATA(0xDF);
	
	LCD_WR_8REG(0x2C);	
	
} 

*/

//以下为公共函数
/*******************************************************************************
//画点			modify	YZ
//x,y:坐标
//POINT_COLOR:此点的颜色
*******************************************************************************/
void LCD_DrawPoint(uint16_t  x,uint16_t  y)
{
	LCD_SetCursor(x,y);		//设置光标位置 
	
	LCD_WriteRAM(POINT_COLOR);
	
//	LCD->LCD_RAM=POINT_COLOR; 
	
}

/*******************************************************************************
//快速画点  modify YZ
//x,y:坐标
//color:颜色
*******************************************************************************/
void LCD_Fast_DrawPoint(uint16_t  x,uint16_t  y,uint16_t  color)
{	  
	LCD_SetCursor(x,y);	

	LCD_WriteRAM(color);
	
//	LCD->LCD_RAM=color;	
	
}

/*******************************************************************************
//画线				YZ
//x1,y1:起点坐标
//x2,y2:终点坐标  
*******************************************************************************/
void LCD_DrawLine(uint16_t  x1, uint16_t  y1, uint16_t  x2, uint16_t  y2)
{
	uint16_t  t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    

/*******************************************************************************
//画矩形	  YZ
//(x1,y1),(x2,y2):矩形的对角坐标
*******************************************************************************/
void LCD_DrawRectangle(uint16_t  x1, uint16_t  y1, uint16_t  x2, uint16_t  y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}

/*******************************************************************************
//在指定位置画一个指定大小的圆  YZ
//(x,y):中心点
//r    :半径
*******************************************************************************/
void LCD_Draw_Circle(uint16_t  x0,uint16_t  y0,uint8_t  r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 

/*******************************************************************************
//在指定位置显示一个字符   YZ
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
//解释：
//1、字符宽度(size/2)＝高度(size)一半。
//2、字符取模为竖向取模，即每列占几个字节，最后不够完整字节数的占一字节。
//3、字符所占空间为：每列所占字节＊列数。
//csize=(size/8+((size%8)?1:0))*(size/2)
//乘号*前为计算每列所占字节数，乘号*后为列数（字符高度一半）
*******************************************************************************/
void LCD_ShowChar(uint16_t  x,uint16_t  y,uint8_t  num,uint8_t  size,uint8_t  mode)
{  							  
    uint8_t  temp,t1,t;
	uint16_t  y0=y;
	uint8_t  csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//超区域了
				break;
			}
		}  	 
	}  	    	   	 	  
}  

/*******************************************************************************
//m^n函数  YZ
//返回值:m^n次方.
*******************************************************************************/
uint32_t LCD_Pow(uint8_t  m,uint8_t  n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}	

/*******************************************************************************
//显示数字,高位为0,则不显示  YZ
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);
*******************************************************************************/
void LCD_ShowNum(uint16_t  x,uint16_t  y,uint32_t num,uint8_t  len,uint8_t  size)
{         	
	uint8_t  t,temp;
	uint8_t  enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 

/*******************************************************************************
//显示数字,高位为0,还是显示  YZ
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
*******************************************************************************/
void LCD_ShowxNum(uint16_t  x,uint16_t  y,uint32_t num,uint8_t  len,uint8_t  size,uint8_t  mode)
{  
	uint8_t  t,temp;
	uint8_t  enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 

/*******************************************************************************
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
// *p:字符串起始地址		
//说明：字符是有点组成的，故在字符函数中不断调用画点函数LCD_DrawPoint();来画点（往LCD->LCD_RAM里写颜色值，这个颜色值在lcd.h里被定义为16位的地址）。
*******************************************************************************/
void LCD_ShowString(uint16_t  x,uint16_t  y,uint16_t  width,uint16_t  height,uint8_t  size,uint8_t  *p)
{         
	uint8_t  x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}

/***********************************End FUNCTION********************************************/

//显示水平彩条
void display_colorbar_H(void)	
{  
	uint16_t  x,y;
	
//	BlockWrite(0,480,0,100);
//	LCD->LCD_RAM = 0xf800;
//	LCD_WriteRAM_Prepare();     		  //开始写入GRAM	 	
//	
//	
//	BlockWrite(0,480,100,200);
//	LCD->LCD_RAM = 0x07E0;
//	LCD_WriteRAM_Prepare();     		  //开始写入GRAM	 	

//	
//	BlockWrite(0,480,200,300);
//	LCD->LCD_RAM = 0x001F;
//	LCD_WriteRAM_Prepare();     		  //开始写入GRAM	 	

//	
//	BlockWrite(0,480,300,400);
//	LCD->LCD_RAM = 0xFFE0;
//	LCD_WriteRAM_Prepare();     		  //开始写入GRAM	 	
//	
//	
//	BlockWrite(0,480,400,500);
//	LCD->LCD_RAM = 0x07FF;
//	LCD_WriteRAM_Prepare();     		  //开始写入GRAM	 	

//	
//	BlockWrite(0,480,500,600);
//	LCD->LCD_RAM = 0xF81F;
//	LCD_WriteRAM_Prepare();     		  //开始写入GRAM	 	

//	
//	BlockWrite(0,480,600,700);
//	LCD->LCD_RAM = 0xFFFF;
//	LCD_WriteRAM_Prepare();     		  //开始写入GRAM	 	

//	
//	BlockWrite(0,480,700,800);
//	LCD->LCD_RAM = 0x0000;
//	LCD_WriteRAM_Prepare();     		  //开始写入GRAM	 	

		
	for( y = 0 ; y < 100 ; y++ )
	  for( x = 0 ; x < 480 ; x++ )
	     {
				LCD_SetCursor(x,y);		 
				LCD_WriteRAM_Prepare();	
				LCD->LCD_RAM = 0xf800;
			 }
	
	for( y = 100 ; y < 200 ; y++ )
	  for( x = 0 ; x < 480 ; x++ )
	     {
				LCD_SetCursor(x,y);		 
				LCD_WriteRAM_Prepare();	
				LCD->LCD_RAM = 0x07E0;
			 }
			 
			 
	for( y = 200 ; y < 300 ; y++ )
	  for( x = 0 ; x < 480 ; x++ )
	     {
				LCD_SetCursor(x,y);		 
				LCD_WriteRAM_Prepare();	
				LCD->LCD_RAM = 0x001F;
			 }	 
			 
			 
	for( y = 300 ; y < 400 ; y++ )
	  for( x = 0 ; x < 480 ; x++ )
	     {
				LCD_SetCursor(x,y);		 
				LCD_WriteRAM_Prepare();	
				LCD->LCD_RAM = 0xFFE0;
			 }			 
			 
	for( y = 400 ; y < 500 ; y++ )
	  for( x = 0 ; x < 480 ; x++ )
	     {
				LCD_SetCursor(x,y);		 
				LCD_WriteRAM_Prepare();	
				LCD->LCD_RAM = 0x07FF;
			 }			 
			 
	for( y = 500 ; y < 600 ; y++ )
	  for( x = 0 ; x < 480 ; x++ )
	     {
				LCD_SetCursor(x,y);		 
				LCD_WriteRAM_Prepare();	
				LCD->LCD_RAM = 0xF81F;
			 }	
	for( y = 600 ; y < 700 ; y++ )
	  for( x = 0 ; x < 480 ; x++ )
	     {
				LCD_SetCursor(x,y);		 
				LCD_WriteRAM_Prepare();	
				LCD->LCD_RAM = 0xFFFF;
			 }	
	for( y = 700 ; y < 800 ; y++ )
	  for( x = 0 ; x < 480 ; x++ )
	     {
				LCD_SetCursor(x,y);		 
				LCD_WriteRAM_Prepare();	
				LCD->LCD_RAM = 0x0000;
			 }		 		 
}





