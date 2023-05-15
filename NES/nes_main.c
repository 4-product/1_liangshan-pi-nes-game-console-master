#include "nes_main.h" 
#include "nes_ppu.h"
#include "nes_mapper.h"
#include "nes_apu.h"
#include "malloc.h"
#include "string.h"
#include "circus.h"

#include "stdio.h"
#include "dac_audio.h"
#include "adc_key.h"
#include "systick.h"
#include "key.h"
#include "DacAudio.h"
#include "cw_queue.h"
#include "lcd_init.h"
//#include "bomb.h"

#include "ff.h"

#include "ui_app.h"
//extern set_ui  ui_set;

const uint8_t wavN[5]={0x1f,0x0f,0x07,0x03,0x01};
extern void rtc_last_show(void);


//////////////////////////////////////////////////////////////////////////////////	 
//本程序移植自网友ye781205的NES模拟器工程
//ALIENTEK STM32F407开发板
//NES主函数 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/7/1
//版本：V1.0  			  
////////////////////////////////////////////////////////////////////////////////// 	 
// #define __GWDBG__  
#ifdef __GWDBG__ 
#define GWDBG(format,...) printf("GWDBG[%s,%d] "format"\r\n",__func__,__LINE__, ##__VA_ARGS__)    
#else  
#define GWDBG(format,...)  
#endif   

//#define G_NES_GAM_RES g_bomb_data
//#define G_NES_GAM_RES g_circus_data
// #define mymalloc(memx,size) malloc(size)
// #define myfree(memx,ptr) free(ptr)

cw_cycle_queue t_queue;
u8 nes_frame_cnt;		//nes帧计数器 
int MapperNo;			//map编号
int NES_scanline;		//nes扫描线
int VROM_1K_SIZE;
int VROM_8K_SIZE;

u8 PADdata;   			//手柄1键值 [7:0]右7 左6 下5 上4 Start3 Select2 B1 A0  
u8 PADdata1;   			//手柄2键值 [7:0]右7 左6 下5 上4 Start3 Select2 B1 A0  
u8 *NES_RAM;			//保持1024字节对齐
u8 *NES_SRAM;  
NES_header *RomHeader; 	//rom文件头
MAPPER *NES_Mapper;		 
MapperCommRes *MAPx;  


u8* spr_ram;			//精灵RAM,256字节
ppu_data* ppu;			//ppu指针
u8* VROM_banks;
u8* VROM_tiles;

apu_t *apu; 			//apu指针
u16 *wave_buffers; 		
u8 *i2sbuf1; 			//音频缓冲帧,占用内存数 367*4 字节@22050Hz
//u8 *i2sbuf2; 			//音频缓冲帧,占用内存数 367*4 字节@22050Hz

