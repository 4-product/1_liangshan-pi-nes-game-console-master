#include "ui_app.h"
#include <stdio.h>
#ifdef _LCD_8080_
#include "lcd9841.h"
#include "TFTLCD_Init.h"
#else
#include "lcd_init.h"
#include "lcd.h"
#endif
#include "image.h"
#include "text.h"
#include "string.h"
#include "Dac.h"
#include "rtc.h"
#include "key.h"
#include "adc_key.h"
#include "adc_bat.h"
#include "i2c.h"
#include "eeprom.h"


#include "fat_app.h"

show_ui ui_ram;
set_ui  ui_set;



///各种显示字符串的位置
const char *ui_show[2][41]={
{
"[开始游戏]",
"[上次游戏]",
"[管理游戏]",
"[游戏设置]",
"[SD卡 存储]",
"[SPI FLASH 存储]",
"[SD卡 游戏选择]",
"[SPI FLASH 游戏选择]",
" SD卡:     /     MB    个",
" SPI Flash:     /     KB    个",
"[复制游戏]",
"[删除游戏]",
" 未找到文件！",
"[存在故障!!]",
"[NES游戏机设置]",
"1：操作说明",
"2：显示语言",    
"3：屏幕亮度",
"4：音量等级",
"5：按键声音",
"6：按键震动",
"7：计时显示",
"[A或B]",
"[中文]",
"[  级]",
"[开]",
"[关]",
"[主界面按键说明：]",
"摇杆：上下左右 功能：上下左右",
"按键：KEY-A/B  功能：确定",
"按键：KEY-L/R  功能：返回",
"[游戏界面按键说明：]",
"摇杆：上下左右 功能：上下左右",
"按键：KEY-A  功能：NES-A",
"按键：KEY-B  功能：NES-B",
"按键：KEY-L  功能：NES-Start",
"按键：KEY-R  功能：NES-Select",
"按键：[R+L]  功能：退出游戏",
"目录未找到或打开失败！",
"文件未找到或打开失败！",
"文件复制进度：   %",
},
{
"[Start Game]",
"[Last Game]",
"[Manage Game]",
"[Device Set]",
"[SD Card Store]",
"[SPI FLASH Store]",
"[SD Card Game Select]",
"[SPI FLASH Game Select]",
"   SD:     /     MB    N ",
" SPI Flash:     /     KB    N ",
"[Copy Game]",
"[Delete Game]",
" No File Found!",
"[Existing Problem!!]",
"[NES Games Set]",
"1: Play Explain",
"2: Show Language",    
"3: LCD Luminance",
"4: Volume Class",
"5: Key Voice",
"6: Key Shake",
"7: Show Time",
"[A-B] ",
"[ En ]",
"[  Lv]",
"[ON] ",
"[OFF]",
"[Main KEY Explain: ]",
"Key: U/D/L/R  M: U/D/L/R",
"Key: KEY-A/B  M: Enter",
"Key: KEY-L/R  M: Exit",
"[NES KEY Explain: ]",
"Key: U/D/L/R  M: U/D/L/R",
"Key: KEY-A  M: NES-A",
"Key: KEY-B  M: NES-B",
"Key: KEY-L  M: NES-Start",
"Key: KEY-R  M: NES-Select",
"Key: [R+L]  M: Exit",
"Dir No Found Or Opened!",
"File No Found Or Opened!",
"file copy on:    %",
},
};




//开机掉电设置
void ui_main_set_init(void)
{
    i2c_config();
    i2c_eeprom_init();
    eeprom_buffer_read_timeout((u8 *)&ui_set,0,sizeof(ui_set));
    //默认
    if(ui_set.language > 1) ui_set.language=0;
    if(ui_set.luminance > 10) ui_set.luminance=3;
    if(ui_set.volume_class > 3) ui_set.volume_class=1;
    if(ui_set.key_volume > 1) ui_set.key_volume=1;
    if(ui_set.key_shake > 1) ui_set.key_shake=1;
    if(ui_set.show_time > 1) ui_set.show_time=1;
    
}
//开机logo显示
void ui_main_lcd_init(void)
{
    LCD_Init();
    LCD_timer_config();
    LCD_ShowPicture1(0,0,LCD_W,LCD_H,START_BMP); //开机logo
    LCD_ShowString(((240-strlen("LCD_Init OK!")*8)/2),uihint_show_start,"LCD_Init OK!",LCEDA,WHITE,16,0);
    timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_1,50+ui_set.luminance*10);
    delay_1ms(500);//  
}




