#include "w25qxx.h" 
#include "spi.h"
#include "stdio.h"


uint16_t W25QXX_TYPE  =  W25Q128;	//Ĭ����W25Q128
//4KbytesΪһ��Sector
//16������Ϊ1��Block
//W25Q128
//����Ϊ16M�ֽ�,����128��Block,4096��Sector 
			
/* ����ʱ */
void delay_us1(uint8_t us)
{
	while(us --){
		__NOP;
		__NOP;
		__NOP;
		__NOP;
		__NOP;
	}
}

//��ʼ��SPI FLASH��IO��
uint8_t W25QXX_Init(void)
{ 
	/* enable the LEDs GPIO clock */
  rcu_periph_clock_enable(W25QXX_CS_RCU);
	 /* configure LED1 GPIO port */ 
	gpio_mode_set(W25QXX_CS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, W25QXX_CS_PIN);
	gpio_output_options_set(W25QXX_CS_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, W25QXX_CS_PIN);

	W25QXX_CS_ON(1);			//SPI FLASH��ѡ��

	//SPI1_Init();		   			//��ʼ��SPI
	// SPI1_SetSpeed(SPI_BaudRatePrescaler_4);		//����Ϊ21Mʱ��
	
	W25QXX_TYPE = W25QXX_ReadID();	//��ȡFLASH ID.
    
    if(((W25QXX_TYPE>=W25Q80)&&(W25QXX_TYPE<=W25Q128)) || ((W25QXX_TYPE>=NM25Q80)&&(W25QXX_TYPE<=NM25Q256))) return 1;
    else return 0;
}  

