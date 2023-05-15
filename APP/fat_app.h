#ifndef _FAT_APP_H
#define _FAT_APP_H

#include "gd32f4xx.h"
#include "systick.h"
#include "ff.h"

////exfuns_init();							//为fatfs相关变量申请内存

#define max_nes_number  100 //最多显示nes游戏数量
#define max_name_number 64  //名字长度
//nes文件列表结构体
typedef struct  
{
    uint8_t sd_card;                        //sd卡
    u32     sd_total;                       //总容量
    u32     sd_free;                        //剩余容量
    uint8_t *sd_nesname[max_nes_number];    ///指向nes游戏名的指针 动态申请大小
    uint8_t *sd_sizename[max_nes_number];   //带容量显示游戏名指针
    uint8_t sd_nesnumber;                   //数量
    uint8_t spi_flash;                      //spi flash
    uint8_t word_stock;                     //字库
    u32     spi_total;                      //总容量
    u32     spi_free;                       //剩余容量
    uint8_t *spi_nesname[max_nes_number];   ///指向nes游戏名的指针 动态申请大小
    uint8_t *spi_sizename[max_nes_number];  //带容量显示游戏名指针
    uint8_t spi_nesnumber;                  //数量
    
}show_fat;
extern show_fat fat_ram;





extern FATFS *fs[_VOLUMES];//逻辑磁盘工作区.
extern FILINFO *tfileinfo;	//临时文件信息
extern DIR *picdir;    
extern FIL *filin;
extern FIL *filout;
extern u32 br,bw;
extern u8 pname[_MAX_LFN+1]; //带路径的文件名
extern uint8_t buf_r[512];



u8 exfuns_init(void);   //申请所需要变量
void sd_fatfs_init(void);//sd fatfs初始化
void spi_fatfs_init(void);//spi flash fatfs+字库 初始化
u8 exf_getfree(char *drv,u32 *total,u32 *free);//得到磁盘剩余容量

void copy_data_file(const char *str,const unsigned char *nesdata,u32 number);//复制文件数组数组到文件系统//
void copy_file_file(const unsigned char *namea,const unsigned char *nameb);////复制文件到文件目录//
uint8_t sd_file_search(void);//指定检索sd卡游戏 带字节数
uint8_t spiflash_file_search(void);//指定检索SPI游戏 带字节数



#endif

