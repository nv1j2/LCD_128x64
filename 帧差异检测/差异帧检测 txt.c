#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include <math.h>

#pragma pack(2)

typedef unsigned short WORD;/*定义WORD为两个字节的类型*/

int main(){

	int i,j,k;
	FILE *fpw,*fpr;
	WORD getBit1[64][8];
	WORD getBit2[64][8];
	double num0 = 0,num1 = 0;

	int count = 0;
    if((fpr = fopen("test.dat", "rb")) == NULL)
    {
        printf("Cann't open the file!\n");
        exit(0);
    }
	fpw = fopen("test1.txt","ab");
	

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
			for(j = 0 ; j < 8;j++){
				if(getBit1[i][j] == getBit2[i][j]){
					fprintf(fpw,"0");
					num0++;
				}
				else{
					fprintf(fpw,"1");
					num1++;
				}

			}
			fprintf(fpw,"\n");
		}
		fprintf(fpw,"\n");


	}
	fclose(fpw);
	fclose(fpr);
	printf("0: %f,1: %f;",num0,num1);
	return 0;
}