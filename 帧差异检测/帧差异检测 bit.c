#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include <math.h>

#pragma pack(2)

typedef unsigned char BIT;/*定义WORD为两个字节的类型*/

int main(){

	int i,j,k;
	FILE *fpw,*fpr;
	BIT getBit1[64][16];
	BIT getBit2[64][16];
	BIT temp = 0;
	double num0 = 0,num1 = 0;

	int count = 0;
    if((fpr = fopen("file1.dat", "rb")) == NULL)
    {
        printf("Cann't open the file!\n");
        exit(0);
    }
	fpw = fopen("test1.dat","ab");
	

	fread(&getBit1, sizeof(getBit1), 1, fpr);


	for(k = 0; k < 7183;k ++){
		if(count == 0){
			fread(&getBit2, sizeof(getBit2), 1, fpr);
			count = 1;
		}else{
			fread(&getBit1, sizeof(getBit1), 1, fpr);
			count = 0;
		}
		for(i = 0 ; i < 64;i ++){
			for(j = 0 ; j < 8 ;j++){
				if((getBit1[i][j*2] == getBit2[i][j*2]) &&  (getBit1[i][j*2+1] == getBit2[i][j*2+1])){
					//fprintf(fpw,"0");
					temp = temp * 2;
					num0++;
				}
				else{
					//fprintf(fpw,"1");
					temp = temp * 2 + 1;
					num1++;
				}
				if((j+1)%8 == 0){
					fwrite(&temp,sizeof(temp),1,fpw);
					temp = 0;
				}
			}
			//fprintf(fpw,"\n");
		}
		//fprintf(fpw,"\n");


	}
	fclose(fpw);
	fclose(fpr);
	printf("0: %f,1: %f;",num0,num1);
	return 0;
}