//��ȡW25QXX��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
uint8_t W25QXX_ReadSR(void)   
{  
	uint8_t byte=0;   
	W25QXX_CS_ON(0);                            //ʹ������   
	spi_ReadWriteByte(W25X_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������    
	byte=spi_ReadWriteByte(0Xff);             //��ȡһ���ֽ�  
	W25QXX_CS_ON(1);                            //ȡ��Ƭѡ     
	return byte;   
} 
//дW25QXX״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void W25QXX_Write_SR(uint8_t sr)   
{   
	W25QXX_CS_ON(0);                            //ʹ������   
	spi_ReadWriteByte(W25X_WriteStatusReg);   //����дȡ״̬�Ĵ�������    
	spi_ReadWriteByte(sr);               //д��һ���ֽ�  
	W25QXX_CS_ON(1);                            //ȡ��Ƭѡ     	      
}   
//W25QXXдʹ��	
//��WEL��λ   
void W25QXX_Write_Enable(void)   
{
	W25QXX_CS_ON(0);                            //ʹ������   
  spi_ReadWriteByte(W25X_WriteEnable);      //����дʹ��  
	W25QXX_CS_ON(1);                            //ȡ��Ƭѡ     	      
} 
//W25QXXд��ֹ	
//��WEL����  
void W25QXX_Write_Disable(void)   
{  
	W25QXX_CS_ON(0);                            //ʹ������   
    spi_ReadWriteByte(W25X_WriteDisable);     //����д��ָֹ��    
	W25QXX_CS_ON(1);                            //ȡ��Ƭѡ     	      
} 		
//��ȡоƬID
//����ֵ����:				   
//0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
//0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
//0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
//0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
//0XEF17,��ʾоƬ�ͺ�ΪW25Q128 	  
uint16_t W25QXX_ReadID(void)
{
	uint16_t Temp = 0;	  
	W25QXX_CS_ON(0);				    
	spi_ReadWriteByte(0x90);//���Ͷ�ȡID����	    
	spi_ReadWriteByte(0x00); 	    
	spi_ReadWriteByte(0x00); 	    
	spi_ReadWriteByte(0x00); 	 			   
	Temp|=spi_ReadWriteByte(0xFF)<<8;  
	Temp|=spi_ReadWriteByte(0xFF);	 
	W25QXX_CS_ON(1);				    
	return Temp;
}   		    
//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)   
{ 
 	uint16_t i;   										    
	W25QXX_CS_ON(0);                            //ʹ������   
    spi_ReadWriteByte(W25X_ReadData);         //���Ͷ�ȡ����   
    spi_ReadWriteByte((uint8_t)((ReadAddr)>>16));  //����24bit��ַ    
    spi_ReadWriteByte((uint8_t)((ReadAddr)>>8));   
    spi_ReadWriteByte((uint8_t)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]= spi_ReadWriteByte(0XFF);   //ѭ������  
    }
	W25QXX_CS_ON(1);  				    	      
}  
//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
 	uint16_t i;  
    W25QXX_Write_Enable();                  //SET WEL 
	W25QXX_CS_ON(0);                            //ʹ������   
    spi_ReadWriteByte(W25X_PageProgram);      //����дҳ����   
    spi_ReadWriteByte((uint8_t)((WriteAddr)>>16)); //����24bit��ַ    
    spi_ReadWriteByte((uint8_t)((WriteAddr)>>8));   
    spi_ReadWriteByte((uint8_t)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)spi_ReadWriteByte(pBuffer[i]);//ѭ��д��  
	W25QXX_CS_ON(1);                            //ȡ��Ƭѡ 
	W25QXX_Wait_Busy();					   //�ȴ�д�����
} 
//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    
} 
//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)						
//NumByteToWrite:Ҫд����ֽ���(���65535)   
uint8_t W25QXX_BUFFER[4096];		 
void W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    
	uint8_t * W25QXX_BUF;	  
   	W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//������ַ  
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//������
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			W25QXX_Erase_Sector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//д����������  

		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
		   	NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	};	 
}
//��������оƬ		  
//�ȴ�ʱ�䳬��...
void W25QXX_Erase_Chip(void)   
{                                   
    W25QXX_Write_Enable();                  //SET WEL 
    W25QXX_Wait_Busy();   
  	W25QXX_CS_ON(0);                            //ʹ������   
    spi_ReadWriteByte(W25X_ChipErase);        //����Ƭ��������  
	W25QXX_CS_ON(1);                            //ȡ��Ƭѡ     	      
	W25QXX_Wait_Busy();   				   //�ȴ�оƬ��������
}   
//����һ������
//Dst_Addr:������ַ ����ʵ����������
//����һ��ɽ��������ʱ��:150ms
void W25QXX_Erase_Sector(uint32_t Dst_Addr)   
{  
	//����falsh�������,������   
 	printf("fe:%x\r\n",Dst_Addr);	  
 	Dst_Addr*=4096;
    W25QXX_Write_Enable();                  //SET WEL 	 
    W25QXX_Wait_Busy();   
  	W25QXX_CS_ON(0);                            //ʹ������   
    spi_ReadWriteByte(W25X_SectorErase);      //������������ָ�� 
    spi_ReadWriteByte((uint8_t)((Dst_Addr)>>16));  //����24bit��ַ    
    spi_ReadWriteByte((uint8_t)((Dst_Addr)>>8));   
    spi_ReadWriteByte((uint8_t)Dst_Addr);  
		W25QXX_CS_ON(1);                            //ȡ��Ƭѡ     	      
    W25QXX_Wait_Busy();   				   //�ȴ��������
}  
//�ȴ�����
void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR()&0x01)==0x01);   // �ȴ�BUSYλ���
}  
//�������ģʽ
void W25QXX_PowerDown(void)   
{ 
  	W25QXX_CS_ON(0);                            //ʹ������   
    spi_ReadWriteByte(W25X_PowerDown);        //���͵�������  
		W25QXX_CS_ON(1);                            //ȡ��Ƭѡ     	      
    delay_us1(3);                               //�ȴ�TPD  
}   
//����
void W25QXX_WAKEUP(void)   
{  
  	W25QXX_CS_ON(0);                            //ʹ������   
    spi_ReadWriteByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
		W25QXX_CS_ON(1);                            //ȡ��Ƭѡ     	      
    delay_us1(3);                               //�ȴ�TRES1
}   

























