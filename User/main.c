/*!
	\file    main.c
	\brief   led spark with systick

	\version 2016-08-15, V1.0.0, firmware for GD32F4xx
	\version 2018-12-12, V2.0.0, firmware for GD32F4xx
	\version 2020-09-30, V2.1.0, firmware for GD32F4xx
	\version 2022-03-09, V3.0.0, firmware for GD32F4xx
*/

/*
	Copyright (c) 2022, GigaDevice Semiconductor Inc.

	Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice, this
	   list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright notice,
	   this list of conditions and the following disclaimer in the documentation
	   and/or other materials provided with the distribution.
	3. Neither the name of the copyright holder nor the names of its contributors
	   may be used to endorse or promote products derived from this software without
	   specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "usart0.h"
#include "led.h"
#include "key.h"
#include "beep.h"
#include "spi.h"
#include "adc_key.h"
#include "taskTimer.h"
#include "dac_audio.h"
#include "exmc_sdram.h"
#include "nes_main.h"
#include "malloc.h"
#ifdef _LCD_8080_
#include "lcd9841.h"
#include "TFTLCD_Init.h"
#else
#include "lcd_init.h"
#include "lcd.h"
#endif
#include "Dac.h"
#include "DacAudio.h"
#include "sdcard.h"
#include "ff.h"
#include "string.h"
#include "fontupd.h"
#include "text.h"	
#include "bsp_pwm.h"
#include "rtc.h"
#include "adc_bat.h"
//#include "flash_nes.h"
#include "ui_app.h"
#include "fat_app.h"

    
    
    
void game_start_main(void);///开始游戏
void game_manage_main(void);///管理游戏
void game_last_main(void);///上次游戏
void game_set_main(void);//游戏设置


void key_wav_motor(void);//按键声音震动
void TIMER3_IRQHandler(void);/////按键音及按键震动中断
void RTC_WKUP_IRQHandler(void);/////1S中断内处理
void HardFault_Handler(void);////硬件错误处理

/*!
	\brief    main function
	\param[in]  none
	\param[out] none
	\retval     none
*/
int main(void)
{
    //初始化
    systick_config();
    usart0_init();
    adc_config();
    adc_bat_init();
    key_init();
    led_init();
    ui_main_set_init();////开机掉电设置
    ui_main_lcd_init();//开机logo显示
    keywav_init(ui_set.volume_class);
    Dac_Init();
    motro_gpio_config();
    key_timeinit(600*50);    //按键音频率
    rtctime_init();
    //sram初始化
    exmc_synchronous_dynamic_ram_init(EXMC_SDRAM_DEVICE0);
    my_mem_init(SRAMIN);
    my_mem_init(SRAMEX);
    my_mem_init(SRAMCCM);
    //文件系统
    exfuns_init();  //为fatfs相关变量申请内存    
    sd_fatfs_init();//sd fatfs初始化
    spi_fatfs_init();//spi flash fatfs+字库 初始化
    //无字库显示英文
    if(ui_ram.language==0 && fat_ram.word_stock==0) ui_ram.language=1;
    ///主界面小图标显示
    rtc_init_show();
    ///主界面大图标全部显示
    ui_main_init_show();
    
    while(1)
    {
        //主界面选择
        ui_adckey.now_adckey = five_way_key_scan();
        if( ui_adckey.last_adckey != ui_adckey.now_adckey)
        {
            ui_adckey.last_adckey = ui_adckey.now_adckey;
            if(ui_adckey.last_adckey == adckey_null) continue;
            key_wav_motor();//按键声音震动
            //循环切换
            //if(ui_ram.now_app & 0xf0) ui_ram.now_app &= 0x0f;
            //else ui_ram.now_app |= 0x10;
            //if(ui_ram.now_app & 0x0f) ui_ram.now_app &= 0xf0;
            //else ui_ram.now_app |= 0x01;
            //不可循环切换 上下同时选中
            if(ui_adckey.last_adckey & adckey_up) ui_ram.now_app &= 0x0f;
            else if(ui_adckey.last_adckey & adckey_down) ui_ram.now_app |= 0x10;
            else;
            if(ui_adckey.last_adckey & adckey_left) ui_ram.now_app &= 0xf0;
            else if(ui_adckey.last_adckey & adckey_right) ui_ram.now_app |= 0x01;
            else;
        }
        //选择变化显示
        ui_main_last_show();
        ///确定进入功能
        ui_key.now_key = get_key_val();
        if(ui_key.last_key != ui_key.now_key)
        {
            ui_key.last_key = ui_key.now_key;
            if(ui_key.last_key == key_null) continue;
            if(ui_key.last_key & key_exit) continue;
            key_wav_motor();//按键声音震动
            //进入功能
            switch(ui_ram.last_app)
            {
                case game_start:
                    game_start_main();//开始游戏
                    break;
                case game_last:
                    game_last_main();//上次游戏
                    break;
                case game_manage:
                    game_manage_main();//管理游戏
                    break;
                case game_set:
                    game_set_main();//游戏设置
                    break;
                default:
                    break;
            }
        }
        //小图标显示
        rtc_last_show();
        delay_1ms(10);
    }
}