u8* romfile;			//nes文件指针,指向整个nes文件的起始地址.
//////////////////////////////////////////////////////////////////////////////////////

 
//加载ROM
//返回值:0,成功
//    1,内存错误
//    3,map错误
u8 nes_load_rom(void)
{  
    u8* p;  
	u8 i;
	u8 res=0;
	p=(u8*)romfile;	
	if(strncmp((char*)p,"NES",3)==0)
	{  
		GWDBG("");
		RomHeader->ctrl_z=p[3];
		RomHeader->num_16k_rom_banks=p[4];
		RomHeader->num_8k_vrom_banks=p[5];
		RomHeader->flags_1=p[6];
		RomHeader->flags_2=p[7]; 
		if(RomHeader->flags_1&0x04)p+=512;		//有512字节的trainer:
		if(RomHeader->num_8k_vrom_banks>0)		//存在VROM,进行预解码
		{		
			VROM_banks=p+16+(RomHeader->num_16k_rom_banks*0x4000);
#if	NES_RAM_SPEED==1	//1:内存占用小 0:速度快	 
			VROM_tiles=VROM_banks;	 
#else  
			VROM_tiles=mymalloc(SRAMEX,RomHeader->num_8k_vrom_banks*8*1024);//这里可能申请多达1MB内存!!!
			if(VROM_tiles==0)VROM_tiles=VROM_banks;//内存不够用的情况下,尝试VROM_titles与VROM_banks共用内存			
			compile(RomHeader->num_8k_vrom_banks*8*1024/16,VROM_banks,VROM_tiles);  
#endif	
			GWDBG("");
		}else 
		{
			VROM_banks=mymalloc(SRAMIN,8*1024);
			VROM_tiles=mymalloc(SRAMEX,8*1024);
			if(!VROM_banks||!VROM_tiles)res=1;
			GWDBG("");
		}  	
		VROM_1K_SIZE = RomHeader->num_8k_vrom_banks * 8;
		VROM_8K_SIZE = RomHeader->num_8k_vrom_banks;  
		MapperNo=(RomHeader->flags_1>>4)|(RomHeader->flags_2&0xf0);
		if(RomHeader->flags_2 & 0x0E)MapperNo=RomHeader->flags_1>>4;//忽略高四位，如果头看起来很糟糕 
		GWDBG("use map:%d\r\n",MapperNo);
		GWDBG("");
		for(i=0;i<255;i++)  // 查找支持的Mapper号
		{		
			if (MapTab[i]==MapperNo)break;		
			if (MapTab[i]==-1)res=3; 
		} 
		if(res==0)
		{
			GWDBG("MapperNo:%d",MapperNo);
			switch(MapperNo)
			{
				case 1:  
					MAP1=mymalloc(SRAMIN,sizeof(Mapper1Res)); 
					if(!MAP1)res=1;
					break;
				case 4:  
				case 6: 
				case 16:
				case 17:
				case 18:
				case 19:
				case 21: 
				case 23:
				case 24:
				case 25:
				case 64:
				case 65:
				case 67:
				case 69:
				case 85:
				case 189:
					MAPx=mymalloc(SRAMIN,sizeof(MapperCommRes)); 
					if(!MAPx)res=1;
					break;  
				default:
					break;
			}
		}
	} 
	return res;	//返回执行结果
} 
//释放内存 
void nes_sram_free(void)
{ 
	myfree(SRAMIN,NES_RAM);		
	myfree(SRAMIN,NES_SRAM);	
	myfree(SRAMIN,RomHeader);	
	myfree(SRAMIN,NES_Mapper);
	myfree(SRAMIN,spr_ram);		
	myfree(SRAMIN,ppu);	
	myfree(SRAMIN,apu);	
	myfree(SRAMIN,wave_buffers);	
	myfree(SRAMIN,i2sbuf1);	
	//myfree(SRAMIN,i2sbuf2);	 
	myfree(SRAMEX,romfile);	  
	if((VROM_tiles!=VROM_banks)&&VROM_banks&&VROM_tiles)//如果分别为VROM_banks和VROM_tiles申请了内存,则释放
	{
		myfree(SRAMIN,VROM_banks);
		myfree(SRAMEX,VROM_tiles);		 
	}
	switch (MapperNo)//释放map内存
	{
		case 1: 			//释放内存
			myfree(SRAMIN,MAP1);
			break;	 	
		case 4: 
		case 6: 
		case 16:
		case 17:
		case 18:
		case 19:
		case 21:
		case 23:
		case 24:
		case 25:
		case 64:
		case 65:
		case 67:
		case 69:
		case 85:
		case 189:
			myfree(SRAMIN,MAPx);break;	 		//释放内存 
		default:break; 
	}
	NES_RAM=0;	
	NES_SRAM=0;
	RomHeader=0;
	NES_Mapper=0;
	spr_ram=0;
	ppu=0;
	apu=0;
	wave_buffers=0;
	i2sbuf1=0;
	//i2sbuf2=0;
	romfile=0; 
	VROM_banks=0;
	VROM_tiles=0; 
	MAP1=0;
	MAPx=0;
} 
//为NES运行申请内存
//romsize:nes文件大小
//返回值:0,申请成功
//       1,申请失败
u8 nes_sram_malloc(u32 romsize)
{
	u16 i=0;
	for(i=0;i<1024;i++)//为NES_RAM,查找1024对齐的内存
	{
		NES_SRAM=mymalloc(SRAMIN,i);
		NES_RAM=mymalloc(SRAMIN,0X800);	//申请2K字节,必须1024字节对齐
//        GWDBG("NES_RAM %0x",NES_RAM);
		if((u32)NES_RAM%1024)			//不是1024字节对齐
		{
			myfree(SRAMIN,NES_RAM);		//释放内存,然后重新尝试分配
			myfree(SRAMIN,NES_SRAM); 
		}else 
		{
			myfree(SRAMIN,NES_SRAM); 	//释放内存
			break;
		}
	}	 
 	NES_SRAM=mymalloc(SRAMIN,0X2000);
	RomHeader=mymalloc(SRAMIN,sizeof(NES_header));
	NES_Mapper=mymalloc(SRAMIN,sizeof(MAPPER));
	spr_ram=mymalloc(SRAMIN,0X100);		
	ppu=mymalloc(SRAMIN,sizeof(ppu_data));  
	apu=mymalloc(SRAMIN,sizeof(apu_t));		//sizeof(apu_t)=  12588
	wave_buffers=mymalloc(SRAMIN,APU_PCMBUF_SIZE*2);
	i2sbuf1=mymalloc(SRAMIN,APU_PCMBUF_SIZE*8+10);
	//i2sbuf2=mymalloc(SRAMIN,APU_PCMBUF_SIZE*4+10);
 	romfile=mymalloc(SRAMEX,romsize);			//申请游戏rom空间,等于nes文件大小 
	if(romfile==NULL)//内存不够?释放主界面占用内存,再重新申请
	{
		//spb_delete();//释放SPB占用的内存
		romfile=mymalloc(SRAMEX,romsize);//重新申请
	}
	GWDBG("i %d",i);
	GWDBG("NES_RAM %0x",NES_RAM);
	GWDBG("NES_SRAM %0x",NES_SRAM);
	GWDBG("RomHeader %0x",RomHeader);
	GWDBG("NES_Mapper %0x",NES_Mapper);
	GWDBG("spr_ram %0x",spr_ram);
	GWDBG("ppu %0x",ppu);
	GWDBG("apu %0x",apu);
	GWDBG("wave_buffers %0x",wave_buffers);
	GWDBG("i2sbuf1 %0x",i2sbuf1);
	//GWDBG("i2sbuf2 %0x",i2sbuf2);
	GWDBG("romfile %0x",romfile);

	if(i==1024||!NES_RAM||!NES_SRAM||!RomHeader||!NES_Mapper||!spr_ram||!ppu||!apu||!wave_buffers||!i2sbuf1||!romfile)
	{
		nes_sram_free();
		return 1;
	}
	memset(NES_SRAM,0,0X2000);				//清零
	memset(RomHeader,0,sizeof(NES_header));	//清零
	memset(NES_Mapper,0,sizeof(MAPPER));	//清零
	memset(spr_ram,0,0X100);				//清零
	memset(ppu,0,sizeof(ppu_data));			//清零
	memset(apu,0,sizeof(apu_t));			//清零
	memset(wave_buffers,0,APU_PCMBUF_SIZE*2);//清零
	memset(i2sbuf1,0,APU_PCMBUF_SIZE*4+10);	//清零
	//memset(i2sbuf2,0,APU_PCMBUF_SIZE*4+10);	//清零
	memset(romfile,0,romsize);				//清零 
	GWDBG("");
	return 0;
} 


