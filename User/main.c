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

    
    
    
void game_start_main(void);///��ʼ��Ϸ
void game_manage_main(void);///������Ϸ
void game_last_main(void);///�ϴ���Ϸ
void game_set_main(void);//��Ϸ����


void key_wav_motor(void);//����������
void TIMER3_IRQHandler(void);/////���������������ж�
void RTC_WKUP_IRQHandler(void);/////1S�ж��ڴ���
void HardFault_Handler(void);////Ӳ��������

/*!
	\brief    main function
	\param[in]  none
	\param[out] none
	\retval     none
*/
int main(void)
{
    //��ʼ��
    systick_config();
    usart0_init();
    adc_config();
    adc_bat_init();
    key_init();
    led_init();
    ui_main_set_init();////������������
    ui_main_lcd_init();//����logo��ʾ
    keywav_init(ui_set.volume_class);
    Dac_Init();
    motro_gpio_config();
    key_timeinit(600*50);    //������Ƶ��
    rtctime_init();
    //sram��ʼ��
    exmc_synchronous_dynamic_ram_init(EXMC_SDRAM_DEVICE0);
    my_mem_init(SRAMIN);
    my_mem_init(SRAMEX);
    my_mem_init(SRAMCCM);
    //�ļ�ϵͳ
    exfuns_init();  //Ϊfatfs��ر��������ڴ�    
    sd_fatfs_init();//sd fatfs��ʼ��
    spi_fatfs_init();//spi flash fatfs+�ֿ� ��ʼ��
    //���ֿ���ʾӢ��
    if(ui_ram.language==0 && fat_ram.word_stock==0) ui_ram.language=1;
    ///������Сͼ����ʾ
    rtc_init_show();
    ///�������ͼ��ȫ����ʾ
    ui_main_init_show();
    
    while(1)
    {
        //������ѡ��
        ui_adckey.now_adckey = five_way_key_scan();
        if( ui_adckey.last_adckey != ui_adckey.now_adckey)
        {
            ui_adckey.last_adckey = ui_adckey.now_adckey;
            if(ui_adckey.last_adckey == adckey_null) continue;
            key_wav_motor();//����������
            //ѭ���л�
            //if(ui_ram.now_app & 0xf0) ui_ram.now_app &= 0x0f;
            //else ui_ram.now_app |= 0x10;
            //if(ui_ram.now_app & 0x0f) ui_ram.now_app &= 0xf0;
            //else ui_ram.now_app |= 0x01;
            //����ѭ���л� ����ͬʱѡ��
            if(ui_adckey.last_adckey & adckey_up) ui_ram.now_app &= 0x0f;
            else if(ui_adckey.last_adckey & adckey_down) ui_ram.now_app |= 0x10;
            else;
            if(ui_adckey.last_adckey & adckey_left) ui_ram.now_app &= 0xf0;
            else if(ui_adckey.last_adckey & adckey_right) ui_ram.now_app |= 0x01;
            else;
        }
        //ѡ��仯��ʾ
        ui_main_last_show();
        ///ȷ�����빦��
        ui_key.now_key = get_key_val();
        if(ui_key.last_key != ui_key.now_key)
        {
            ui_key.last_key = ui_key.now_key;
            if(ui_key.last_key == key_null) continue;
            if(ui_key.last_key & key_exit) continue;
            key_wav_motor();//����������
            //���빦��
            switch(ui_ram.last_app)
            {
                case game_start:
                    game_start_main();//��ʼ��Ϸ
                    break;
                case game_last:
                    game_last_main();//�ϴ���Ϸ
                    break;
                case game_manage:
                    game_manage_main();//������Ϸ
                    break;
                case game_set:
                    game_set_main();//��Ϸ����
                    break;
                default:
                    break;
            }
        }
        //Сͼ����ʾ
        rtc_last_show();
        delay_1ms(10);
    }
}