///开始游戏
void game_start_main(void)
{
    u8 res;
    
    ///选择界面大图标全部显示
    ui_select_init_show();
    while(1)
    {
        //选择
        ui_adckey.now_adckey = five_way_key_scan();
        if( ui_adckey.last_adckey != ui_adckey.now_adckey)
        {
            ui_adckey.last_adckey = ui_adckey.now_adckey;
            if(ui_adckey.last_adckey == adckey_null) continue;
            if(ui_adckey.last_adckey & adckey_up_down) continue;
            key_wav_motor();
            if(ui_adckey.last_adckey & adckey_left_right)
            {
                //不可循环
                if(ui_adckey.last_adckey & adckey_left) ui_ram.now_sdspi = 0;
                else if(ui_adckey.last_adckey & adckey_right) ui_ram.now_sdspi = 1;
                else;
            }
        }
        //选择变化界面
        ui_select_last_show();
        //进入游戏
        ui_key.now_key = get_key_val();
        if( ui_key.last_key != ui_key.now_key)
        {
            ui_key.last_key = ui_key.now_key;
            if(ui_key.last_key == key_null) continue;
            key_wav_motor();
            if(ui_key.last_key & key_exit) 
            {
                ///主界面大图标全部显示
                ui_main_init_show();
                //while(get_key_val());
                break;
            }
            ///目录游戏打开  maxnes  fat_ram.sd_nesname
            if(ui_ram.now_sdspi) res = spiflash_file_search();
            else res = sd_file_search();
            if(res)
            {
                //错误提示 
                ui_error_show((u8 *)ui_show[ui_ram.language][uierror_dir_show]);
                ///选择界面大图标全部显示
                ui_select_init_show();
                while(get_key_val());
                continue;
            }
            //初始化变量
            ui_ram.sd_show_number=0; //显示位置
            ui_ram.spi_show_number=0; //显示位置
            ui_ram.sd_make_number=0;//选择游戏
            ui_ram.spi_make_number=0;//选择游戏
            ///初始化游戏选择界面
            ui_selectgame_init_show();
            //选择游戏
            while(1)
            {
                //摇杆方向
                ui_adckey.now_adckey = five_way_key_scan();
                if( ui_adckey.last_adckey != ui_adckey.now_adckey)
                {
                    ui_adckey.last_adckey = ui_adckey.now_adckey;
                    if(ui_adckey.last_adckey == adckey_null) continue;
                    if(ui_adckey.last_adckey & adckey_left_right) continue;
                    key_wav_motor();
                    if(ui_adckey.last_adckey & adckey_up_down)
                    {
                        ui_ram.now_game=0xff; //变化
                    }
                }
                ///选择游戏变化界面
                ui_selectgame_last_show();
                //进入游戏
                ui_key.now_key = get_key_val();
                if( ui_key.last_key != ui_key.now_key)
                {
                    ui_key.last_key = ui_key.now_key;
                    if(ui_key.last_key == key_null) continue;
                    key_wav_motor();
                    if(ui_key.last_key & key_exit)
                    {
                        ///选择界面大图标全部显示
                        ui_select_init_show();
                        //while(get_key_val());
                        break;
                    }
                    if(ui_key.last_key & key_enter) 
                    {
                        //保存历史游戏记录
                        printf("//////////\r\n");
                        res = f_open(filin,"1:/LastGame.txt",FA_CREATE_ALWAYS|FA_WRITE);//打开文件 记录
                        if(res)
                        {
                            printf("1:/LastGame.txt FA_WRITE OFF!  %d \r\n",res);
                            LCD_Fill(0,uihint_show_start,240,280,WHITE);
                            Show_Str(((240-strlen("[记录失败]")*8)/2),uihint_show_line,"[记录失败]",LCEDA,WHITE,16,0);
                        }
                        if(ui_ram.now_sdspi) strcpy((char*)&buf_r,(char*)fat_ram.spi_nesname[ui_ram.spi_make_number]);
                        else strcpy((char*)&buf_r,(char*)fat_ram.sd_nesname[ui_ram.sd_make_number]);
                        f_write(filin,buf_r,512,&bw);
                        f_close(filin);//关闭文件
                        
                        
                        /////开始打开游戏
                        LCD_Fill(0,uihint_show_start,240,280,WHITE);
                        Show_Str(0,uihint_show_line,buf_r,LCEDA,WHITE,16,0);
                        res = f_open(filout,(char *)buf_r,FA_READ);//打开文件
                        if(res == FR_OK)
                        {
                            printf("nes_load start\r\n");
                            nes_load1(buf_r);
                            printf("nes_load end\r\n");
                        }
                        else
                        {
                            //错误提示
                            printf("//ui_error_show:  %s  error: %d \r\n",buf_r , res);
                            ui_error_show((u8 *)ui_show[ui_ram.language][uierror_file_show]);
                            while(get_key_val());
                        }
                        ///初始化游戏选择界面
                        ui_selectgame_init_show();
                        while(get_key_val());
                    }
                }
                //小图标显示
                rtc_last_show();
                delay_1ms(10);
            }
        }
        //小图标显示
        rtc_last_show();
        delay_1ms(10);
    }
}


