#include "cw_queue.h"

/******************************************
 *                  base
 * ****************************************/


Tcw_delay cw_delay = 0;
void cw_queue_init( cw_cycle_queue *Q_ptr,w_u8 *pbuf,w_u32 len,Tcw_delay cwdelay)
{
    Q_ptr->buf = pbuf;
    Q_ptr->size = len;        
    Q_ptr->head = 0;
    Q_ptr->tail = 0;
    Q_ptr->empty = 1;
    Q_ptr->full = 0;
    cw_delay = cwdelay;
    #ifdef QUEUE_REWEIRW
    Q_ptr->full = 1;  //queue input after full, overwrite occur
    #endif
}

void cw_queue_clean( cw_cycle_queue *Q_ptr )
{
    Q_ptr->head = Q_ptr->tail = 0;
    Q_ptr->empty = 1;
    Q_ptr->full = 0;

    #ifdef QUEUE_REWEIRW
    Q_ptr->overflow = 0;
    #endif
}

int cw_queue_write( cw_cycle_queue *Q_ptr,w_u8 *data,w_u32 len ,w_u32 timeout)
{
    int ret = 0;

    for (; ret < len; ret++)
    {
        *(Q_ptr->buf + Q_ptr->head) = *(data + ret);

        if ((1==Q_ptr->full) && (Q_ptr->head==Q_ptr->tail))
        {
            #ifdef QUEUE_REWEIRW
            Q_ptr->overflow = 1;
            #else
            break;
            #endif
        }

        Q_ptr->head = ++Q_ptr->head % Q_ptr->size;

        if (Q_ptr->head == Q_ptr->tail)
        {
            while (timeout--)
            {
                if(cw_delay!=0)cw_delay(1);
                if (Q_ptr->head != Q_ptr->tail)
                {
                    Q_ptr->full = 0;
                    break;
                }
            }
            Q_ptr->full = 1;
        }

        if (1 == Q_ptr->empty)
        {
            Q_ptr->empty = 0;
        }
    }

    #ifdef QUEUE_REWEIRW
    if ( Q_ptr->overflow )
    {
        Q_ptr->tail = Q_ptr->head;
    }
    #endif
          
    return ret;
}

int cw_queue_read( cw_cycle_queue *Q_ptr, w_u8 *data, w_u32 len,w_u32 timeout )
{
    int ret = 0;

    if ( !Q_ptr->empty )
    {    
        while( ret < len )
        {
            *(data + ret) = *(Q_ptr->buf + Q_ptr->tail);
            Q_ptr->tail = ++Q_ptr->tail % Q_ptr->size;
            ret++;

            if ( Q_ptr->tail == Q_ptr->head )
            {
                while (timeout--)
                {
                    if(cw_delay!=0)cw_delay(1);
                    if (Q_ptr->tail == Q_ptr->head)
                    {
                        Q_ptr->empty  = 0;
                        break;
                    }
                }
                Q_ptr->empty = 1;
                break;
            }
        }
    }

    if ( ( ret > 0 ) && ( 1 == Q_ptr->full ) )
    {
        Q_ptr->full = 0;

        #ifdef QUEUE_REWEIRW
        Q_ptr->overflow = 0;
        #endif 
    }
    
    return ret;
}


int cw_queue_blank( cw_cycle_queue *Q_ptr)
{
    int ret;
    if(Q_ptr->tail > Q_ptr->head ){
        ret = Q_ptr->head + Q_ptr->size - Q_ptr->tail;
    }else{
        ret = Q_ptr->head - Q_ptr->tail;
    }
    return ret;
}