#ifndef _UI_APP_H
#define _UI_APP_H

#include "gd32f4xx.h"
#include "systick.h"

//��ʾ���嶨�� 
#define uiszie  16

///��ʾλ�ö���
#define uiline_start            0       //��
#define uiline_stop             240
#define uistate_show_start      0       //״̬
#define uistate_show_line       2       //״̬��ʼλ��
#define uistate_show_stop       20
#define uitela_show_start       20      //������
#define uitela_show_stop        260
#define uihint_show_start       260     //��ʾ����
#define uihint_show_stop        280
#define uihint_show_line        262     //��ʾ������
#define uiselect_show_start     36      //��Ϸ��ʼ
#define uiselect_game_show      14      //��ʾ��Ϸ��
#define uierror_line            200     //������ʾ
#define uicopy_showsd_init      20     //sd��Ϸ��ʼ
#define uicopy_showsd_start     36     //
#define uicopy_showspi_init     140     //spi��Ϸ��ʼ
#define uicopy_showspi_start    156     //spi��Ϸ��ʼ
#define uicopy_game_show        6      //��ʾ��Ϸ��
#define uiset_start             36     //
#define uiset_number            7      //������

//���������λ��
#define uimain_bmp_szie     90  //90*90
#define uimain_bmp_paly_x   60  //main ����
#define uimain_bmp_paly_y   80
#define uimain_bmp_last_x   180  //main ����
#define uimain_bmp_last_y   80
#define uimain_bmp_copy_x   60  //main ����
#define uimain_bmp_copy_y   200
#define uimain_bmp_set_x    180  //main ����
#define uimain_bmp_set_y    200
#define uiselect_bmp_sd_x   60   //ѡ�� sd
#define uiselect_bmp_sd_y   140  
#define uiselect_bmp_spi_x  180  //ѡ�� spi
#define uiselect_bmp_spi_y  140  
#define uierror_x           120  //������ʾ
#define uierror_y           140
#define uiset_result_x      170  //���ý��

//С������ʾλ�� Yλ��ȷ�� uistate_show_line
#define uistate_bmp_szie    16  //16*16
#define uistate_flash_x     32
#define uistate_sd_x        48
#define uistate_language_x  64
#define uistate_time_x      88
#define uistate_eeror_x     160
#define uistate_battery_x   176
#define uistate_volume_x    196

///������ʾ�ַ�����λ��
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


///���幦�ܱ���
#define game_start  0x00    //��ʼ��Ϸ
#define game_last   0x01    //�ϴ���Ϸ
#define game_manage 0x10    //������Ϸ
#define game_set    0x11    //��Ϸ����



//ui��ʾ�ṹ��   now/last  ״̬�����ظ�������
typedef struct  
{
    uint8_t     language;           //����
    uint8_t     last_app;           //�ϴ�ѡ��4����
    uint8_t     now_app;            //��ǰѡ��4����
    uint8_t     last_sdspi;         //�ϴ�ѡ��SD����SPI
    uint8_t     now_sdspi;          //��ǰѡ��SD����SPI
    uint8_t     sd_show_number;     //��ʾλ��
    uint8_t     sd_make_number;     //ѡ����Ϸ
    uint8_t     spi_show_number;    //��ʾλ��
    uint8_t     spi_make_number;    //ѡ����Ϸ
    uint8_t     last_game;          //�ϴ�ѡ����Ϸ
    uint8_t     now_game;           //��ǰѡ����Ϸ
    uint8_t     last_nes_sdspi;     //�ϴ�ѡ��SD����SPI��Ϸ
    uint8_t     now_nes_sdspi;      //��ǰѡ��SD����SPI��Ϸ
    uint8_t     last_set;           //�ϴ�����
    uint8_t     now_set;            //��ǰ����
    uint8_t     last_makeset;       //�ϴ�����
    uint8_t     now_makeset;        //��ǰ����
    uint8_t     show_state;         //Сͼ��
    
}show_ui;
extern show_ui ui_ram;
extern const char *ui_show[2][41];


//���ýṹ��
typedef struct  
{
    uint8_t     language;           //����
    uint8_t     luminance;          //����
    uint8_t     volume_class;       //����
    uint8_t     key_volume;         //����
    uint8_t     key_shake;          //��
    uint8_t     show_time;          //ʱ��
}set_ui;
extern set_ui  ui_set;



void ui_main_set_init(void);//������������
void ui_main_lcd_init(void);//����logo��ʾ

void ui_main_init_show(void);       //��ʼ����ʾ������
void ui_main_last_show(void);       //�����б仯��ʾ
void ui_select_init_show(void);     //��ʼ��sd spi ѡ�����
void ui_select_last_show(void);     //ѡ��仯����
void ui_selectgame_init_show(void); //��ʼ����Ϸѡ�����
void ui_selectgame_last_show(void); //ѡ����Ϸ�仯����
void ui_copygame_init_show(void);   //����ɾ����Ϸ�ļ� ��ʼ����ʾ
void ui_copygame_last_show(void);   //����ɾ����Ϸ�ļ� �л���ʾ
void ui_spigame_show(void);         //�仯spi�ļ���ˢ��
void ui_set_init_show(void);        //���ý���ȫˢ����ʾ
void ui_set_last_show(void);        //���ý���仯��ʾ
void ui_set_explain_show(void);     //����˵����ʾ
void ui_error_show(const u8 *str);  //����״̬��ʾ �ȴ�2��
void rtc_init_show(void);           //Сͼ���ʼ����ʾ
void rtc_last_show(void);           //Сͼ��仯��ʾ



#endif

