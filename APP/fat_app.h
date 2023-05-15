#ifndef _FAT_APP_H
#define _FAT_APP_H

#include "gd32f4xx.h"
#include "systick.h"
#include "ff.h"

////exfuns_init();							//Ϊfatfs��ر��������ڴ�

#define max_nes_number  100 //�����ʾnes��Ϸ����
#define max_name_number 64  //���ֳ���
//nes�ļ��б�ṹ��
typedef struct  
{
    uint8_t sd_card;                        //sd��
    u32     sd_total;                       //������
    u32     sd_free;                        //ʣ������
    uint8_t *sd_nesname[max_nes_number];    ///ָ��nes��Ϸ����ָ�� ��̬�����С
    uint8_t *sd_sizename[max_nes_number];   //��������ʾ��Ϸ��ָ��
    uint8_t sd_nesnumber;                   //����
    uint8_t spi_flash;                      //spi flash
    uint8_t word_stock;                     //�ֿ�
    u32     spi_total;                      //������
    u32     spi_free;                       //ʣ������
    uint8_t *spi_nesname[max_nes_number];   ///ָ��nes��Ϸ����ָ�� ��̬�����С
    uint8_t *spi_sizename[max_nes_number];  //��������ʾ��Ϸ��ָ��
    uint8_t spi_nesnumber;                  //����
    
}show_fat;
extern show_fat fat_ram;





extern FATFS *fs[_VOLUMES];//�߼����̹�����.
extern FILINFO *tfileinfo;	//��ʱ�ļ���Ϣ
extern DIR *picdir;    
extern FIL *filin;
extern FIL *filout;
extern u32 br,bw;
extern u8 pname[_MAX_LFN+1]; //��·�����ļ���
extern uint8_t buf_r[512];



u8 exfuns_init(void);   //��������Ҫ����
void sd_fatfs_init(void);//sd fatfs��ʼ��
void spi_fatfs_init(void);//spi flash fatfs+�ֿ� ��ʼ��
u8 exf_getfree(char *drv,u32 *total,u32 *free);//�õ�����ʣ������

void copy_data_file(const char *str,const unsigned char *nesdata,u32 number);//�����ļ��������鵽�ļ�ϵͳ//
void copy_file_file(const unsigned char *namea,const unsigned char *nameb);////�����ļ����ļ�Ŀ¼//
uint8_t sd_file_search(void);//ָ������sd����Ϸ ���ֽ���
uint8_t spiflash_file_search(void);//ָ������SPI��Ϸ ���ֽ���



#endif