//开始nes游戏
//pname:nes游戏路径
//返回值:
//0,正常退出
//1,内存错误
//2,文件错误
//3,不支持的map
u8 nes_load1(u8* pname)
{
	 FIL *file; 
	 UINT br;
	u8 res=0;  
    u32 number=0;
    
	GWDBG("");
    
    file=mymalloc(SRAMIN,sizeof(FIL));
    if(file==0)return 1;
    res=f_open(file,(char*)pname,FA_READ);
	if(res!=FR_OK)	//打开文件失败
	{
		myfree(SRAMIN,file);
		return 2;
	}
    GWDBG(" %s OK! ",pname);
    GWDBG("file->obj.objsize %d  ",(file->obj.objsize));
    
	res=nes_sram_malloc(file->obj.objsize);			//申请内存 
	cw_queue_init(&t_queue,i2sbuf1,APU_PCMBUF_SIZE*8,delay_1ms);
	cw_queue_clean(&t_queue);
	if(res==0)
	{
		GWDBG("");
        while(1)
        {
            f_read(file,romfile+number,512,&br);	//读取nes文件
            number+=br;
            if((number >= (file->obj.objsize))) break;
        }
        
        //f_read(file,romfile,file->fsize,&br);	//读取nes文件
		res=nes_load_rom();						//加载ROM
		if(res==0) 					
		{   
			GWDBG("");
			Mapper_Init();						//map初始化
			GWDBG("");
			cpu6502_init();						//初始化6502,并复位	 
			GWDBG(""); 	 
			PPU_reset();						//ppu复位
			GWDBG("");
			apu_init(); 						//apu初始化 
			GWDBG("");
			if(ui_set.volume_class) nes_sound_open(0,APU_SAMPLE_RATE);	//初始化播放设备
			GWDBG("");
			nes_emulate_frame();				//进入NES模拟器主循环 
			GWDBG("");
			nes_sound_close();					//关闭声音输出
			GWDBG("");
		}
		GWDBG("");
	}
	f_close(file);
    myfree(SRAMIN,file);//释放内存
	nes_sram_free();	//释放内存
	GWDBG("res:%d",res);
	return res;
}  





