
#ifndef _CW_QUEUE_H
#define _CW_QUEUE_H
#include "stdio.h"

#define  FD_GPS_UART_BUFFER_SIZE        50

//#define QUEUE_REWEIRW
/*****************************/
typedef unsigned char		w_u8;
typedef unsigned short		w_u16;
typedef unsigned int 	    w_u32;
/*****************************************************************/
typedef void (*Tcw_delay)(unsigned int);
typedef struct {
    w_u8 *buf;
    w_u32 size;        
    w_u32 head;
    w_u32 tail;
    w_u8 empty: 1;
    w_u8 full:  1;

    #ifdef QUEUE_REWEIRW
    w_u8 overflow:  1;  //queue input after full, overwrite occur
    #endif
}   cw_cycle_queue;


typedef struct{
	int flag;	
	cw_cycle_queue *queue;
} flg_queue;


void cw_queue_init( cw_cycle_queue *Q_ptr,w_u8 *pbuf,w_u32 len,Tcw_delay cwdelay);
int cw_queue_write( cw_cycle_queue *Q_ptr,w_u8 *data,w_u32 len,w_u32 timeout );
void cw_queue_clean( cw_cycle_queue *Q_ptr );
int cw_queue_read( cw_cycle_queue *Q_ptr, w_u8 *data, w_u32 len,w_u32 timeout );
int cw_queue_blank( cw_cycle_queue *Q_ptr);

#endif
