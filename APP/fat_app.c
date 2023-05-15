#include "fat_app.h"
#include "malloc.h"
#include "string.h"
#include "ui_app.h"
#include <stdio.h>
#ifdef _LCD_8080_
#include "lcd9841.h"
#include "TFTLCD_Init.h"
#else
#include "lcd_init.h"
#include "lcd.h"
#endif
#include "text.h"
#include "sdcard.h"
#include "w25qxx.h"
#include "flash_nes.h"


    show_fat fat_ram;
    
    
    
    FATFS *fs[_VOLUMES];//逻辑磁盘工作区.
    FILINFO *tfileinfo;	//临时文件信息
    DIR *picdir;    
    FIL *filin;
    FIL *filout;
    u32 br,bw;
    u8 pname[_MAX_LFN+1]; //带路径的文件名
    
    
//uint32_t buf_write[512];                                    /* store the data written to the card */
//uint32_t buf_read[512];                                     /* store the data read from the card */
uint8_t buf_r[512];









//申请变量
u8 exfuns_init(void)
{
    uint8_t i;
    
    //文件系统申请变量
    for(i=0;i<_VOLUMES;i++)
    {
        fs[i]=(FATFS*)mymalloc(SRAMIN,sizeof(FATFS));	//为磁盘i工作区申请内存	
        if(!fs[i])return 1;//错误
    }
    filout=(FIL*)mymalloc(SRAMIN,sizeof(FIL));		//为file申请内存
    filin=(FIL*)mymalloc(SRAMIN,sizeof(FIL));		//为file申请内存
    picdir=(DIR*)mymalloc(SRAMIN,sizeof(DIR));
    tfileinfo=(FILINFO*)mymalloc(SRAMIN,sizeof(FILINFO));
    //文件系名称申请变量  动态改为非动态 
    for(i=0;i<max_nes_number;i++)
    {
        fat_ram.sd_nesname[i]=mymalloc(SRAMIN,max_name_number);
        fat_ram.sd_sizename[i]=mymalloc(SRAMIN,max_name_number);
        fat_ram.spi_nesname[i]=mymalloc(SRAMIN,max_name_number);
        fat_ram.spi_sizename[i]=mymalloc(SRAMIN,max_name_number);
        if(!fat_ram.spi_sizename[i])return 1;//错误
    }
    return 0; 
}
//sd fatfs初始化
void sd_fatfs_init(void)
{
    uint8_t i;
    sd_error_enum sd_error;
    
    fat_ram.sd_card=0;
    fat_ram.sd_total=0;
    fat_ram.sd_free=0;
    for(i=0;i<5;i++)
    {
        delay_1ms(50);
        sd_error = sd_io_init();
        if(sd_error == SD_OK)
        {
            fat_ram.sd_card=1;
            LCD_Fill(0,uihint_show_start,240,276,WHITE);
            LCD_ShowString(((240-strlen("SD_Init OK!")*8)/2),uihint_show_start,(u8 *)"SD_Init OK!",LCEDA,WHITE,16,0);
            printf("Card init success!\r\n");
            i=f_mount(fs[0],"0:",1);
            if(!i)
            {
                fat_ram.sd_card = 0x81;
                printf("SD f_mount success!\r\n");
                exf_getfree("0:",&fat_ram.sd_total,&fat_ram.sd_free);
            }
            else printf("SD f_mount failed!\r\n");
            break;
        }
        else
        {
            printf("Card init failed!\r\n");
        }
    }
}
//spi flash fatfs+字库 初始化
void spi_fatfs_init(void)
{
    uint8_t i;
    uint8_t temp;
    
    fat_ram.spi_flash=0;
    fat_ram.word_stock=0;
    fat_ram.spi_total=0;
    fat_ram.spi_free=0;
    for(i=0;i<5;i++)
    {
        delay_1ms(50);
        temp = W25QXX_Init();
        if(temp)
        {
            fat_ram.spi_flash = 1;
            LCD_Fill(0,uihint_show_start,240,276,WHITE);
            LCD_ShowString(((240-strlen("W25QXX_Init OK!")*8)/2),uihint_show_start,(u8 *)"W25QXX_Init OK!",LCEDA,WHITE,16,0);
            printf("W25QXX_TYPE=  0x%4x \r\n", W25QXX_TYPE);
            temp=f_mount(fs[1],"1:",1); 				//挂载FLASH.	
            if(temp==0X0D)//FLASH磁盘,FAT文件系统错误,重新格式化FLASH
            {
                temp=f_mkfs("1:",1,4096);//格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
                if(temp==0) 
                {
                    fat_ram.spi_flash = 0x81;
                    printf("FLASH f_mkfs success!\r\n");
                    exf_getfree("1:",&fat_ram.spi_total,&fat_ram.spi_free);
                    //创建文件夹
                    temp=f_opendir(picdir,(const TCHAR*)"1:/nes"); 	//打开目录
                    if(temp) f_mkdir("1:/nes");
                    //复制默认三个游戏到spi falsh
                    copy_data_file("1:/nes/maoxiandao.nes",maoxiandao,sizeof(maoxiandao));
                    copy_data_file("1:/nes/MARIO.NES",mario,sizeof(mario));
                    copy_data_file("1:/nes/Battle City (J).nes",battle,sizeof(battle));
                    //统计容量
                    exf_getfree("1:",&fat_ram.spi_total,&fat_ram.spi_free);
                }
                else
                {
                     printf("FLASH f_mkfs failed!\r\n");
                }
                
            }
            else if(temp == FR_OK)
            {
                fat_ram.spi_flash = 0x81;
                //统计容量
                exf_getfree("1:",&fat_ram.spi_total,&fat_ram.spi_free);
            }
            else;
            //检查字库
            temp = font_init();
            if(temp)
            {
                printf("FLASH font_init failed!\r\n");
                //插入sd卡存在文件系统
                if(fat_ram.sd_card == 0x81)
                {
                    temp = update_font(20,uihint_show_start,16,"0:");//更新字库
                    if(temp)
                    {
                        LCD_Fill(0,uihint_show_start,240,276,WHITE);
                        LCD_ShowString(30,uihint_show_start,(u8 *)"update_font ERROR!",RED,WHITE,16,0);
                        printf("FLASH update_font failed!\r\n");
                    }
                    else
                    {
                        fat_ram.word_stock = 1;
                        LCD_Fill(0,uihint_show_start,240,276,WHITE);
                        LCD_ShowString(30,uihint_show_start,(u8 *)"update_font OK!",LCEDA,WHITE,16,0);
                        printf("FLASH update_font success!\r\n");
                    }
                }
            }
            else
            {
                fat_ram.word_stock = 1;
                LCD_Fill(0,uihint_show_start,240,276,WHITE);
                Show_Str(((240-strlen("中文字库 OK!")*8)/2),uihint_show_start,(u8 *)"中文字库 OK!",LCEDA,WHITE,16,0);
                printf("FLASH font_init success!\r\n");
            }
            break;
        }
        else
        {
            printf("error W25QXX_TYPE=  0x%4x \r\n", W25QXX_TYPE);
        }
    }
}