u8 nes_xoff=0;	//显示在x轴方向的偏移量(实际显示宽度=256-2*nes_xoff)
//设置游戏显示窗口
void nes_set_window(void)
{	

	LCD_Address_Set(0,0,240,280);//设置显示范围


	// u16 xoff=0,yoff=0; 
	// u16 lcdwidth,lcdheight;
	// if(lcddev.width==240)
	// {
	// 	lcdwidth=240;
	// 	lcdheight=240;
	// 	nes_xoff=(256-lcddev.width)/2;	//得到x轴方向的偏移量
 	// 	xoff=0; 
	// }else if(lcddev.width==320) 
	// {
	// 	lcdwidth=256;
	// 	lcdheight=240; 
	// 	nes_xoff=0;
	// 	xoff=(lcddev.width-256)/2;
	// }else if(lcddev.width==480)
	// {
	// 	lcdwidth=480;
	// 	lcdheight=480; 
	// 	nes_xoff=(256-(lcddev.width/2))/2;//得到x轴方向的偏移量
 	// 	xoff=0;  
	// }	
	// yoff=480 ;//(lcddev.height-lcdheight-gui_phy.tbheight)/2+gui_phy.tbheight;//屏幕高度 
	// //LCD_Set_Window(xoff,yoff,lcdwidth,lcdheight);//让NES始终在屏幕的正中央显示
	// LCD_SetCursor(xoff,yoff);
	// LCD_WriteRAM_Prepare();//写入LCD RAM的准备   

}

extern void KEYBRD_FCPAD_Decode(u8 *fcbuf,u8 mode);
//读取游戏手柄数据