///上次游戏
void game_last_main(void)
{
    u8 res;
    
    //打开文件
    res = f_open(filout,"1:/LastGame.txt",FA_READ);//打开文件
    if(res)
    {
        printf("1:/LastGame.txt FA_READ OFF!  %d \r\n",res);
        //错误提示
        ui_error_show((u8 *)ui_show[ui_ram.language][uierror_file_show]);
        ///主界面大图标全部显示
        ui_main_init_show();
        while(get_key_val()); 
    }
    else 
    {
        //读取文件
        f_read(filout,buf_r,512,&br);
        LCD_Fill(0,uihint_show_start,240,280,WHITE);
        Show_Str(0,uihint_show_line,buf_r,LCEDA,WHITE,16,0);
        printf("f_read  %s \r\n",buf_r);
        //开始游戏
        res = f_open(filout,(char *)buf_r,FA_READ);//打开文件
        if(res == FR_OK)
        {
            printf("nes_load start\r\n");
            nes_load1(buf_r);
            printf("nes_load end\r\n");
        }
        else
        {
            //错误提示
            ui_error_show((u8 *)ui_show[ui_ram.language][uierror_file_show]);
            while(get_key_val());
        }
        ///主界面大图标全部显示
        ui_main_init_show();
        //while(get_key_val());
    }
    f_close(filout);//关闭文件
}
///管理游戏
void game_manage_main(void)
{
    u8 res;
    
    //必须SPI flash 文件系统存在
    if((fat_ram.spi_flash == 0x81))
    {
        //检索SD卡游戏
        sd_file_search();
        //检索SPI游戏
        spiflash_file_search();
        //初始化变量
        ui_ram.sd_show_number=0; //显示位置
        ui_ram.spi_show_number=0; //显示位置
        ui_ram.sd_make_number=0;//选择游戏
        ui_ram.spi_make_number=0;//选择游戏
        ui_ram.last_nes_sdspi=0;//选择
        ui_ram.now_nes_sdspi=0;//选择
        //复制删除游戏文件 初始化显示
        ui_copygame_init_show();
        while(1)
        {
            //操作按键 选择文件和切换功能
            ui_adckey.now_adckey = five_way_key_scan();
            if( ui_adckey.last_adckey != ui_adckey.now_adckey)
            {
                ui_adckey.last_adckey = ui_adckey.now_adckey;
                if(ui_adckey.last_adckey == adckey_null) continue;
                key_wav_motor();
                if(ui_adckey.last_adckey & adckey_up_down)
                {
                    ui_ram.now_game=0xff;
                }
                else if(ui_adckey.last_adckey == adckey_left) //左
                {
                    if(ui_ram.now_nes_sdspi) ui_ram.now_nes_sdspi=0;
                }
                else if(ui_adckey.last_adckey == adckey_right) //右
                {
                    if(ui_ram.now_nes_sdspi == 0) ui_ram.now_nes_sdspi=0x01;
                }
                 else;
            }
            //复制删除游戏文件 切换显示 
            ui_copygame_last_show();
            //进入游戏
            ui_key.now_key = get_key_val();
            if( ui_key.last_key != ui_key.now_key)
            {
                ui_key.last_key = ui_key.now_key;
                if(ui_key.last_key == key_null) continue;
                key_wav_motor();
                if(ui_key.last_key & key_exit) //返回
                {
                    ///主界面大图标全部显示
                    ui_main_init_show();
                    //while(get_key_val());
                    break;
                }
                if(ui_key.last_key & key_enter)
                {
                    if(ui_ram.now_nes_sdspi)
                    {//删除文件
                        if(fat_ram.spi_flash == 0x81)
                        {
                            f_unlink((const char*)fat_ram.spi_nesname[ui_ram.spi_make_number]);
                            while(exf_getfree("1:",&fat_ram.spi_total,&fat_ram.spi_free))delay_1ms(10);
                            //检索SPI游戏
                            spiflash_file_search();
                            //位置归零
                            //ui_ram.spi_show_number=0;
                            //ui_ram.spi_make_number=0;
                            //位置继承
                            if(ui_ram.spi_show_number) ui_ram.spi_show_number--;
                            if(ui_ram.spi_make_number) ui_ram.spi_make_number--;
                            //变化spi文件后刷新
                            ui_spigame_show();
                        }
                    }
                    else
                    {//复制文件
                        if((fat_ram.sd_card == 0x81) ||  (fat_ram.spi_flash == 0x81))
                        {
                            //文件复制
                            strcpy((char*)&pname,"1:/nes/");
                            strcat((char*)&pname,(const char*)fat_ram.sd_nesname[ui_ram.sd_make_number]+7);//将文件名接在后面
                            res=f_open(filout,(char*)pname,FA_READ);//打开文件
                            if(res) 
                            {
                                //复制文件
                                copy_file_file(fat_ram.sd_nesname[ui_ram.sd_make_number],pname);
                                ///////列表更新
                                while(exf_getfree("1:",&fat_ram.spi_total,&fat_ram.spi_free))delay_1ms(10);
                                //检索SPI游戏
                                spiflash_file_search();
                                //位置归零
                                //ui_ram.spi_show_number=0;
                                //ui_ram.spi_make_number=0;
                                //位置继承
                                //变化spi文件后刷新
                                ui_spigame_show();
                            }
                        }
                    }
                }
                else;
            }
            //小图标显示
            rtc_last_show();
            delay_1ms(10);
        }
    }
    else
    {
        //错误提示
        ui_error_show((u8 *)ui_show[ui_ram.language][uierror_file_show]);
        ///主界面大图标全部显示
        ui_main_init_show();
        while(get_key_val());
    }
}
//游戏设置
void game_set_main(void)
{
    //变量初始化
    ui_ram.now_set=0;
    ui_ram.last_set=0;
    //设置界面全刷新显示
    ui_set_init_show();
    while(1)
    {
        //主界面选择
        ui_adckey.now_adckey = five_way_key_scan();
        if( ui_adckey.last_adckey != ui_adckey.now_adckey)
        {
            ui_adckey.last_adckey = ui_adckey.now_adckey;
            if(ui_adckey.last_adckey == adckey_null) continue;
            if(ui_adckey.last_adckey & adckey_left_right) continue;
            key_wav_motor();
            if(ui_adckey.last_adckey & adckey_up_down)
            {
                if(ui_adckey.last_adckey & 0x10) 
                {
                    if(ui_ram.now_set) ui_ram.now_set--;
                }
                else if(ui_adckey.last_adckey & 0x20) 
                {
                    if(ui_ram.now_set < (uiset_number-1)) ui_ram.now_set++;
                }
                else;
            }
        }
        ///确定进入功能
        ui_key.now_key = get_key_val();
        if(ui_key.last_key != ui_key.now_key)
        {
            ui_key.last_key = ui_key.now_key;
            if(ui_key.last_key == key_null) continue;
            key_wav_motor();
            if(ui_key.last_key & key_exit) 
            {
                ///主界面大图标全部显示
                ui_main_init_show();
                //while(get_key_val());
                break;
            }
            else
            {
                ui_ram.now_makeset=0xff;//变化标志
            }
        }
        ///变化显示
        ui_set_last_show();
        //小图标显示
        rtc_last_show();
        delay_1ms(10);
    }
}
//按键声音震动
void key_wav_motor(void)
{
    if(ui_set.key_volume || ui_set.key_shake)
    {
        if(ui_set.key_shake) motor_on();
        timer_enable(TIMER3);
    }
}