//得到磁盘剩余容量
u8 exf_getfree(char *drv,u32 *total,u32 *free)
{
    FATFS *fs1;
    u8 res;
    u32 fre_clust=0, fre_sect=0, tot_sect=0;
    //初始化
    *total = 0;
    *free = 0;
    //得到磁盘信息及空闲簇数量
    res =(u32)f_getfree((const TCHAR*)drv, (DWORD*)&fre_clust, &fs1);
    if(res==0)
    { 
        tot_sect=(fs1->n_fatent-2)*fs1->csize;  //得到总扇区数
        fre_sect=fre_clust*fs1->csize;          //得到空闲扇区数
    #if _MAX_SS!=512                            //扇区大小不是512字节,则转换为512字节
        tot_sect*=fs1->ssize/512;
        fre_sect*=fs1->ssize/512;
    #endif
        *total=tot_sect>>1; //单位为KB
        *free=fre_sect>>1;  //单位为KB 
    }
    return res;
} 
/////复制文件数组数组到文件系统//
void copy_data_file(const char *str,const unsigned char *nesdata,u32 number)
{
    uint8_t temp;
    uint16_t wn=0;
    
    br=0;bw=0;
    LCD_Fill(0,uihint_show_start,240,uihint_show_stop,WHITE);
    //Show_Str((0),uihint_show_line,(u8*)ui_show[setmake[1]][uicopy_file_show],LCEDA,WHITE,16,0); //提示
    Show_Str((0),uihint_show_line,(u8*)str,LCEDA,WHITE,16,0);//名称
    Show_Str(188,uihint_show_line,(u8*)"   %",LCEDA,WHITE,16,0);//名称
    temp=f_open(filin,str,FA_CREATE_ALWAYS|FA_WRITE);//创建文件
    while(temp == FR_OK)
    {
        if(br < (number-512)) wn = 512;
        else wn = number-br;
        if(wn)
        {
            temp = f_write(filin,&nesdata[br],wn,&bw);
            if(temp||bw<wn) break;
            br += wn;
            //LCD_ShowIntNum(8*14,uihint_show_line,br*100/number,3,LCEDA,WHITE,16);
            LCD_ShowIntNum(188,uihint_show_line,br*100/number,3,LCEDA,WHITE,16);
        }
        if(br == number) break;
    }
    f_close(filin);//关闭文件
}
////复制文件到文件目录//
void copy_file_file(const unsigned char *namea,const unsigned char *nameb)
{
    uint8_t temp;
    uint32_t wn=0;
    
    LCD_Fill(0,uihint_show_start,240,uihint_show_stop,WHITE);
    Show_Str((0),uihint_show_line,(u8*)ui_show[ui_set.language][uicopy_file_show],LCEDA,WHITE,16,0); //提示
    printf("fat_ram.sd_nesname %s \r\n",namea);
    printf("fat_ram.sd_nesname %s \r\n",nameb);
    temp=f_open(filout,(char*)namea,FA_READ);//打开文件
    if(temp != FR_OK) return;
    temp=f_open(filin,(char*)nameb,FA_CREATE_ALWAYS|FA_WRITE);//打开文件
    if(temp != FR_OK) return;
    while(temp == FR_OK)
    {
        temp = f_read(filout,buf_r,512,&br);
        if(temp||br==0) break;
        temp = f_write(filin,buf_r,br,&bw);
        if(temp||bw<br) break;
        printf(".");
        wn += br;
        LCD_ShowIntNum(8*14,uihint_show_line,wn*100/filout->obj.objsize,3,LCEDA,WHITE,16);
    }
    printf("\r\n FA_WRITE OVER！  \r\n");
    f_close(filout);//关闭文件
    f_close(filin);//关闭文件
}
//指定检索sd卡游戏 带字节数
uint8_t sd_file_search(void)
{
    uint8_t temp;
    uint8_t len;
    
    temp=f_opendir(picdir,(const TCHAR*)"0:/nes"); 	//打开目录
    if(temp != FR_OK) 
    {
        fat_ram.sd_nesnumber = 0;
        fat_ram.sd_nesname[0] = (uint8_t *)ui_show[ui_set.language][uigame_no_found];//提示未找到
        fat_ram.sd_sizename[0] = (uint8_t *)ui_show[ui_set.language][uigame_no_found];//提示未找到
        return temp;
    }
    fat_ram.sd_nesnumber=0;
    while(temp==FR_OK)
    {
        temp=f_readdir(picdir,tfileinfo);
        if(temp!=FR_OK||tfileinfo->fname[0]==0)break;
        strcpy((char*)&pname,"0:/nes/");
        strcat((char*)&pname,(const char*)tfileinfo->fname);//将文件名接在后面
        len=strlen((const char*)tfileinfo->fname);
        if(tfileinfo->fname[len-4] == '.' && len < (max_name_number-8))
        {
            if((tfileinfo->fname[len-3] == 'n') || (tfileinfo->fname[len-3] == 'N'))
            {
                if((tfileinfo->fname[len-2] == 'e') || (tfileinfo->fname[len-2] == 'E'))
                {
                    if((tfileinfo->fname[len-1] == 's') || (tfileinfo->fname[len-1] == 'S'))
                    {
                        //带路径记录
                        strcpy((char*)fat_ram.sd_nesname[fat_ram.sd_nesnumber],(char*)pname);
                        //带容量路径记录
                        if((tfileinfo->fsize >> 10) < 4096) sprintf((char*)pname,"%4dKB ",(unsigned int)(tfileinfo->fsize >> 10)+1);
                        else sprintf((char*)pname,"%4dMB ",(unsigned int)(tfileinfo->fsize >> 20));
                        strcat((char*)&pname,(const char*)tfileinfo->fname);//将文件名接在后面
                        strcpy((char*)fat_ram.sd_sizename[fat_ram.sd_nesnumber],(char*)pname);
                        if(++fat_ram.sd_nesnumber >= max_nes_number) break;
                    }
                }
            }
        }
    }
    //没找到文件
    if(fat_ram.sd_nesnumber == 0) 
    {
        fat_ram.sd_nesname[0] = (uint8_t *)ui_show[ui_set.language][uigame_no_found];//提示未找到
        fat_ram.sd_sizename[0] = (uint8_t *)ui_show[ui_set.language][uigame_no_found];//提示未找到
    }
    return 0;
}