void nes_get_gamepadval(void)
{  
    //手柄1键值 [7:0]右7 左6 下5 上4 Start3 Select2 B1 A0  
    // uint16_t key_val[]={0,4,5,6,7,0xB1,0xA0};

    //右128 左64 下32 上16 Start 8 Select 4 B 2 A0
    //uint16_t five_key_Val[]={0,16,32,64,128};
    //uint16_t one_key_Val[]={0,8,4,2,1};

    volatile static uint32_t time = 0;

    if (g_systick-10 > time)
    {
        PADdata = 0;
        PADdata1=0;

       PADdata = five_way_key_scan();
       PADdata |= get_key_val();
        
       //PADdata1=PADdata;
    }
}    
//nes模拟器主循环
void nes_emulate_frame(void)
{  
	u8 nes_frame;
	//TIM3_Int_Init(10000-1,8400-1);//启动TIM3 ,1s中断一次	//GWLOSE
	// nes_set_window();//设置窗口
	while(1)
	{	
		// LINES 0-239
		PPU_start_frame();
		
		for(NES_scanline = 0; NES_scanline< 240; NES_scanline++)
		{
			run6502(113*256);
			NES_Mapper->HSync(NES_scanline);
			//扫描一行		  
			if(nes_frame==0)scanline_draw(NES_scanline);
			else do_scanline_and_dont_draw(NES_scanline); 
		}  
		NES_scanline=240;
		run6502(113*256);//运行1线
		NES_Mapper->HSync(NES_scanline); 
		start_vblank(); 
		if(NMI_enabled()) 
		{
			cpunmi=1;
			run6502(7*256);//运行中断
		}
		NES_Mapper->VSync();
		// LINES 242-261    
		for(NES_scanline=241;NES_scanline<262;NES_scanline++)
		{
			run6502(113*256);	  
			NES_Mapper->HSync(NES_scanline);		  
		}	   
		end_vblank(); 
		nes_get_gamepadval();//每3帧查询一次USB
		if(ui_set.volume_class) apu_soundoutput();//输出游戏声音	 
		nes_frame_cnt++; 	
		nes_frame++;
		if(nes_frame>NES_SKIP_FRAME)nes_frame=0;//跳帧 画面突然加速
        //nes_frame=0;//跳帧 
        
        rtc_last_show();
        if(PADdata == 0x0c) break; //两个按键同时按
        
		//GWLOSE
		// if(system_task_return)break;//TPAD返回  
		// if(spbdev.spbheight==0&&spbdev.spbwidth==0)//产生了来电事件需要重新设置窗口
		// {
		// 	nes_set_window();
		// }
	}
	// LCD_Set_Window(0,0,lcddev.width,lcddev.height);//恢复屏幕窗口
	// TIM3->CR1&=~(1<<0);//关闭定时器3
}
//在6502.s里面被调用
void debug_6502(u16 reg0,u8 reg1)
{
	GWDBG("6502 error:%x,%d\r\n",reg0,reg1);
}
////////////////////////////////////////////////////////////////////////////////// 	 
//nes,音频输出支持部分
//I2S音频播放回调函数

//NES打开音频输出
int nes_sound_open(int samples_per_sync,int sample_rate) 
{
	
	GWDBG("samples_per_sync:%d sample_rate:%d\r\n",samples_per_sync,sample_rate);
	DacAudio_Play(sample_rate);  //22050
 	return 1;
}
//NES关闭音频输出
void nes_sound_close(void) 
{ 
	// I2S_Play_Stop();
	// app_wm8978_volset(0);				//关闭WM8978音量输出
} 
////NES音频输出到I2S缓存
//void nes_apu_fill_buffer(int samples,u16* wavebuf)
//{	
//	int ret = 0;
//	uint8_t data = 0;
//	for(int i = 0; i<APU_PCMBUF_SIZE; i++)
//	{
//        
////        printf(" 0x%04x ",wavebuf[i]);
////        if(wavebuf[i] > 0x8000) data = (wavebuf[i]>>8)+ ;
////            else data = (wavebuf[i]>>8) + 0x80;
//        
//		data = ( (wavebuf[i]>>8)  + 128);//
//		ret = cw_queue_write(&t_queue,&data,1,10);
//		if(!ret){//if write ok start next Rx 
//			GWDBG("queue write faild");
//		}
//	}
//    
//} 



/////音频输出验证
//NES音频输出到I2S缓存
void nes_apu_fill_buffer(int samples,u16* wavebuf)
{
    int ret = 0;
    int i;
    uint8_t data = 0;

    for(i = 0; i<APU_PCMBUF_SIZE; i++)
    {
        if(ui_set.volume_class)
        {
            data = wavebuf[i]>>8;
            if(data > 0x80) data = 0x80 - ((0xff - (data + (data & wavN[ui_set.volume_class])))>>(6-ui_set.volume_class));//
            else data = ((data + (data & wavN[ui_set.volume_class]))  >>(6-ui_set.volume_class)) + 0x80;//
        }
        else data = 0x80;
        ret = cw_queue_write(&t_queue,&data,1,10);
        if(!ret){//if write ok start next Rx 
            GWDBG("queue write faild");
        }
    }
} 


















