#include <stdio.h>
#include <conio.h>
#include <time.h>
#include "ApiExusb.h"
#pragma comment(lib,"ApiExusb.lib")

//#pragma pack(2)

//typedef unsigned short WORD;
typedef unsigned char BIT;

void clear();
void cmdsetup();
void datasetup();
void main()                                                    
{

	int i,j,count;
	BIT getBit1[64][16];
	BIT getBit2[64];
	FILE *fpr1,*fpr2;
	if((fpr1 = fopen("file1.dat","rb")) == NULL){
		printf("ERROR:Cann't open the file1");
		getch();
		exit(0);
	}
	if((fpr2 = fopen("file2.dat","rb")) == NULL){
		printf("ERROR:Cann't open the file2");
		exit(0);
	}
      
	


	printf("Press any key to begin!\n\n");
	//getch();
	if(!Startup())			/*打开设备*/
	{
		printf("ERROR: Open Device Error!\n");
		return;
	}
	fseek(fpr1,(sizeof(getBit1)*1000),1);
	fseek(fpr2,(sizeof(getBit2)*1000),1);

	fread(&getBit1,sizeof(getBit1),1,fpr1);

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
				PortWriteByte(0x288,getBit1[i][j*2]);

				datasetup();

				PortWriteByte(0x288,getBit1[i][j*2+1]);

				datasetup();
			}
			else{
				PortWriteByte(0x288,getBit1[i+32][j*2]);

				datasetup();

				PortWriteByte(0x288,getBit1[i+32][j*2+1]);

				datasetup();
			}
			
		}
	}
	
	PortWriteByte(0x288,0x36);//打开绘图显示功能。


	PortWriteByte(0x28a,0x04);
	PortWriteByte(0x28a,0x00);
	//cmdsetup();
	//printf("1");


	for(count = 0 ; count < 1000 ; count ++){
		printf("%d\t",count);
		start = clock();
		fread(&getBit1,sizeof(getBit1),1,fpr1);
		fread(&getBit2,sizeof(getBit2),1,fpr2);
		//getch();
///////////////////////////////////////////////

///////////////////////////////////////////////
	
	
	//Sleep(200);
	//PortWriteByte(0x288,0x34);//关闭绘图显示功能。
	

	//PortWriteByte(0x28a,0x04);
	PortWriteByte(0x28a,0x00);
	for( i = 0 ;i < 32 ;i++){
		for( j = 0 ; j < 16;j++){
			if(getBit2[i] == 0x00){
				break;
			}
			if(j < 8 ){
				if(getBit2[i] & ((0x80)>>j)){
					PortWriteByte(0x288,0x80|i);
					cmdsetup();
					PortWriteByte(0x288,0x80|j);
					cmdsetup();
					
					
					
					PortWriteByte(0x288,getBit1[i][j*2]);
					
					datasetup();

					PortWriteByte(0x288,getBit1[i][j*2+1]);

					datasetup();
				}
			}
			else{						//j>=8
				if(getBit2[i+32] & ((0x80)>>(j-8))){
					PortWriteByte(0x288,0x80|i);
					cmdsetup();
					PortWriteByte(0x288,0x80|j);
					cmdsetup();
			
					
					PortWriteByte(0x288,getBit1[i+32][(j-8)*2]);

					datasetup();

					PortWriteByte(0x288,getBit1[i+32][(j-8)*2+1]);

					datasetup();
				}
			}		
		}
	}
	
	//PortWriteByte(0x288,0x36);//打开绘图显示功能。


	//PortWriteByte(0x28a,0x04);
	//PortWriteByte(0x28a,0x00);
	
	end = clock();
	cost = end - start;
	printf("%f\n",cost);

	}


	Cleanup();				/*关闭设备*/
	getch();
}

void  clear()
{
	
      //cmdsetup();

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