///��ʼ��Ϸ
void game_start_main(void)
{
    u8 res;
    
    ///ѡ������ͼ��ȫ����ʾ
    ui_select_init_show();
    while(1)
    {
        //ѡ��
        ui_adckey.now_adckey = five_way_key_scan();
        if( ui_adckey.last_adckey != ui_adckey.now_adckey)
        {
            ui_adckey.last_adckey = ui_adckey.now_adckey;
            if(ui_adckey.last_adckey == adckey_null) continue;
            if(ui_adckey.last_adckey & adckey_up_down) continue;
            key_wav_motor();
            if(ui_adckey.last_adckey & adckey_left_right)
            {
                //����ѭ��
                if(ui_adckey.last_adckey & adckey_left) ui_ram.now_sdspi = 0;
                else if(ui_adckey.last_adckey & adckey_right) ui_ram.now_sdspi = 1;
                else;
            }
        }
        //ѡ��仯����
        ui_select_last_show();
        //������Ϸ
        ui_key.now_key = get_key_val();
        if( ui_key.last_key != ui_key.now_key)
        {
            ui_key.last_key = ui_key.now_key;
            if(ui_key.last_key == key_null) continue;
            key_wav_motor();
            if(ui_key.last_key & key_exit) 
            {
                ///�������ͼ��ȫ����ʾ
                ui_main_init_show();
                //while(get_key_val());
                break;
            }
            ///Ŀ¼��Ϸ��  maxnes  fat_ram.sd_nesname
            if(ui_ram.now_sdspi) res = spiflash_file_search();
            else res = sd_file_search();
            if(res)
            {
                //������ʾ 
                ui_error_show((u8 *)ui_show[ui_ram.language][uierror_dir_show]);
                ///ѡ������ͼ��ȫ����ʾ
                ui_select_init_show();
                while(get_key_val());
                continue;
            }
            //��ʼ������
            ui_ram.sd_show_number=0; //��ʾλ��
            ui_ram.spi_show_number=0; //��ʾλ��
            ui_ram.sd_make_number=0;//ѡ����Ϸ
            ui_ram.spi_make_number=0;//ѡ����Ϸ
            ///��ʼ����Ϸѡ�����
            ui_selectgame_init_show();
            //ѡ����Ϸ
            while(1)
            {
                //ҡ�˷���
                ui_adckey.now_adckey = five_way_key_scan();
                if( ui_adckey.last_adckey != ui_adckey.now_adckey)
                {
                    ui_adckey.last_adckey = ui_adckey.now_adckey;
                    if(ui_adckey.last_adckey == adckey_null) continue;
                    if(ui_adckey.last_adckey & adckey_left_right) continue;
                    key_wav_motor();
                    if(ui_adckey.last_adckey & adckey_up_down)
                    {
                        ui_ram.now_game=0xff; //�仯
                    }
                }
                ///ѡ����Ϸ�仯����
                ui_selectgame_last_show();
                //������Ϸ
                ui_key.now_key = get_key_val();
                if( ui_key.last_key != ui_key.now_key)
                {
                    ui_key.last_key = ui_key.now_key;
                    if(ui_key.last_key == key_null) continue;
                    key_wav_motor();
                    if(ui_key.last_key & key_exit)
                    {
                        ///ѡ������ͼ��ȫ����ʾ
                        ui_select_init_show();
                        //while(get_key_val());
                        break;
                    }
                    if(ui_key.last_key & key_enter) 
                    {
                        //������ʷ��Ϸ��¼
                        printf("//////////\r\n");
                        res = f_open(filin,"1:/LastGame.txt",FA_CREATE_ALWAYS|FA_WRITE);//���ļ� ��¼
                        if(res)
                        {
                            printf("1:/LastGame.txt FA_WRITE OFF!  %d \r\n",res);
                            LCD_Fill(0,uihint_show_start,240,280,WHITE);
                            Show_Str(((240-strlen("[��¼ʧ��]")*8)/2),uihint_show_line,"[��¼ʧ��]",LCEDA,WHITE,16,0);
                        }
                        if(ui_ram.now_sdspi) strcpy((char*)&buf_r,(char*)fat_ram.spi_nesname[ui_ram.spi_make_number]);
                        else strcpy((char*)&buf_r,(char*)fat_ram.sd_nesname[ui_ram.sd_make_number]);
                        f_write(filin,buf_r,512,&bw);
                        f_close(filin);//�ر��ļ�
                        
                        
                        /////��ʼ����Ϸ
                        LCD_Fill(0,uihint_show_start,240,280,WHITE);
                        Show_Str(0,uihint_show_line,buf_r,LCEDA,WHITE,16,0);
                        res = f_open(filout,(char *)buf_r,FA_READ);//���ļ�
                        if(res == FR_OK)
                        {
                            printf("nes_load start\r\n");
                            nes_load1(buf_r);
                            printf("nes_load end\r\n");
                        }
                        else
                        {
                            //������ʾ
                            printf("//ui_error_show:  %s  error: %d \r\n",buf_r , res);
                            ui_error_show((u8 *)ui_show[ui_ram.language][uierror_file_show]);
                            while(get_key_val());
                        }
                        ///��ʼ����Ϸѡ�����
                        ui_selectgame_init_show();
                        while(get_key_val());
                    }
                }
                //Сͼ����ʾ
                rtc_last_show();
                delay_1ms(10);
            }
        }
        //Сͼ����ʾ
        rtc_last_show();
        delay_1ms(10);
    }
}


