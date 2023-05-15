#ifndef _UI_APP_H
#define _UI_APP_H

#include "gd32f4xx.h"
#include "systick.h"

//显示字体定义 
#define uiszie  16

///显示位置定义
#define uiline_start            0       //行
#define uiline_stop             240
#define uistate_show_start      0       //状态
#define uistate_show_line       2       //状态开始位置
#define uistate_show_stop       20
#define uitela_show_start       20      //主界面
#define uitela_show_stop        260
#define uihint_show_start       260     //提示界面
#define uihint_show_stop        280
#define uihint_show_line        262     //提示栏字体
#define uiselect_show_start     36      //游戏开始
#define uiselect_game_show      14      //显示游戏数
#define uierror_line            200     //错误提示
#define uicopy_showsd_init      20     //sd游戏开始
#define uicopy_showsd_start     36     //
#define uicopy_showspi_init     140     //spi游戏开始
#define uicopy_showspi_start    156     //spi游戏开始
#define uicopy_game_show        6      //显示游戏数
#define uiset_start             36     //
#define uiset_number            7      //设置数

//主界面相关位置
#define uimain_bmp_szie     90  //90*90
#define uimain_bmp_paly_x   60  //main 中心
#define uimain_bmp_paly_y   80
#define uimain_bmp_last_x   180  //main 中心
#define uimain_bmp_last_y   80
#define uimain_bmp_copy_x   60  //main 中心
#define uimain_bmp_copy_y   200
#define uimain_bmp_set_x    180  //main 中心
#define uimain_bmp_set_y    200
#define uiselect_bmp_sd_x   60   //选择 sd
#define uiselect_bmp_sd_y   140  
#define uiselect_bmp_spi_x  180  //选择 spi
#define uiselect_bmp_spi_y  140  
#define uierror_x           120  //错误提示
#define uierror_y           140
#define uiset_result_x      170  //设置结果

//小标题显示位置 Y位置确定 uistate_show_line
#define uistate_bmp_szie    16  //16*16
#define uistate_flash_x     32
#define uistate_sd_x        48
#define uistate_language_x  64
#define uistate_time_x      88
#define uistate_eeror_x     160
#define uistate_battery_x   176
#define uistate_volume_x    196

///各种显示字符串的位置
#define uimain_playh_show   0
#define uimain_last_show    1
#define uimain_copy_show    2
#define uimain_set_show     3
#define uiselect_sd_save    4
#define uiselect_spi_save   5
#define uiselect_sd_game    6
#define uiselect_spi_game   7
#define uiselect_sd_title   8
#define uiselect_spi_title  9
#define uigame_copy         10
#define uigame_delete       11
#define uigame_no_found     12
#define uistate_eroor       13
#define uiset_title         14
#define uiset_explain       22
#define uiset_language      23
#define uiset_class         24
#define uiset_on            25
#define uiset_off           26
#define uikey_explain_show  27
#define uikey_show_number   11
#define uierror_dir_show    38
#define uierror_file_show   39
#define uicopy_file_show    40


///定义功能变量
#define game_start  0x00    //开始游戏
#define game_last   0x01    //上次游戏
#define game_manage 0x10    //管理游戏
#define game_set    0x11    //游戏设置



//ui显示结构体   now/last  状态避免重复进函数
typedef struct  
{
    uint8_t     language;           //语言
    uint8_t     last_app;           //上次选中4大功能
    uint8_t     now_app;            //当前选中4大功能
    uint8_t     last_sdspi;         //上次选中SD或者SPI
    uint8_t     now_sdspi;          //当前选中SD或者SPI
    uint8_t     sd_show_number;     //显示位置
    uint8_t     sd_make_number;     //选择游戏
    uint8_t     spi_show_number;    //显示位置
    uint8_t     spi_make_number;    //选择游戏
    uint8_t     last_game;          //上次选中游戏
    uint8_t     now_game;           //当前选中游戏
    uint8_t     last_nes_sdspi;     //上次选中SD或者SPI游戏
    uint8_t     now_nes_sdspi;      //当前选中SD或者SPI游戏
    uint8_t     last_set;           //上次设置
    uint8_t     now_set;            //当前设置
    uint8_t     last_makeset;       //上次设置
    uint8_t     now_makeset;        //当前设置
    uint8_t     show_state;         //小图标
    
}show_ui;
extern show_ui ui_ram;
extern const char *ui_show[2][41];


//设置结构体
typedef struct  
{
    uint8_t     language;           //语言
    uint8_t     luminance;          //亮度
    uint8_t     volume_class;       //音量
    uint8_t     key_volume;         //声音
    uint8_t     key_shake;          //震动
    uint8_t     show_time;          //时间
}set_ui;
extern set_ui  ui_set;



void ui_main_set_init(void);//开机掉电设置
void ui_main_lcd_init(void);//开机logo显示

void ui_main_init_show(void);       //初始化显示主界面
void ui_main_last_show(void);       //操作有变化显示
void ui_select_init_show(void);     //初始化sd spi 选择界面
void ui_select_last_show(void);     //选择变化界面
void ui_selectgame_init_show(void); //初始化游戏选择界面
void ui_selectgame_last_show(void); //选择游戏变化界面
void ui_copygame_init_show(void);   //复制删除游戏文件 初始化显示
void ui_copygame_last_show(void);   //复制删除游戏文件 切换显示
void ui_spigame_show(void);         //变化spi文件后刷新
void ui_set_init_show(void);        //设置界面全刷新显示
void ui_set_last_show(void);        //设置界面变化显示
void ui_set_explain_show(void);     //操作说明显示
void ui_error_show(const u8 *str);  //错误状态显示 等待2秒
void rtc_init_show(void);           //小图标初始化显示
void rtc_last_show(void);           //小图标变化显示



#endif