///初始化显示主界面
void ui_main_init_show(void)
{
    //清空
    LCD_Fill(0,uitela_show_start,LCD_W,LCD_H,WHITE);
    //显示
    switch(ui_ram.now_app)
    {
        case game_start:
            LCD_ShowPicture1((uimain_bmp_paly_x-(uimain_bmp_szie>>1)),(uimain_bmp_paly_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[onplay_number]);
            LCD_ShowPicture1((uimain_bmp_last_x-(uimain_bmp_szie>>1)),(uimain_bmp_last_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[offlast_number]);
            LCD_ShowPicture1((uimain_bmp_copy_x-(uimain_bmp_szie>>1)),(uimain_bmp_copy_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[offcopy_number]);
            LCD_ShowPicture1((uimain_bmp_set_x-(uimain_bmp_szie>>1)),(uimain_bmp_set_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[offset_number]);
            Show_Str(((240-strlen((char*)ui_show[ui_set.language][uimain_playh_show])*8)/2),uihint_show_line,(u8*)ui_show[ui_set.language][uimain_playh_show],LCEDA,WHITE,16,0);
            break;
        case game_last:
            LCD_ShowPicture1((uimain_bmp_paly_x-(uimain_bmp_szie>>1)),(uimain_bmp_paly_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[offplay_number]);
            LCD_ShowPicture1((uimain_bmp_last_x-(uimain_bmp_szie>>1)),(uimain_bmp_last_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[onlast_number]);
            LCD_ShowPicture1((uimain_bmp_copy_x-(uimain_bmp_szie>>1)),(uimain_bmp_copy_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[offcopy_number]);
            LCD_ShowPicture1((uimain_bmp_set_x-(uimain_bmp_szie>>1)),(uimain_bmp_set_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[offset_number]);
            Show_Str(((240-strlen((char*)ui_show[ui_set.language][uimain_last_show])*8)/2),uihint_show_line,(u8*)ui_show[ui_set.language][uimain_last_show],LCEDA,WHITE,16,0);
            break;
        case game_manage:
            LCD_ShowPicture1((uimain_bmp_paly_x-(uimain_bmp_szie>>1)),(uimain_bmp_paly_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[offplay_number]);
            LCD_ShowPicture1((uimain_bmp_last_x-(uimain_bmp_szie>>1)),(uimain_bmp_last_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[offlast_number]);
            LCD_ShowPicture1((uimain_bmp_copy_x-(uimain_bmp_szie>>1)),(uimain_bmp_copy_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[oncopy_number]);
            LCD_ShowPicture1((uimain_bmp_set_x-(uimain_bmp_szie>>1)),(uimain_bmp_set_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[offset_number]);
            Show_Str(((240-strlen((char*)ui_show[ui_set.language][uimain_copy_show])*8)/2),uihint_show_line,(u8*)ui_show[ui_set.language][uimain_copy_show],LCEDA,WHITE,16,0);
            break;
        case game_set:
            LCD_ShowPicture1((uimain_bmp_paly_x-(uimain_bmp_szie>>1)),(uimain_bmp_paly_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[offplay_number]);
            LCD_ShowPicture1((uimain_bmp_last_x-(uimain_bmp_szie>>1)),(uimain_bmp_last_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[offlast_number]);
            LCD_ShowPicture1((uimain_bmp_copy_x-(uimain_bmp_szie>>1)),(uimain_bmp_copy_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[offcopy_number]);
            LCD_ShowPicture1((uimain_bmp_set_x-(uimain_bmp_szie>>1)),(uimain_bmp_set_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[onset_number]);
            Show_Str(((240-strlen((char*)ui_show[ui_set.language][uimain_set_show])*8)/2),uihint_show_line,(u8*)ui_show[ui_set.language][uimain_set_show],LCEDA,WHITE,16,0);
            break;
        default:
            break;
    }
}
////操作有变化显示
void ui_main_last_show(void)
{
    if(ui_ram.now_app != ui_ram.last_app)
    {
        //消除
        switch(ui_ram.last_app)
        {
            case game_start:
                LCD_ShowPicture1((uimain_bmp_paly_x-(uimain_bmp_szie>>1)),(uimain_bmp_paly_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[offplay_number]);
                break;
            case game_last:
                LCD_ShowPicture1((uimain_bmp_last_x-(uimain_bmp_szie>>1)),(uimain_bmp_last_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[offlast_number]);
                break;
            case game_manage:
                LCD_ShowPicture1((uimain_bmp_copy_x-(uimain_bmp_szie>>1)),(uimain_bmp_copy_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[offcopy_number]);
                break;
            case game_set:
                LCD_ShowPicture1((uimain_bmp_set_x-(uimain_bmp_szie>>1)),(uimain_bmp_set_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[offset_number]);
                break;
            default:
                break;
        }
        //显示
        LCD_Fill(0,uihint_show_start,240,uihint_show_stop,WHITE);
        switch(ui_ram.now_app)
        {
            case game_start:
                LCD_ShowPicture1((uimain_bmp_paly_x-(uimain_bmp_szie>>1)),(uimain_bmp_paly_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[onplay_number]);
                Show_Str(((240-strlen((char*)ui_show[ui_set.language][uimain_playh_show])*8)/2),uihint_show_line,(u8*)ui_show[ui_set.language][uimain_playh_show],LCEDA,WHITE,16,0);
                break;
            case game_last:
                LCD_ShowPicture1((uimain_bmp_last_x-(uimain_bmp_szie>>1)),(uimain_bmp_last_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[onlast_number]);
                Show_Str(((240-strlen((char*)ui_show[ui_set.language][uimain_last_show])*8)/2),uihint_show_line,(u8*)ui_show[ui_set.language][uimain_last_show],LCEDA,WHITE,16,0);
                break;
            case game_manage:
                LCD_ShowPicture1((uimain_bmp_copy_x-(uimain_bmp_szie>>1)),(uimain_bmp_copy_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[oncopy_number]);
                Show_Str(((240-strlen((char*)ui_show[ui_set.language][uimain_copy_show])*8)/2),uihint_show_line,(u8*)ui_show[ui_set.language][uimain_copy_show],LCEDA,WHITE,16,0);
                break;
            case game_set:
                LCD_ShowPicture1((uimain_bmp_set_x-(uimain_bmp_szie>>1)),(uimain_bmp_set_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[onset_number]);
                Show_Str(((240-strlen((char*)ui_show[ui_set.language][uimain_set_show])*8)/2),uihint_show_line,(u8*)ui_show[ui_set.language][uimain_set_show],LCEDA,WHITE,16,0);
                break;
            default:
                break;
        }
        ui_ram.last_app = ui_ram.now_app;
    }
}
///初始化sd spi选择界面
void ui_select_init_show(void)
{
    //清空
    LCD_Fill(0,uitela_show_start,LCD_W,LCD_H,WHITE);
    //显示
    if(ui_ram.now_sdspi)
    {   //SPI
        LCD_ShowPicture1((uiselect_bmp_sd_x-(uimain_bmp_szie>>1)),(uiselect_bmp_sd_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[offsd_number]);
        LCD_ShowPicture1((uiselect_bmp_spi_x-(uimain_bmp_szie>>1)),(uiselect_bmp_spi_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[onspi_number]);
        Show_Str(((240-strlen((char*)ui_show[ui_set.language][uiselect_spi_save])*8)/2),uihint_show_line,(u8*)ui_show[ui_set.language][uiselect_spi_save],LCEDA,WHITE,16,0);
    }
    else
    {   //SD
        LCD_ShowPicture1((uiselect_bmp_sd_x-(uimain_bmp_szie>>1)),(uiselect_bmp_sd_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[onsd_number]);
        LCD_ShowPicture1((uiselect_bmp_spi_x-(uimain_bmp_szie>>1)),(uiselect_bmp_spi_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[offspi_number]);
        Show_Str(((240-strlen((char*)ui_show[ui_set.language][uiselect_sd_save])*8)/2),uihint_show_line,(u8*)ui_show[ui_set.language][uiselect_sd_save],LCEDA,WHITE,16,0);
    }
}
///选择变化界面
void ui_select_last_show(void)
{
    //显示
    if(ui_ram.now_sdspi != ui_ram.last_sdspi)
    {
        ui_ram.last_sdspi = ui_ram.now_sdspi;
        LCD_Fill(0,uihint_show_start,240,uihint_show_stop,WHITE);
        if(ui_ram.last_sdspi)
        {   //SPI
            LCD_ShowPicture1((uiselect_bmp_sd_x-(uimain_bmp_szie>>1)),(uiselect_bmp_sd_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[offsd_number]);
            LCD_ShowPicture1((uiselect_bmp_spi_x-(uimain_bmp_szie>>1)),(uiselect_bmp_spi_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[onspi_number]);
            Show_Str(((240-strlen((char*)ui_show[ui_set.language][uiselect_spi_save])*8)/2),uihint_show_line,(u8*)ui_show[ui_set.language][uiselect_spi_save],LCEDA,WHITE,16,0);
        }
        else
        {   //SD
            LCD_ShowPicture1((uiselect_bmp_sd_x-(uimain_bmp_szie>>1)),(uiselect_bmp_sd_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[onsd_number]);
            LCD_ShowPicture1((uiselect_bmp_spi_x-(uimain_bmp_szie>>1)),(uiselect_bmp_spi_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[offspi_number]);
            Show_Str(((240-strlen((char*)ui_show[ui_set.language][uiselect_sd_save])*8)/2),uihint_show_line,(u8*)ui_show[ui_set.language][uiselect_sd_save],LCEDA,WHITE,16,0);
        }
    }
}

///初始化游戏选择界面
void ui_selectgame_init_show(void)
{
    uint8_t i;
    //清空
    LCD_Fill(0,uitela_show_start,LCD_W,LCD_H,WHITE);
    //判断sd卡还是spi
    if(ui_ram.now_sdspi) 
    {//spi
        //显示文件系统标题
        Show_Str(0,uitela_show_start,(u8*)ui_show[ui_set.language][uiselect_spi_title],WHITE,LCEDA,16,0);
        LCD_Fill(8*strlen((char*)ui_show[ui_set.language][uiselect_spi_title]),uitela_show_start,240,uitela_show_start+uiszie,LCEDA); //填满
        LCD_ShowIntNum(8*11,uitela_show_start,fat_ram.spi_total,5,WHITE,LCEDA,16);
        LCD_ShowIntNum(8*17,uitela_show_start,fat_ram.spi_free,5,WHITE,LCEDA,16);
        LCD_ShowIntNum(8*25,uitela_show_start,(fat_ram.spi_nesnumber),3,WHITE,LCEDA,16);
        //显示文件名
        for(i=0;i<uiselect_game_show;i++)
        {
            if(ui_ram.spi_show_number+i < fat_ram.spi_nesnumber)
            {
                if((ui_ram.spi_show_number + i) == ui_ram.spi_make_number) 
                {
                    Show_Str(0,uiselect_show_start+i*uiszie,fat_ram.spi_nesname[ui_ram.spi_show_number+i],WHITE,LCEDA1,16,0);
                    LCD_Fill((8*strlen((char*)fat_ram.spi_nesname[ui_ram.spi_show_number+i])),uiselect_show_start+i*uiszie,240,uiselect_show_start+(i+1)*uiszie,LCEDA1);
                }
                else 
                {
                    Show_Str(0,uiselect_show_start+i*uiszie,fat_ram.spi_nesname[ui_ram.spi_show_number+i],LCEDA,WHITE,16,0);
                }
            }
        }
        //显示提示栏
        Show_Str(((240-strlen((char*)ui_show[ui_set.language][uiselect_spi_game])*8)/2),uihint_show_line,(u8*)ui_show[ui_set.language][uiselect_spi_game],LCEDA,WHITE,16,0);
    }
    else
    {//sd
        //显示文件系统标题
        Show_Str(0,uitela_show_start,(u8*)ui_show[ui_set.language][uiselect_sd_title],WHITE,LCEDA,16,0);
        LCD_Fill(8*strlen((char*)ui_show[ui_set.language][uiselect_sd_title]),uitela_show_start,240,uitela_show_start+uiszie,LCEDA); //填满
        LCD_ShowIntNum(8*6,uitela_show_start,fat_ram.sd_total>>10,5,WHITE,LCEDA,16);
        LCD_ShowIntNum(8*12,uitela_show_start,fat_ram.sd_free>>10,5,WHITE,LCEDA,16);
        LCD_ShowIntNum(8*20,uitela_show_start,(fat_ram.sd_nesnumber),3,WHITE,LCEDA,16);
        //显示文件名
        for(i=0;i<uiselect_game_show;i++)
        {
            if(ui_ram.sd_show_number+i < fat_ram.sd_nesnumber)
            {
                if((ui_ram.sd_show_number + i) == ui_ram.sd_make_number) 
                {
                    Show_Str(0,uiselect_show_start+i*uiszie,fat_ram.sd_nesname[ui_ram.sd_show_number+i],WHITE,LCEDA1,16,0);
                    LCD_Fill((8*strlen((char*)fat_ram.sd_nesname[ui_ram.sd_show_number+i])),uiselect_show_start+i*uiszie,240,uiselect_show_start+(i+1)*uiszie,LCEDA1);
                }
                else 
                {
                    Show_Str(0,uiselect_show_start+i*uiszie,fat_ram.sd_nesname[ui_ram.sd_show_number+i],LCEDA,WHITE,16,0);
                }
            }
        }
        //显示提示栏
        Show_Str(((240-strlen((char*)ui_show[ui_set.language][uiselect_sd_game])*8)/2),uihint_show_line,(u8*)ui_show[ui_set.language][uiselect_sd_game],LCEDA,WHITE,16,0);
    }
}
///选择游戏变化界面
void ui_selectgame_last_show(void)
{
    uint8_t i;
    //显示
    if(ui_ram.last_game != ui_ram.now_game)
    {
        ui_ram.now_game=0;
        ui_ram.last_game = ui_ram.now_game;
        
        //判断sd卡还是spi
        if(ui_ram.now_sdspi) 
        {
            //不可循环
            if(ui_adckey.now_adckey & adckey_down) 
            {
                if(ui_ram.spi_make_number < (fat_ram.spi_nesnumber-1)) 
                {
                    if(++ui_ram.spi_make_number >= (14+ui_ram.spi_show_number))
                    {
                        //移动一行
                        ui_ram.spi_show_number++;
                        for(i=0;i<uiselect_game_show;i++)
                        {
                            if((ui_ram.spi_show_number + i) == ui_ram.spi_make_number) 
                            {
                                Show_Str(0,uiselect_show_start+i*16,fat_ram.spi_nesname[ui_ram.spi_show_number+i],WHITE,LCEDA1,16,0);
                                LCD_Fill((8*strlen((char*)fat_ram.spi_nesname[ui_ram.spi_show_number+i])),uiselect_show_start+i*16,240,uiselect_show_start+(i+1)*16,LCEDA1);
                            }
                            else 
                            {
                                Show_Str(0,uiselect_show_start+(i)*16,fat_ram.spi_nesname[ui_ram.spi_show_number+i],LCEDA,WHITE,16,0);
                                LCD_Fill((8*strlen((char*)fat_ram.spi_nesname[ui_ram.spi_show_number+i])),uiselect_show_start+i*16,240,uiselect_show_start+(i+1)*16,WHITE);
                            }
                        }
                    }
                    else
                    {
                        Show_Str(0,uiselect_show_start+(ui_ram.spi_make_number-ui_ram.spi_show_number-1)*16,fat_ram.spi_nesname[ui_ram.spi_make_number-1],LCEDA,WHITE,16,0);
                        LCD_Fill((8*strlen((char*)fat_ram.spi_nesname[ui_ram.spi_make_number-1])),uiselect_show_start+(ui_ram.spi_make_number-ui_ram.spi_show_number-1)*16,240,uiselect_show_start+(ui_ram.spi_make_number-ui_ram.spi_show_number)*16,WHITE);
                        Show_Str(0,uiselect_show_start+(ui_ram.spi_make_number-ui_ram.spi_show_number)*16,fat_ram.spi_nesname[ui_ram.spi_make_number],WHITE,LCEDA1,16,0);
                        LCD_Fill((8*strlen((char*)fat_ram.spi_nesname[ui_ram.spi_make_number])),uiselect_show_start+(ui_ram.spi_make_number-ui_ram.spi_show_number)*16,240,uiselect_show_start+(ui_ram.spi_make_number-ui_ram.spi_show_number+1)*16,LCEDA1);
                    }
                }
            }
            else if(ui_adckey.now_adckey & adckey_up) 
            {
                if(ui_ram.spi_make_number)
                {
                    if(--ui_ram.spi_make_number < ui_ram.spi_show_number)
                    {
                        //移动一行
                        ui_ram.spi_show_number--;
                        for(i=0;i<uiselect_game_show;i++)
                        {
                            if((ui_ram.spi_show_number + i) == ui_ram.spi_make_number) 
                            {
                                Show_Str(0,uiselect_show_start+(i)*16,fat_ram.spi_nesname[ui_ram.spi_show_number+i],WHITE,LCEDA1,16,0);
                                LCD_Fill((8*strlen((char*)fat_ram.spi_nesname[ui_ram.spi_show_number+i])),uiselect_show_start+i*16,240,uiselect_show_start+(i+1)*16,LCEDA1);
                            }
                            else 
                            {
                                Show_Str(0,uiselect_show_start+i*16,fat_ram.spi_nesname[ui_ram.spi_show_number+i],LCEDA,WHITE,16,0);
                                LCD_Fill((8*strlen((char*)fat_ram.spi_nesname[ui_ram.spi_show_number+i])),uiselect_show_start+i*16,240,uiselect_show_start+(i+1)*16,WHITE);
                            }
                        }
                    }
                    else
                    {
                        Show_Str(0,uiselect_show_start+(ui_ram.spi_make_number-ui_ram.spi_show_number+1)*16,fat_ram.spi_nesname[ui_ram.spi_make_number+1],LCEDA,WHITE,16,0);
                        LCD_Fill((8*strlen((char*)fat_ram.spi_nesname[ui_ram.spi_make_number+1])),uiselect_show_start+(ui_ram.spi_make_number-ui_ram.spi_show_number+1)*16,240,uiselect_show_start+(ui_ram.spi_make_number-ui_ram.spi_show_number+2)*16,WHITE);
                        Show_Str(0,uiselect_show_start+(ui_ram.spi_make_number-ui_ram.spi_show_number)*16,fat_ram.spi_nesname[ui_ram.spi_make_number],WHITE,LCEDA1,16,0);
                        LCD_Fill((8*strlen((char*)fat_ram.spi_nesname[ui_ram.spi_make_number])),uiselect_show_start+(ui_ram.spi_make_number-ui_ram.spi_show_number)*16,240,uiselect_show_start+(ui_ram.spi_make_number-ui_ram.spi_show_number+1)*16,LCEDA1);
                    }
                }
            }
            else;
        }
        else
        {//sd
            //不可循环
            if(ui_adckey.now_adckey & adckey_down) 
            {
                if(ui_ram.sd_make_number < (fat_ram.sd_nesnumber-1)) 
                {
                    if(++ui_ram.sd_make_number >= (14+ui_ram.sd_show_number))
                    {
                        //移动一行
                        ui_ram.sd_show_number++;
                        for(i=0;i<uiselect_game_show;i++)
                        {
                            if((ui_ram.sd_show_number + i) == ui_ram.sd_make_number) 
                            {
                                Show_Str(0,uiselect_show_start+i*16,fat_ram.sd_nesname[ui_ram.sd_show_number+i],WHITE,LCEDA1,16,0);
                                LCD_Fill((8*strlen((char*)fat_ram.sd_nesname[ui_ram.sd_show_number+i])),uiselect_show_start+i*16,240,uiselect_show_start+(i+1)*16,LCEDA1);
                            }
                            else 
                            {
                                Show_Str(0,uiselect_show_start+(i)*16,fat_ram.sd_nesname[ui_ram.sd_show_number+i],LCEDA,WHITE,16,0);
                                LCD_Fill((8*strlen((char*)fat_ram.sd_nesname[ui_ram.sd_show_number+i])),uiselect_show_start+i*16,240,uiselect_show_start+(i+1)*16,WHITE);
                            }
                        }
                    }
                    else
                    {
                        Show_Str(0,uiselect_show_start+(ui_ram.sd_make_number-ui_ram.sd_show_number-1)*16,fat_ram.sd_nesname[ui_ram.sd_make_number-1],LCEDA,WHITE,16,0);
                        LCD_Fill((8*strlen((char*)fat_ram.sd_nesname[ui_ram.sd_make_number-1])),uiselect_show_start+(ui_ram.sd_make_number-ui_ram.sd_show_number-1)*16,240,uiselect_show_start+(ui_ram.sd_make_number-ui_ram.sd_show_number)*16,WHITE);
                        Show_Str(0,uiselect_show_start+(ui_ram.sd_make_number-ui_ram.sd_show_number)*16,fat_ram.sd_nesname[ui_ram.sd_make_number],WHITE,LCEDA1,16,0);
                        LCD_Fill((8*strlen((char*)fat_ram.sd_nesname[ui_ram.sd_make_number])),uiselect_show_start+(ui_ram.sd_make_number-ui_ram.sd_show_number)*16,240,uiselect_show_start+(ui_ram.sd_make_number-ui_ram.sd_show_number+1)*16,LCEDA1);
                    }
                }
            }
            else if(ui_adckey.now_adckey & adckey_up) 
            {
                if(ui_ram.sd_make_number)
                {
                    if(--ui_ram.sd_make_number < ui_ram.sd_show_number)
                    {
                        //移动一行
                        ui_ram.sd_show_number--;
                        for(i=0;i<uiselect_game_show;i++)
                        {
                            if((ui_ram.sd_show_number + i) == ui_ram.sd_make_number) 
                            {
                                Show_Str(0,uiselect_show_start+(i)*16,fat_ram.sd_nesname[ui_ram.sd_show_number+i],WHITE,LCEDA1,16,0);
                                LCD_Fill((8*strlen((char*)fat_ram.sd_nesname[ui_ram.sd_show_number+i])),uiselect_show_start+i*16,240,uiselect_show_start+(i+1)*16,LCEDA1);
                            }
                            else 
                            {
                                Show_Str(0,uiselect_show_start+i*16,fat_ram.sd_nesname[ui_ram.sd_show_number+i],LCEDA,WHITE,16,0);
                                LCD_Fill((8*strlen((char*)fat_ram.sd_nesname[ui_ram.sd_show_number+i])),uiselect_show_start+i*16,240,uiselect_show_start+(i+1)*16,WHITE);
                            }
                        }
                    }
                    else
                    {
                        Show_Str(0,uiselect_show_start+(ui_ram.sd_make_number-ui_ram.sd_show_number+1)*16,fat_ram.sd_nesname[ui_ram.sd_make_number+1],LCEDA,WHITE,16,0);
                        LCD_Fill((8*strlen((char*)fat_ram.sd_nesname[ui_ram.sd_make_number+1])),uiselect_show_start+(ui_ram.sd_make_number-ui_ram.sd_show_number+1)*16,240,uiselect_show_start+(ui_ram.sd_make_number-ui_ram.sd_show_number+2)*16,WHITE);
                        Show_Str(0,uiselect_show_start+(ui_ram.sd_make_number-ui_ram.sd_show_number)*16,fat_ram.sd_nesname[ui_ram.sd_make_number],WHITE,LCEDA1,16,0);
                        LCD_Fill((8*strlen((char*)fat_ram.sd_nesname[ui_ram.sd_make_number])),uiselect_show_start+(ui_ram.sd_make_number-ui_ram.sd_show_number)*16,240,uiselect_show_start+(ui_ram.sd_make_number-ui_ram.sd_show_number+1)*16,LCEDA1);
                    }
                }
            }
            else;
        }
    }
}
//复制删除游戏文件 初始化显示
void ui_copygame_init_show(void)
{
    uint8_t i;
    //清空
    LCD_Fill(0,uitela_show_start,LCD_W,LCD_H,WHITE);
    //显示标题及内容
    if(ui_ram.now_nes_sdspi)
    {//SPI
        //sd
        Show_Str(0,uicopy_showsd_init,(u8*)ui_show[ui_set.language][uiselect_sd_title],WHITE,LGRAY,16,0);
        LCD_Fill(8*strlen((char*)ui_show[ui_set.language][uiselect_sd_title]),uicopy_showsd_init,240,uicopy_showsd_init+uiszie,LGRAY); //填满
        LCD_ShowIntNum(8*6,uicopy_showsd_init,fat_ram.sd_total>>10,5,WHITE,LGRAY,16);
        LCD_ShowIntNum(8*12,uicopy_showsd_init,fat_ram.sd_free>>10,5,WHITE,LGRAY,16);
        LCD_ShowIntNum(8*20,uicopy_showsd_init,(fat_ram.sd_nesnumber),3,WHITE,LGRAY,16);
        //spi
        Show_Str(0,uicopy_showspi_init,(u8*)ui_show[ui_set.language][uiselect_spi_title],WHITE,LCEDA,16,0);
        LCD_Fill(8*strlen((char*)ui_show[ui_set.language][uiselect_spi_title]),uicopy_showspi_init,240,uicopy_showspi_init+uiszie,LCEDA); //填满
        LCD_ShowIntNum(8*11,uicopy_showspi_init,fat_ram.spi_total,5,WHITE,LCEDA,16);
        LCD_ShowIntNum(8*17,uicopy_showspi_init,fat_ram.spi_free,5,WHITE,LCEDA,16);
        LCD_ShowIntNum(8*25,uicopy_showspi_init,(fat_ram.spi_nesnumber),3,WHITE,LCEDA,16);
        //显示列表
        Show_Str(0,uicopy_showsd_start,fat_ram.sd_sizename[0],WHITE,LGRAY,16,0);
        LCD_Fill(8*strlen((char*)fat_ram.sd_sizename[0]),uicopy_showsd_start,240,uicopy_showsd_start+16,LGRAY);
        Show_Str(0,uicopy_showspi_start,fat_ram.spi_sizename[0],WHITE,LCEDA1,16,0);
        LCD_Fill(8*strlen((char*)fat_ram.spi_sizename[0]),uicopy_showspi_start,240,uicopy_showspi_start+16,LCEDA1);
        for(i=1;i<uicopy_game_show;i++)
        {
            if(i<fat_ram.sd_nesnumber)Show_Str(0,uicopy_showsd_start+i*16,fat_ram.sd_sizename[i],LCEDA,WHITE,16,0);
            if(i<fat_ram.spi_nesnumber)Show_Str(0,uicopy_showspi_start+i*16,fat_ram.spi_sizename[i],LCEDA,WHITE,16,0);
        }
        //提示
        Show_Str(((240-strlen((char*)ui_show[ui_set.language][uigame_delete])*8)/2),uihint_show_line,(u8 *)ui_show[ui_set.language][uigame_delete],LCEDA,WHITE,16,0);
    }
    else
    {//SD
        //sd
        Show_Str(0,uicopy_showsd_init,(u8*)ui_show[ui_set.language][uiselect_sd_title],WHITE,LCEDA,16,0);
        LCD_Fill(8*strlen((char*)ui_show[ui_set.language][uiselect_sd_title]),uicopy_showsd_init,240,uicopy_showsd_init+uiszie,LCEDA); //填满
        LCD_ShowIntNum(8*6,uicopy_showsd_init,fat_ram.sd_total>>10,5,WHITE,LCEDA,16);
        LCD_ShowIntNum(8*12,uicopy_showsd_init,fat_ram.sd_free>>10,5,WHITE,LCEDA,16);
        LCD_ShowIntNum(8*20,uicopy_showsd_init,(fat_ram.sd_nesnumber),3,WHITE,LCEDA,16);
        //spi
        Show_Str(0,uicopy_showspi_init,(u8*)ui_show[ui_set.language][uiselect_spi_title],WHITE,LGRAY,16,0);
        LCD_Fill(8*strlen((char*)ui_show[ui_set.language][uiselect_spi_title]),uicopy_showspi_init,240,uicopy_showspi_init+uiszie,LGRAY); //填满
        LCD_ShowIntNum(8*11,uicopy_showspi_init,fat_ram.spi_total,5,WHITE,LGRAY,16);
        LCD_ShowIntNum(8*17,uicopy_showspi_init,fat_ram.spi_free,5,WHITE,LGRAY,16);
        LCD_ShowIntNum(8*25,uicopy_showspi_init,(fat_ram.spi_nesnumber),3,WHITE,LGRAY,16);
        //显示列表
        Show_Str(0,uicopy_showsd_start,fat_ram.sd_sizename[0],WHITE,LCEDA1,16,0);
        LCD_Fill(8*strlen((char*)fat_ram.sd_sizename[0]),uicopy_showsd_start,240,uicopy_showsd_start+16,LCEDA1);
        Show_Str(0,uicopy_showspi_start,fat_ram.spi_sizename[0],WHITE,LGRAY,16,0);
        LCD_Fill(8*strlen((char*)fat_ram.spi_sizename[0]),uicopy_showspi_start,240,uicopy_showspi_start+16,LGRAY);
        for(i=1;i<uicopy_game_show;i++)
        {
            if(i<fat_ram.sd_nesnumber)Show_Str(0,uicopy_showsd_start+i*16,fat_ram.sd_sizename[i],LCEDA,WHITE,16,0);
            if(i<fat_ram.spi_nesnumber)Show_Str(0,uicopy_showspi_start+i*16,fat_ram.spi_sizename[i],LCEDA,WHITE,16,0);
        }
        //提示
        Show_Str(((240-strlen((char*)ui_show[ui_set.language][uigame_copy])*8)/2),uihint_show_line,(u8 *)ui_show[ui_set.language][uigame_copy],LCEDA,WHITE,16,0);
    }
}

//复制删除游戏文件 切换显示
void ui_copygame_last_show(void)
{
    uint8_t i;
    //功能切换
    if(ui_ram.last_nes_sdspi != ui_ram.now_nes_sdspi)
    {
        ui_ram.last_nes_sdspi = ui_ram.now_nes_sdspi;
        if(ui_ram.now_nes_sdspi)
        {//SPI
            //sd
            Show_Str(0,uicopy_showsd_init,(u8*)ui_show[ui_set.language][uiselect_sd_title],WHITE,LGRAY,16,0);
            LCD_Fill(8*strlen((char*)ui_show[ui_set.language][uiselect_sd_title]),uicopy_showsd_init,240,uicopy_showsd_init+uiszie,LGRAY); //填满
            LCD_ShowIntNum(8*6,uicopy_showsd_init,fat_ram.sd_total>>10,5,WHITE,LGRAY,16);
            LCD_ShowIntNum(8*12,uicopy_showsd_init,fat_ram.sd_free>>10,5,WHITE,LGRAY,16);
            LCD_ShowIntNum(8*20,uicopy_showsd_init,(fat_ram.sd_nesnumber),3,WHITE,LGRAY,16);
            Show_Str(0,uicopy_showsd_start+(ui_ram.sd_make_number-ui_ram.sd_show_number)*16,fat_ram.sd_sizename[ui_ram.sd_make_number],WHITE,LGRAY,16,0);
            LCD_Fill((8*strlen((char*)fat_ram.sd_sizename[ui_ram.sd_make_number])),uicopy_showsd_start+(ui_ram.sd_make_number-ui_ram.sd_show_number)*16,240,uicopy_showsd_start+(ui_ram.sd_make_number-ui_ram.sd_show_number+1)*16,LGRAY);
            //spi
            Show_Str(0,uicopy_showspi_init,(u8*)ui_show[ui_set.language][uiselect_spi_title],WHITE,LCEDA,16,0);
            LCD_Fill(8*strlen((char*)ui_show[ui_set.language][uiselect_spi_title]),uicopy_showspi_init,240,uicopy_showspi_init+uiszie,LCEDA); //填满
            LCD_ShowIntNum(8*11,uicopy_showspi_init,fat_ram.spi_total,5,WHITE,LCEDA,16);
            LCD_ShowIntNum(8*17,uicopy_showspi_init,fat_ram.spi_free,5,WHITE,LCEDA,16);
            LCD_ShowIntNum(8*25,uicopy_showspi_init,(fat_ram.spi_nesnumber),3,WHITE,LCEDA,16);
            Show_Str(0,uicopy_showspi_start+(ui_ram.spi_make_number-ui_ram.spi_show_number)*16,fat_ram.spi_sizename[ui_ram.spi_make_number],WHITE,LCEDA1,16,0);
            LCD_Fill((8*strlen((char*)fat_ram.spi_sizename[ui_ram.spi_make_number])),uicopy_showspi_start+(ui_ram.spi_make_number-ui_ram.spi_show_number)*16,240,uicopy_showspi_start+(ui_ram.spi_make_number-ui_ram.spi_show_number+1)*16,LCEDA1);
            //提示
            LCD_Fill(0,uihint_show_start,240,uihint_show_stop,WHITE);
            Show_Str(((240-strlen((char*)ui_show[ui_set.language][uigame_delete])*8)/2),uihint_show_line,(u8 *)ui_show[ui_set.language][uigame_delete],LCEDA,WHITE,16,0);
        }
        else
        {//SD
            //sd
            Show_Str(0,uicopy_showsd_init,(u8*)ui_show[ui_set.language][uiselect_sd_title],WHITE,LCEDA,16,0);
            LCD_Fill(8*strlen((char*)ui_show[ui_set.language][uiselect_sd_title]),uicopy_showsd_init,240,uicopy_showsd_init+uiszie,LCEDA); //填满
            LCD_ShowIntNum(8*6,uicopy_showsd_init,fat_ram.sd_total>>10,5,WHITE,LCEDA,16);
            LCD_ShowIntNum(8*12,uicopy_showsd_init,fat_ram.sd_free>>10,5,WHITE,LCEDA,16);
            LCD_ShowIntNum(8*20,uicopy_showsd_init,(fat_ram.sd_nesnumber),3,WHITE,LCEDA,16);
            Show_Str(0,uicopy_showsd_start+(ui_ram.sd_make_number-ui_ram.sd_show_number)*16,fat_ram.sd_sizename[ui_ram.sd_make_number],WHITE,LCEDA1,16,0);
            LCD_Fill((8*strlen((char*)fat_ram.sd_sizename[ui_ram.sd_make_number])),uicopy_showsd_start+(ui_ram.sd_make_number-ui_ram.sd_show_number)*16,240,uicopy_showsd_start+(ui_ram.sd_make_number-ui_ram.sd_show_number+1)*16,LCEDA1);
            //spi
            Show_Str(0,uicopy_showspi_init,(u8*)ui_show[ui_set.language][uiselect_spi_title],WHITE,LGRAY,16,0);
            LCD_Fill(8*strlen((char*)ui_show[ui_set.language][uiselect_spi_title]),uicopy_showspi_init,240,uicopy_showspi_init+uiszie,LGRAY); //填满
            LCD_ShowIntNum(8*11,uicopy_showspi_init,fat_ram.spi_total,5,WHITE,LGRAY,16);
            LCD_ShowIntNum(8*17,uicopy_showspi_init,fat_ram.spi_free,5,WHITE,LGRAY,16);
            LCD_ShowIntNum(8*25,uicopy_showspi_init,(fat_ram.spi_nesnumber),3,WHITE,LGRAY,16);
            Show_Str(0,uicopy_showspi_start+(ui_ram.spi_make_number-ui_ram.spi_show_number)*16,fat_ram.spi_sizename[ui_ram.spi_make_number],WHITE,LGRAY,16,0);
            LCD_Fill((8*strlen((char*)fat_ram.spi_sizename[ui_ram.spi_make_number])),uicopy_showspi_start+(ui_ram.spi_make_number-ui_ram.spi_show_number)*16,240,uicopy_showspi_start+(ui_ram.spi_make_number-ui_ram.spi_show_number+1)*16,LGRAY);
            //提示
            LCD_Fill(0,uihint_show_start,240,uihint_show_stop,WHITE);
            Show_Str(((240-strlen((char*)ui_show[ui_set.language][uigame_copy])*8)/2),uihint_show_line,(u8 *)ui_show[ui_set.language][uigame_copy],LCEDA,WHITE,16,0);
        }
    }
    //列表切换
    //显示
    if(ui_ram.now_game != ui_ram.last_game)
    {
        ui_ram.now_game=0;
        ui_ram.last_game = ui_ram.now_game;
        if(ui_adckey.now_adckey == 0x20) //下
        {
            if(ui_ram.now_nes_sdspi)
            {//SPI
                if(ui_ram.spi_make_number < (fat_ram.spi_nesnumber-1)) 
                {
                    if(++ui_ram.spi_make_number >= (uicopy_game_show+ui_ram.spi_show_number))
                    {
                        //移动一行
                        ui_ram.spi_show_number++;
                        for(i=0;i<uicopy_game_show;i++)
                        {
                            if((ui_ram.spi_show_number + i) == ui_ram.spi_make_number) 
                            {
                                Show_Str(0,uicopy_showspi_start+i*16,fat_ram.spi_sizename[ui_ram.spi_show_number+i],WHITE,LCEDA1,16,0);
                                LCD_Fill((8*strlen((char*)fat_ram.spi_sizename[ui_ram.spi_show_number+i])),uicopy_showspi_start+i*16,240,uicopy_showspi_start+(i+1)*16,LCEDA1);
                            }
                            else 
                            {
                                Show_Str(0,uicopy_showspi_start+i*16,fat_ram.spi_sizename[ui_ram.spi_show_number+i],LCEDA,WHITE,16,0);
                                LCD_Fill((8*strlen((char*)fat_ram.spi_sizename[ui_ram.spi_show_number+i])),uicopy_showspi_start+i*16,240,uicopy_showspi_start+(i+1)*16,WHITE);
                            }
                        }
                    }
                    else
                    {
                        Show_Str(0,uicopy_showspi_start+(ui_ram.spi_make_number-ui_ram.spi_show_number-1)*16,fat_ram.spi_sizename[ui_ram.spi_make_number-1],LCEDA,WHITE,16,0);
                        LCD_Fill((8*strlen((char*)fat_ram.spi_sizename[ui_ram.spi_make_number-1])),uicopy_showspi_start+(ui_ram.spi_make_number-ui_ram.spi_show_number-1)*16,240,uicopy_showspi_start+(ui_ram.spi_make_number-ui_ram.spi_show_number)*16,WHITE);
                        Show_Str(0,uicopy_showspi_start+(ui_ram.spi_make_number-ui_ram.spi_show_number)*16,fat_ram.spi_sizename[ui_ram.spi_make_number],WHITE,LCEDA1,16,0);
                        LCD_Fill((8*strlen((char*)fat_ram.spi_sizename[ui_ram.spi_make_number])),uicopy_showspi_start+(ui_ram.spi_make_number-ui_ram.spi_show_number)*16,240,uicopy_showspi_start+(ui_ram.spi_make_number-ui_ram.spi_show_number+1)*16,LCEDA1);
                    }
                }
            }
            else
            {
                if(ui_ram.sd_make_number < (fat_ram.sd_nesnumber-1)) 
                {
                    if(++ui_ram.sd_make_number >= (uicopy_game_show+ui_ram.sd_show_number))
                    {
                        //移动一行
                        ui_ram.sd_show_number++;
                        for(i=0;i<uicopy_game_show;i++)
                        {
                            if((ui_ram.sd_show_number + i) == ui_ram.sd_make_number) 
                            {
                                Show_Str(0,uicopy_showsd_start+(i)*16,fat_ram.sd_sizename[ui_ram.sd_show_number+i],WHITE,LCEDA1,16,0);
                                LCD_Fill((8*strlen((char*)fat_ram.sd_sizename[ui_ram.sd_show_number+i])),uicopy_showsd_start+i*16,240,uicopy_showsd_start+(i+1)*16,LCEDA1);
                            }
                            else 
                            {
                                Show_Str(0,uicopy_showsd_start+i*16,fat_ram.sd_sizename[ui_ram.sd_show_number+i],LCEDA,WHITE,16,0);
                                LCD_Fill((8*strlen((char*)fat_ram.sd_sizename[ui_ram.sd_show_number+i])),uicopy_showsd_start+i*16,240,uicopy_showsd_start+(i+1)*16,WHITE);
                            }
                        }
                    }
                    else
                    {
                        Show_Str(0,uicopy_showsd_start+(ui_ram.sd_make_number-ui_ram.sd_show_number-1)*16,fat_ram.sd_sizename[ui_ram.sd_make_number-1],LCEDA,WHITE,16,0);
                        LCD_Fill((8*strlen((char*)fat_ram.sd_sizename[ui_ram.sd_make_number-1])),uicopy_showsd_start+(ui_ram.sd_make_number-ui_ram.sd_show_number-1)*16,240,uicopy_showsd_start+(ui_ram.sd_make_number-ui_ram.sd_show_number)*16,WHITE);
                        Show_Str(0,uicopy_showsd_start+(ui_ram.sd_make_number-ui_ram.sd_show_number)*16,fat_ram.sd_sizename[ui_ram.sd_make_number],WHITE,LCEDA1,16,0);
                        LCD_Fill((8*strlen((char*)fat_ram.sd_sizename[ui_ram.sd_make_number])),uicopy_showsd_start+(ui_ram.sd_make_number-ui_ram.sd_show_number)*16,240,uicopy_showsd_start+(ui_ram.sd_make_number-ui_ram.sd_show_number+1)*16,LCEDA1);
                    }
                }
            }
        }
        else if(ui_adckey.now_adckey == 0x10) //上
        {
            if(ui_ram.now_nes_sdspi)
            {
                if(ui_ram.spi_make_number)
                {
                    if(--ui_ram.spi_make_number < ui_ram.spi_show_number)
                    {
                        //移动一行
                        ui_ram.spi_show_number--;
                        for(i=0;i<uicopy_game_show;i++)
                        {
                            if((ui_ram.spi_show_number + i) == ui_ram.spi_make_number) 
                            {
                                Show_Str(0,uicopy_showspi_start+(i)*16,fat_ram.spi_sizename[ui_ram.spi_show_number+i],WHITE,LCEDA1,16,0);
                                LCD_Fill((8*strlen((char*)fat_ram.spi_sizename[ui_ram.spi_show_number+i])),uicopy_showspi_start+i*16,240,uicopy_showspi_start+(i+1)*16,LCEDA1);
                            }
                            else 
                            {
                                Show_Str(0,uicopy_showspi_start+(i)*16,fat_ram.spi_sizename[ui_ram.spi_show_number+i],LCEDA,WHITE,16,0);
                                LCD_Fill((8*strlen((char*)fat_ram.spi_sizename[ui_ram.spi_show_number+i])),uicopy_showspi_start+i*16,240,uicopy_showspi_start+(i+1)*16,WHITE);
                            }
                        }
                    }
                    else
                    {
                        Show_Str(0,uicopy_showspi_start+(ui_ram.spi_make_number-ui_ram.spi_show_number+1)*16,fat_ram.spi_sizename[ui_ram.spi_make_number+1],LCEDA,WHITE,16,0);
                        LCD_Fill(8*strlen((char*)fat_ram.spi_sizename[ui_ram.spi_make_number+1]),uicopy_showspi_start+(ui_ram.spi_make_number-ui_ram.spi_show_number+1)*16,240,uicopy_showspi_start+(ui_ram.spi_make_number-ui_ram.spi_show_number+2)*16,WHITE);
                        Show_Str(0,uicopy_showspi_start+(ui_ram.spi_make_number-ui_ram.spi_show_number)*16,fat_ram.spi_sizename[ui_ram.spi_make_number],WHITE,LCEDA1,16,0);
                        LCD_Fill(8*strlen((char*)fat_ram.spi_sizename[ui_ram.spi_make_number]),uicopy_showspi_start+(ui_ram.spi_make_number-ui_ram.spi_show_number)*16,240,uicopy_showspi_start+(ui_ram.spi_make_number-ui_ram.spi_show_number+1)*16,LCEDA1);
                    }
                }
            }
            else
            {
                if(ui_ram.sd_make_number)
                {
                    if(--ui_ram.sd_make_number < ui_ram.sd_show_number)
                    {
                        //移动一行
                        ui_ram.sd_show_number--;
                        for(i=0;i<uicopy_game_show;i++)
                        {
                            if((ui_ram.sd_show_number + i) == ui_ram.sd_make_number) 
                            {
                                Show_Str(0,uicopy_showsd_start+(i)*16,fat_ram.sd_sizename[ui_ram.sd_show_number+i],WHITE,LCEDA1,16,0);
                                LCD_Fill(8*strlen((char*)fat_ram.sd_sizename[ui_ram.sd_show_number+i]),uicopy_showsd_start+i*16,240,uicopy_showsd_start+(i+1)*16,LCEDA1);
                            }
                            else 
                            {
                                Show_Str(0,uicopy_showsd_start+i*16,fat_ram.sd_sizename[ui_ram.sd_show_number+i],LCEDA,WHITE,16,0);
                                LCD_Fill(8*strlen((char*)fat_ram.sd_sizename[ui_ram.sd_show_number+i]),uicopy_showsd_start+i*16,240,uicopy_showsd_start+(i+1)*16,WHITE);
                            }
                        }
                    }
                    else
                    { 
                        Show_Str(0,uicopy_showsd_start+(ui_ram.sd_make_number-ui_ram.sd_show_number+1)*16,fat_ram.sd_sizename[ui_ram.sd_make_number+1],LCEDA,WHITE,16,0);
                        LCD_Fill(8*strlen((char*)fat_ram.sd_sizename[ui_ram.sd_make_number+1]),uicopy_showsd_start+(ui_ram.sd_make_number-ui_ram.sd_show_number+1)*16,240,uicopy_showsd_start+(ui_ram.sd_make_number-ui_ram.sd_show_number+2)*16,WHITE);
                        Show_Str(0,uicopy_showsd_start+(ui_ram.sd_make_number-ui_ram.sd_show_number)*16,fat_ram.sd_sizename[ui_ram.sd_make_number],WHITE,LCEDA1,16,0);
                        LCD_Fill(8*strlen((char*)fat_ram.sd_sizename[ui_ram.sd_make_number]),uicopy_showsd_start+(ui_ram.sd_make_number-ui_ram.sd_show_number)*16,240,uicopy_showsd_start+(ui_ram.sd_make_number-ui_ram.sd_show_number+1)*16,LCEDA1);
                    }
                }
            }
        }
        else;
    }
}
//变化spi文件后刷新
void ui_spigame_show(void)
{
    uint8_t i;
    
    //显示状态
    if(ui_ram.now_nes_sdspi)
    {//SPI
        //spi
        Show_Str(0,uicopy_showspi_init,(u8*)ui_show[ui_set.language][uiselect_spi_title],WHITE,LCEDA,16,0);
        LCD_Fill(8*strlen((char*)ui_show[ui_set.language][uiselect_spi_title]),uicopy_showspi_init,240,uicopy_showspi_init+uiszie,LCEDA); //填满
        LCD_ShowIntNum(8*11,uicopy_showspi_init,fat_ram.spi_total,5,WHITE,LCEDA,16);
        LCD_ShowIntNum(8*17,uicopy_showspi_init,fat_ram.spi_free,5,WHITE,LCEDA,16);
        LCD_ShowIntNum(8*25,uicopy_showspi_init,(fat_ram.spi_nesnumber),3,WHITE,LCEDA,16);
        //提示
        LCD_Fill(0,uihint_show_start,240,uihint_show_stop,WHITE);
        Show_Str(((240-strlen((char*)ui_show[ui_set.language][uigame_delete])*8)/2),uihint_show_line,(u8 *)ui_show[ui_set.language][uigame_delete],LCEDA,WHITE,16,0);
    }
    else
    {//SD
        //spi
        Show_Str(0,uicopy_showspi_init,(u8*)ui_show[ui_set.language][uiselect_spi_title],WHITE,LGRAY,16,0);
        LCD_Fill(8*strlen((char*)ui_show[ui_set.language][uiselect_spi_title]),uicopy_showspi_init,240,uicopy_showspi_init+uiszie,LGRAY); //填满
        LCD_ShowIntNum(8*11,uicopy_showspi_init,fat_ram.spi_total,5,WHITE,LGRAY,16);
        LCD_ShowIntNum(8*17,uicopy_showspi_init,fat_ram.spi_free,5,WHITE,LGRAY,16);
        LCD_ShowIntNum(8*25,uicopy_showspi_init,(fat_ram.spi_nesnumber),3,WHITE,LGRAY,16);
        //提示
        LCD_Fill(0,uihint_show_start,240,uihint_show_stop,WHITE);
        Show_Str(((240-strlen((char*)ui_show[ui_set.language][uigame_copy])*8)/2),uihint_show_line,(u8 *)ui_show[ui_set.language][uigame_copy],LCEDA,WHITE,16,0);
    }
    ///显示列表
    LCD_Fill(0,uicopy_showspi_start,240,uicopy_showspi_start+uicopy_game_show*16,WHITE);
    for(i=0;i<uicopy_game_show;i++)
    {
        if(i<fat_ram.spi_nesnumber)
        {
            if((ui_ram.spi_show_number + i) == ui_ram.spi_make_number) 
            {
                if(ui_ram.now_nes_sdspi)
                {
                    Show_Str(0,uicopy_showspi_start+(i)*16,fat_ram.spi_sizename[ui_ram.spi_show_number+i],WHITE,LCEDA1,16,0);
                    LCD_Fill((8*strlen((char*)fat_ram.spi_sizename[ui_ram.spi_show_number+i])),uicopy_showspi_start+i*16,240,uicopy_showspi_start+(i+1)*16,LCEDA1);
                }
                else
                {
                    Show_Str(0,uicopy_showspi_start+(i)*16,fat_ram.spi_sizename[ui_ram.spi_show_number+i],WHITE,LGRAY,16,0);
                    LCD_Fill((8*strlen((char*)fat_ram.spi_sizename[ui_ram.spi_show_number+i])),uicopy_showspi_start+i*16,240,uicopy_showspi_start+(i+1)*16,LGRAY);
                }
            }
            else 
            {
                Show_Str(0,uicopy_showspi_start+(i)*16,fat_ram.spi_sizename[ui_ram.spi_show_number+i],LCEDA,WHITE,16,0);
                LCD_Fill((8*strlen((char*)fat_ram.spi_sizename[ui_ram.spi_show_number+i])),uicopy_showspi_start+i*16,240,uicopy_showspi_start+(i+1)*16,WHITE);
            }
        }
    }
}
//设置界面全刷新显示
void ui_set_init_show(void)
{
    uint8_t i;

    //显示标题
    LCD_Fill(0,uitela_show_start,LCD_W,uitela_show_start+uiszie,LCEDA);
    Show_Str(((240-strlen((char*)ui_show[ui_set.language][uiset_title])*8)/2),uitela_show_start,(u8*)ui_show[ui_set.language][uiset_title],WHITE,LCEDA,16,0);
    //清空
    LCD_Fill(0,uiset_start,LCD_W,LCD_H,WHITE);    
    //显示列表 
    for(i=0;i<uiset_number;i++)
    {
        if(i==ui_ram.now_set) Show_Str(0,uiset_start+i*uiszie,(u8*)ui_show[ui_set.language][uiset_title+1+i],WHITE,LCEDA1,16,0);
        else Show_Str(0,uiset_start+i*uiszie,(u8*)ui_show[ui_set.language][uiset_title+1+i],LCEDA,WHITE,16,0);
    }
    //显示设置项
    Show_Str(uiset_result_x,uiset_start,(u8*)ui_show[ui_set.language][uiset_explain],LCEDA,WHITE,16,0);
    Show_Str(uiset_result_x,uiset_start+1*uiszie,(u8*)ui_show[ui_set.language][uiset_language],LCEDA,WHITE,16,0);
    Show_Str(uiset_result_x,uiset_start+2*uiszie,(u8*)ui_show[ui_set.language][uiset_class],LCEDA,WHITE,16,0);
    LCD_ShowIntNum(uiset_result_x+(uiszie>>1),uiset_start+2*uiszie,ui_set.luminance,2,LCEDA,WHITE,16);
    Show_Str(uiset_result_x,uiset_start+3*uiszie,(u8*)ui_show[ui_set.language][uiset_class],LCEDA,WHITE,16,0);
    LCD_ShowIntNum(uiset_result_x+(uiszie>>1),uiset_start+3*uiszie,ui_set.volume_class,2,LCEDA,WHITE,16);
    if(ui_set.key_volume)Show_Str(uiset_result_x,uiset_start+4*uiszie,(u8*)ui_show[ui_set.language][uiset_on],LCEDA,WHITE,16,0);
    else Show_Str(uiset_result_x,uiset_start+4*uiszie,(u8*)ui_show[ui_set.language][uiset_off],LCEDA,WHITE,16,0);
    if(ui_set.key_shake)Show_Str(uiset_result_x,uiset_start+5*uiszie,(u8*)ui_show[ui_set.language][uiset_on],LCEDA,WHITE,16,0);
    else Show_Str(uiset_result_x,uiset_start+5*uiszie,(u8*)ui_show[ui_set.language][uiset_off],LCEDA,WHITE,16,0);
    if(ui_set.show_time)Show_Str(uiset_result_x,uiset_start+6*uiszie,(u8*)ui_show[ui_set.language][uiset_on],LCEDA,WHITE,16,0);
    else Show_Str(uiset_result_x,uiset_start+6*uiszie,(u8*)ui_show[ui_set.language][uiset_off],LCEDA,WHITE,16,0);
}
//操作说明显示
void ui_set_explain_show(void)
{
    uint16_t i,temp=0;
    //清空
    LCD_Fill(0,uitela_show_start,LCD_W,LCD_H,WHITE);
    //显示说明字符串
    for(i=0;i<uikey_show_number;i++)
    {
        Show_Str(0,uitela_show_start+20*i,(u8*)ui_show[ui_set.language][uikey_explain_show+i],LCEDA,WHITE,16,0);
    }
    //等待3S 释放再按下时
    for(i=0;i<150;i++)
    {
        delay_1ms(20);
        if((get_key_val() == 0) && (temp==0)) temp=1;
        if(get_key_val() && (temp==1)) break; 
    }
    //while(get_key_val());
}
//设置界面变化显示
void ui_set_last_show(void)
{
    //列表更新
    if(ui_ram.last_set != ui_ram.now_set)
    {
        Show_Str(0,uiset_start+ui_ram.last_set*uiszie,(u8*)ui_show[ui_set.language][uiset_title+1+ui_ram.last_set],LCEDA,WHITE,16,0);
        Show_Str(0,uiset_start+ui_ram.now_set*uiszie,(u8*)ui_show[ui_set.language][uiset_title+1+ui_ram.now_set],WHITE,LCEDA1,16,0);
        ui_ram.last_set = ui_ram.now_set;
    }
    //设置更新
    if(ui_ram.last_makeset != ui_ram.now_makeset)
    {
        ui_ram.now_makeset=0;
        ui_ram.last_makeset = ui_ram.now_makeset;
        if(ui_key.now_key == 0x01)  //加
        { 
            switch(ui_ram.last_set)
            {
                case 0://操作说明
                    ui_set_explain_show();
                    //恢复//设置界面全刷新显示
                    ui_set_init_show();
                    while(get_key_val());
                    break;
                case 1://中英文
                    if(!ui_set.language)
                    {
                        ui_set.language = 0x01;
                        ui_set_init_show();
                        ui_ram.show_state =0xff;//小图标更新标志
                    }
                    break;
                case 2://屏幕亮度
                    if(ui_set.luminance < 10) 
                    {
                        ui_set.luminance++;
                        timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_1,50+ui_set.luminance*10);
                        LCD_ShowIntNum(uiset_result_x+(uiszie>>1),uiset_start+2*uiszie,ui_set.luminance,2,LCEDA,WHITE,16);
                    }
                    break;
                case 3://声音大小
                    if(ui_set.volume_class < 3) 
                    {
                        ui_set.volume_class++;
                        ui_ram.show_state =0xff;//小图标更新标志
                        keywav_init(ui_set.volume_class);
                        LCD_ShowIntNum(uiset_result_x+(uiszie>>1),uiset_start+3*uiszie,ui_set.volume_class,2,LCEDA,WHITE,16);
                    }
                    break;
                case 4://按键声音
                    if(!ui_set.key_volume)
                    {
                        ui_set.key_volume = 0x01;
                        Show_Str(uiset_result_x,uiset_start+ui_ram.last_set*uiszie,(u8*)ui_show[ui_set.language][uiset_on],LCEDA,WHITE,16,0);
                    }
                    break;
                case 5://按键震动
                    if(!ui_set.key_shake)
                    {
                        ui_set.key_shake = 0x01;
                        Show_Str(uiset_result_x,uiset_start+ui_ram.last_set*uiszie,(u8*)ui_show[ui_set.language][uiset_on],LCEDA,WHITE,16,0);
                    }
                    break;
                case 6://时间显示
                    if(!ui_set.show_time)
                    {
                        ui_set.show_time = 0x01;
                        Show_Str(uiset_result_x,uiset_start+ui_ram.last_set*uiszie,(u8*)ui_show[ui_set.language][uiset_on],LCEDA,WHITE,16,0);
                        ui_ram.show_state = 0xff; //小图标更新标志
                    }
                    break;
                default:
                    break;
            }
        }
        else
        {
            switch(ui_ram.last_set)
            {
                case 0://操作说明
                    ui_set_explain_show();
                    //恢复//设置界面全刷新显示
                    ui_set_init_show();
                    while(get_key_val());
                    break;
                case 1://中英文
                    if(ui_set.language && fat_ram.word_stock)
                    {
                        ui_set.language = 0x00;
                        ui_set_init_show();
                        ui_ram.show_state =0xff;//小图标更新标志
                    }
                    break;
                case 2://屏幕亮度
                    if(ui_set.luminance) 
                    {
                        ui_set.luminance--;
                        timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_1,50+ui_set.luminance*10);
                        LCD_ShowIntNum(uiset_result_x+(uiszie>>1),uiset_start+2*uiszie,ui_set.luminance,2,LCEDA,WHITE,16);
                    }
                    break;
                case 3://声音大小
                    if(ui_set.volume_class) 
                    {
                        ui_set.volume_class--;
                        ui_ram.show_state =0xff;//小图标更新标志
                        keywav_init(ui_set.volume_class);
                        LCD_ShowIntNum(uiset_result_x+(uiszie>>1),uiset_start+3*uiszie,ui_set.volume_class,2,LCEDA,WHITE,16);
                    }
                    break;
                case 4://按键声音
                    if(ui_set.key_volume)
                    {
                        ui_set.key_volume = 0x00;
                        Show_Str(uiset_result_x,uiset_start+ui_ram.last_set*uiszie,(u8*)ui_show[ui_set.language][uiset_off],LCEDA,WHITE,16,0);
                    }
                    break;
                case 5://按键震动
                    if(ui_set.key_shake)
                    {
                        ui_set.key_shake = 0x00;
                        Show_Str(uiset_result_x,uiset_start+ui_ram.last_set*uiszie,(u8*)ui_show[ui_set.language][uiset_off],LCEDA,WHITE,16,0);
                    }
                    break;
                case 6://时间显示
                    if(ui_set.show_time)
                    {
                        ui_set.show_time = 0x00;
                        Show_Str(uiset_result_x,uiset_start+ui_ram.last_set*uiszie,(u8*)ui_show[ui_set.language][uiset_off],LCEDA,WHITE,16,0);
                        ui_ram.show_state = 0xff; //小图标更新标志
                    }
                    break;
                default:
                    break;
            }
        }
        i2c_config();
        eeprom_buffer_write_timeout((u8 *)&ui_set,0,sizeof(ui_set));
        //小图标同步更新
        //rtc_init_show();
        rtc_last_show();
    }
    
}




////错误状态显示 等待2秒
void ui_error_show(const u8 *str)
{
    uint16_t i,temp=0;
    //清空
    LCD_Fill(0,uitela_show_start,LCD_W,LCD_H,WHITE);
    //图标
    LCD_ShowPicture1((uierror_x-(uimain_bmp_szie>>1)),(uierror_y-(uimain_bmp_szie>>1)),uimain_bmp_szie,uimain_bmp_szie,UIMAIN_BMP[error_number]);
    Show_Str(((240-(strlen((char*)ui_show[ui_set.language][uistate_eroor])*8))/2),uihint_show_line,(u8*)ui_show[ui_set.language][uistate_eroor],RED,WHITE,16,0);
    //原因
    Show_Str(((240-(strlen((char*)str)*8))/2),uierror_line,(u8 *)str,RED,WHITE,16,0);
    //等待2S 释放再按下时
    for(i=0;i<100;i++)
    {
        delay_1ms(20);
        if((get_key_val() == 0) && (temp==0)) temp=1;
        if(get_key_val() && (temp==1)) break; 
    }
   //while(get_key_val());
}






/////小图标初始化显示
void rtc_init_show(void)
{
    char rtc[10];
    //清空
    LCD_Fill(0,uistate_show_start,LCD_W,uistate_show_stop,WHITE);
    ///主界面小图标显示
    if(fat_ram.spi_flash == 0x81) LCD_ShowPicture1(uistate_flash_x,uistate_show_line,uistate_bmp_szie,uistate_bmp_szie,STATE_BMP[offflash_number]); //spi OK
    else LCD_ShowPicture1(uistate_flash_x,uistate_show_line,uistate_bmp_szie,uistate_bmp_szie,STATE_BMP[onflash_number]); //spi 
    if(fat_ram.sd_card == 0x81) LCD_ShowPicture1(uistate_sd_x,uistate_show_line,uistate_bmp_szie,uistate_bmp_szie,STATE_BMP[offcard_number]); // SD ok
    else LCD_ShowPicture1(uistate_sd_x,uistate_show_line,uistate_bmp_szie,uistate_bmp_szie,STATE_BMP[oncard_number]); // SD 
    if(ui_set.show_time) 
    {
        ///中断内显示LCD各种问题
        rtc_current_time_get(&rtc_initpara);
        sprintf(rtc,"%0.2x:%0.2x:%0.2x\0", rtc_initpara.hour, rtc_initpara.minute, rtc_initpara.second);
        LCD_ShowString(uistate_time_x,uistate_show_line,(u8*)rtc,LCEDA,WHITE,16,0);
    }
    if(ui_set.language) LCD_ShowPicture1(uistate_language_x,uistate_show_line,uistate_bmp_szie,uistate_bmp_szie,STATE_BMP[english_number]); //语言 
    else LCD_ShowPicture1(uistate_language_x,uistate_show_line,uistate_bmp_szie,uistate_bmp_szie,STATE_BMP[chinese_number]); //语言 
    LCD_ShowPicture1(uistate_battery_x,uistate_show_line,uistate_bmp_szie,uistate_bmp_szie,STATE_BMP[battery_number+ adc_bat_get_value()]);// 电量
    LCD_ShowPicture1(uistate_volume_x,uistate_show_line,uistate_bmp_szie,uistate_bmp_szie,STATE_BMP[volume_number+ ui_set.volume_class]); // 音量
}
////小图标变化显示
void rtc_last_show(void)
{
    char rtc[10];
    
    if(ui_ram.show_state)
    {
        if(ui_ram.show_state ==1)
        {
            if(ui_set.show_time) 
            {
                rtc_current_time_get(&rtc_initpara);
                sprintf(rtc,"%0.2x:%0.2x:%0.2x\0", rtc_initpara.hour, rtc_initpara.minute, rtc_initpara.second);
                LCD_ShowString(uistate_time_x,uistate_show_line,(u8*)rtc,LCEDA,WHITE,16,0);
            }
            LCD_ShowPicture1(uistate_battery_x,uistate_show_line,uistate_bmp_szie,uistate_bmp_szie,STATE_BMP[battery_number+ adc_bat_get_value()]);// 电量
        }
        else
        {
            if(ui_set.show_time) 
            {
                rtc_current_time_get(&rtc_initpara);
                sprintf(rtc,"%0.2x:%0.2x:%0.2x\0", rtc_initpara.hour, rtc_initpara.minute, rtc_initpara.second);
                LCD_ShowString(uistate_time_x,uistate_show_line,(u8*)rtc,LCEDA,WHITE,16,0);
            }
            else
            {
                LCD_ShowString(uistate_time_x,uistate_show_line,"        ",LCEDA,WHITE,16,0);
            }
            LCD_ShowPicture1(uistate_battery_x,uistate_show_line,uistate_bmp_szie,uistate_bmp_szie,STATE_BMP[battery_number+ adc_bat_get_value()]);// 电量
            LCD_ShowPicture1(uistate_volume_x,uistate_show_line,uistate_bmp_szie,uistate_bmp_szie,STATE_BMP[volume_number+ui_set.volume_class]); // 音量
            if(ui_set.language) LCD_ShowPicture1(uistate_language_x,uistate_show_line,uistate_bmp_szie,uistate_bmp_szie,STATE_BMP[english_number]); // 中英
            else LCD_ShowPicture1(uistate_language_x,uistate_show_line,uistate_bmp_szie,uistate_bmp_szie,STATE_BMP[chinese_number]); // 
        }
        ui_ram.show_state=0;
    }
}










