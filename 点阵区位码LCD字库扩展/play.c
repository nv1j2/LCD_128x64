 /************************/
/*      LCD汉字显示     */
/************************/
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include "ApiExusb.h"
#pragma comment(lib,"ApiExusb.lib")


#define HEIGHT 7 //0~47(63-16)
#define WIDTH 47  //0~111(127-16)


typedef unsigned char BIT;
unsigned char hanzi[32]; /* 16*16点阵 */
FILE *pfile;
BIT LCD[64][16];


void cmdsetup();
void datasetup();
void read_hz(unsigned char hz[2]);
void draw_hz();
void read_hz(unsigned char hz[2])
{
    int  qu = hz[0]-0xA0; /* 区码 */
    int wei = hz[1]-0xA0; /* 位码 */
    int offset = (94*(qu-1)+(wei-1))*32;
    fseek(pfile,offset,SEEK_SET);
    fread(hanzi,32,1,pfile);
}
void draw_hz()
{
    int i,j,k=0;
	bool tmp[256];
	
	BIT tempBit = 0;
	memset(LCD,0,sizeof(LCD));
	for(i=0;i<16;++i)
    {
        for(j=0x80;j;j>>=1)
        {
            (j&hanzi[2*i])?tmp[k++]=1:tmp[k++]=0;
        }
        for(j=0x80;j;j>>=1)
        {
            (j&hanzi[2*i+1])?tmp[k++]=1:tmp[k++]=0;
        }

    }

	for(i = 0 ; i < 64 ; i++){
		for(j = 0 ; j < 128 ; j++){
			if(i >= HEIGHT && i < HEIGHT+16 && j >= WIDTH && j < WIDTH+16){
				if(tmp[(i-HEIGHT)*16 + (j - WIDTH)]){
					tempBit = tempBit * 2 + 1; 
				}
				else{
					tempBit = tempBit * 2;
				}
			}
			else{
				tempBit = tempBit * 2;
			}

			if((j+1)%8 == 0){
				LCD[i][j/8] = tempBit;
				tempBit = 0;
			}
		}
	}
}


void cmdsetup(){
	PortWriteByte(0x28a,0x00);
	//Sleep(1);	
	PortWriteByte(0x28a,0x04);
	//Sleep(1);	
	PortWriteByte(0x28a,0x00);
	//Sleep(1);	
   }
void datasetup()
   {
	PortWriteByte(0x28a,0x01);
	//Sleep(1);	
	PortWriteByte(0x28a,0x05);	
      //Sleep(1);
	PortWriteByte(0x28a,0x01);	
	//Sleep(1);
   }
void main()                                                    
{

	int i,j,count;
    unsigned char str[] = "萌";
   	unsigned char *p = str;
    if(pfile=fopen("hzk16","rb")) /* ucdos的字库文件 */
   	{
	while(p<str+sizeof(str))
        {
            if(*p>127) /* 汉字 */
            {
                read_hz(p);
                draw_hz();
                p+=2;
            }
            else /* ASCII */
            {
                ++p; /* 忽略ASCII字符 */
            }
        }
        fclose(pfile);
    }
    else
    {
        printf("Can't Open hzk16\n");
    }
	
	if(!Startup())			/*打开设备*/
	{
		printf("ERROR: Open Device Error!\n");
		return;
	}


	PortWriteByte(0x28b,0x80);	/*设8255为方式0,C口输出,A口输出*/

	PortWriteByte(0x28a,0x00);

	PortWriteByte(0x288,0x0c);	//显示状态 开/关;整体显示ON,游标OFF

	double start,end,cost;
	


	PortWriteByte(0x288,0x34);//功能设定,扩充指令集

	PortWriteByte(0x288,0x34);//关闭绘图显示功能。
	

	PortWriteByte(0x28a,0x04);
	PortWriteByte(0x28a,0x00);
	for( i = 0 ;i < 32 ;i++){
		PortWriteByte(0x288,0x80|i);
		cmdsetup();
		PortWriteByte(0x288,0x80);
		cmdsetup();
		for( j = 0 ; j < 16;j++){
			if(j < 8){
				PortWriteByte(0x288,LCD[i][j*2]);

				datasetup();

				PortWriteByte(0x288,LCD[i][j*2+1]);

				datasetup();
			}
			else{
				PortWriteByte(0x288,LCD[i+32][j*2]);

				datasetup();

				PortWriteByte(0x288,LCD[i+32][j*2+1]);

				datasetup();
			}
			
		}
	}
	
	PortWriteByte(0x288,0x36);//打开绘图显示功能。


	PortWriteByte(0x28a,0x04);
	PortWriteByte(0x28a,0x00);

	Cleanup();				/*关闭设备*/
	getch();
}