///�ϴ���Ϸ
void game_last_main(void)
{
    u8 res;
    
    //���ļ�
    res = f_open(filout,"1:/LastGame.txt",FA_READ);//���ļ�
    if(res)
    {
        printf("1:/LastGame.txt FA_READ OFF!  %d \r\n",res);
        //������ʾ
        ui_error_show((u8 *)ui_show[ui_ram.language][uierror_file_show]);
        ///�������ͼ��ȫ����ʾ
        ui_main_init_show();
        while(get_key_val()); 
    }
    else 
    {
        //��ȡ�ļ�
        f_read(filout,buf_r,512,&br);
        LCD_Fill(0,uihint_show_start,240,280,WHITE);
        Show_Str(0,uihint_show_line,buf_r,LCEDA,WHITE,16,0);
        printf("f_read  %s \r\n",buf_r);
        //��ʼ��Ϸ
        res = f_open(filout,(char *)buf_r,FA_READ);//���ļ�
        if(res == FR_OK)
        {
            printf("nes_load start\r\n");
            nes_load1(buf_r);
            printf("nes_load end\r\n");
        }
        else
        {
            //������ʾ
            ui_error_show((u8 *)ui_show[ui_ram.language][uierror_file_show]);
            while(get_key_val());
        }
        ///�������ͼ��ȫ����ʾ
        ui_main_init_show();
        //while(get_key_val());
    }
    f_close(filout);//�ر��ļ�
}
///������Ϸ
void game_manage_main(void)
{
    u8 res;
    
    //����SPI flash �ļ�ϵͳ����
    if((fat_ram.spi_flash == 0x81))
    {
        //����SD����Ϸ
        sd_file_search();
        //����SPI��Ϸ
        spiflash_file_search();
        //��ʼ������
        ui_ram.sd_show_number=0; //��ʾλ��
        ui_ram.spi_show_number=0; //��ʾλ��
        ui_ram.sd_make_number=0;//ѡ����Ϸ
        ui_ram.spi_make_number=0;//ѡ����Ϸ
        ui_ram.last_nes_sdspi=0;//ѡ��
        ui_ram.now_nes_sdspi=0;//ѡ��
        //����ɾ����Ϸ�ļ� ��ʼ����ʾ
        ui_copygame_init_show();
        while(1)
        {
            //�������� ѡ���ļ����л�����
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
                else if(ui_adckey.last_adckey == adckey_left) //��
                {
                    if(ui_ram.now_nes_sdspi) ui_ram.now_nes_sdspi=0;
                }
                else if(ui_adckey.last_adckey == adckey_right) //��
                {
                    if(ui_ram.now_nes_sdspi == 0) ui_ram.now_nes_sdspi=0x01;
                }
                 else;
            }
            //����ɾ����Ϸ�ļ� �л���ʾ 
            ui_copygame_last_show();
            //������Ϸ
            ui_key.now_key = get_key_val();
            if( ui_key.last_key != ui_key.now_key)
            {
                ui_key.last_key = ui_key.now_key;
                if(ui_key.last_key == key_null) continue;
                key_wav_motor();
                if(ui_key.last_key & key_exit) //����
                {
                    ///�������ͼ��ȫ����ʾ
                    ui_main_init_show();
                    //while(get_key_val());
                    break;
                }
                if(ui_key.last_key & key_enter)
                {
                    if(ui_ram.now_nes_sdspi)
                    {//ɾ���ļ�
                        if(fat_ram.spi_flash == 0x81)
                        {
                            f_unlink((const char*)fat_ram.spi_nesname[ui_ram.spi_make_number]);
                            while(exf_getfree("1:",&fat_ram.spi_total,&fat_ram.spi_free))delay_1ms(10);
                            //����SPI��Ϸ
                            spiflash_file_search();
                            //λ�ù���
                            //ui_ram.spi_show_number=0;
                            //ui_ram.spi_make_number=0;
                            //λ�ü̳�
                            if(ui_ram.spi_show_number) ui_ram.spi_show_number--;
                            if(ui_ram.spi_make_number) ui_ram.spi_make_number--;
                            //�仯spi�ļ���ˢ��
                            ui_spigame_show();
                        }
                    }
                    else
                    {//�����ļ�
                        if((fat_ram.sd_card == 0x81) ||  (fat_ram.spi_flash == 0x81))
                        {
                            //�ļ�����
                            strcpy((char*)&pname,"1:/nes/");
                            strcat((char*)&pname,(const char*)fat_ram.sd_nesname[ui_ram.sd_make_number]+7);//���ļ������ں���
                            res=f_open(filout,(char*)pname,FA_READ);//���ļ�
                            if(res) 
                            {
                                //�����ļ�
                                copy_file_file(fat_ram.sd_nesname[ui_ram.sd_make_number],pname);
                                ///////�б����
                                while(exf_getfree("1:",&fat_ram.spi_total,&fat_ram.spi_free))delay_1ms(10);
                                //����SPI��Ϸ
                                spiflash_file_search();
                                //λ�ù���
                                //ui_ram.spi_show_number=0;
                                //ui_ram.spi_make_number=0;
                                //λ�ü̳�
                                //�仯spi�ļ���ˢ��
                                ui_spigame_show();
                            }
                        }
                    }
                }
                else;
            }
            //Сͼ����ʾ
            rtc_last_show();
            delay_1ms(10);
        }
    }
    else
    {
        //������ʾ
        ui_error_show((u8 *)ui_show[ui_ram.language][uierror_file_show]);
        ///�������ͼ��ȫ����ʾ
        ui_main_init_show();
        while(get_key_val());
    }
}
//��Ϸ����
void game_set_main(void)
{
    //������ʼ��
    ui_ram.now_set=0;
    ui_ram.last_set=0;
    //���ý���ȫˢ����ʾ
    ui_set_init_show();
    while(1)
    {
        //������ѡ��
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
        ///ȷ�����빦��
        ui_key.now_key = get_key_val();
        if(ui_key.last_key != ui_key.now_key)
        {
            ui_key.last_key = ui_key.now_key;
            if(ui_key.last_key == key_null) continue;
            key_wav_motor();
            if(ui_key.last_key & key_exit) 
            {
                ///�������ͼ��ȫ����ʾ
                ui_main_init_show();
                //while(get_key_val());
                break;
            }
            else
            {
                ui_ram.now_makeset=0xff;//�仯��־
            }
        }
        ///�仯��ʾ
        ui_set_last_show();
        //Сͼ����ʾ
        rtc_last_show();
        delay_1ms(10);
    }
}
//����������
void key_wav_motor(void)
{
    if(ui_set.key_volume || ui_set.key_shake)
    {
        if(ui_set.key_shake) motor_on();
        timer_enable(TIMER3);
    }
}


/////���������������ж�
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
/////�ж��ڴ���
void RTC_WKUP_IRQHandler(void)
{
    if(RESET != rtc_flag_get(RTC_FLAG_WT))
    {
        exti_flag_clear(EXTI_22);
        rtc_flag_clear(RTC_FLAG_WT);
        ui_ram.show_state |= 1; //Сͼ����±�־
        
        LED2_TOGGLE;
    } 
}
////Ӳ��������
void HardFault_Handler(void)
{
    uint32_t temp;
    temp = SCB->CFSR;			  // fault״̬�Ĵ���(@0XE000ED28)����:MMSR,BFSR,UFSR
    printf("CFSR:%8X\r\n", temp); //��ʾ����ֵ
    temp = SCB->HFSR;			  //Ӳ��fault״̬�Ĵ���
    printf("HFSR:%8X\r\n", temp); //��ʾ����ֵ
    temp = SCB->DFSR;			  //����fault״̬�Ĵ���
    printf("DFSR:%8X\r\n", temp); //��ʾ����ֵ
    temp = SCB->AFSR;			  //����fault״̬�Ĵ���
    printf("AFSR:%8X\r\n", temp); //��ʾ����ֵ

    NVIC_SystemReset();
}