//指定检索SPI游戏 带字节数
uint8_t spiflash_file_search(void)
{
    uint8_t temp;
    uint8_t len;
    
    //检索SPI游戏
    temp=f_opendir(picdir,(const TCHAR*)"1:/nes"); 	//打开目录
    if(temp != FR_OK) 
    {
        fat_ram.spi_nesnumber=0;
        fat_ram.spi_nesname[0] = (uint8_t *)ui_show[ui_set.language][uigame_no_found];//提示未找到
        fat_ram.spi_sizename[0] = (uint8_t *)ui_show[ui_set.language][uigame_no_found];//提示未找到
        return temp;
    }
    fat_ram.spi_nesnumber=0;
    while(temp==FR_OK)
    {
        temp=f_readdir(picdir,tfileinfo);
        if(temp!=FR_OK||tfileinfo->fname[0]==0)break;
        strcpy((char*)&pname,"1:/nes/");
        strcat((char*)&pname,(const char*)tfileinfo->fname);//将文件名接在后面
        len=strlen((const char*)tfileinfo->fname);
        if(tfileinfo->fname[len-4] == '.' && len < (max_name_number-8))
        {
            if((tfileinfo->fname[len-3] == 'n') || (tfileinfo->fname[len-3] == 'N'))
            {
                if((tfileinfo->fname[len-2] == 'e') || (tfileinfo->fname[len-2] == 'E'))
                {
                    if((tfileinfo->fname[len-1] == 's') || (tfileinfo->fname[len-1] == 'S'))
                    {
                        //开始记录
                        strcpy((char*)fat_ram.spi_nesname[fat_ram.spi_nesnumber],(char*)pname);
                        //带容量路径记录
                        if((tfileinfo->fsize >> 10) < 4096) sprintf((char*)pname,"%4dKB ",(unsigned int)(tfileinfo->fsize >> 10)+1);
                        else sprintf((char*)pname,"%4dMB ",(unsigned int)(tfileinfo->fsize >> 20));
                        strcat((char*)&pname,(const char*)tfileinfo->fname);//将文件名接在后面
                        strcpy((char*)fat_ram.spi_sizename[fat_ram.spi_nesnumber],(char*)pname);
                        if(++fat_ram.spi_nesnumber >= max_nes_number) break;
                    }
                }
            }
        }
    }
    //没找到文件
    if(fat_ram.spi_nesnumber == 0) 
    {
        fat_ram.spi_nesname[0] = (uint8_t *)ui_show[ui_set.language][uigame_no_found];//提示未找到
        fat_ram.spi_sizename[0] = (uint8_t *)ui_show[ui_set.language][uigame_no_found];//提示未找到
    }
    return 0;
}