/////按键音及按键震动中断
void TIMER3_IRQHandler(void)
{
    if(ui_set.key_volume)
    {
        dac_data_set(DAC1, DAC_ALIGN_8B_R,wavbuff[wavnumber%50]);
        dac_software_trigger_enable(DAC1);
    }
    if(++wavnumber > 1000)
    {
        motor_off();
        wavnumber=0;
        dac_data_set(DAC1, DAC_ALIGN_8B_R,0x80);
        dac_software_trigger_enable(DAC1);
        timer_disable(TIMER3);
    }
    timer_interrupt_flag_clear(TIMER3, TIMER_INT_FLAG_UP);
}
/////中断内处理
void RTC_WKUP_IRQHandler(void)
{
    if(RESET != rtc_flag_get(RTC_FLAG_WT))
    {
        exti_flag_clear(EXTI_22);
        rtc_flag_clear(RTC_FLAG_WT);
        ui_ram.show_state |= 1; //小图标更新标志
        
        LED2_TOGGLE;
    } 
}
////硬件错误处理
void HardFault_Handler(void)
{
    uint32_t temp;
    temp = SCB->CFSR;			  // fault状态寄存器(@0XE000ED28)包括:MMSR,BFSR,UFSR
    printf("CFSR:%8X\r\n", temp); //显示错误值
    temp = SCB->HFSR;			  //硬件fault状态寄存器
    printf("HFSR:%8X\r\n", temp); //显示错误值
    temp = SCB->DFSR;			  //调试fault状态寄存器
    printf("DFSR:%8X\r\n", temp); //显示错误值
    temp = SCB->AFSR;			  //辅助fault状态寄存器
    printf("AFSR:%8X\r\n", temp); //显示错误值

    NVIC_SystemReset();
}



