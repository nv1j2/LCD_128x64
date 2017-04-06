#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include <math.h>

#pragma pack(2)

typedef unsigned short WORD;/*定义WORD为两个字节的类型*/
typedef unsigned long DWORD;/*定义DWORD为e四个字节的类型*/


typedef struct BMP_FILE_HEADER{/*位图文件头*/
    WORD bType; /* 文件标识符 */
    DWORD bSize; /* 文件的大小 */
    WORD bReserved1; /* 保留值,必须设置为0 */
    WORD bReserved2; /* 保留值,必须设置为0 */
    DWORD bOffset; /* 文件头的最后到图像数据位开始的偏移量 */
} BMPFILEHEADER;

typedef struct BMP_INFO{/*位图信息头*/
    DWORD bInfoSize; /* 信息头的大小 */
    DWORD bWidth; /* 图像的宽度 */
    DWORD bHeight; /* 图像的高度 */
    WORD bPlanes; /* 图像的位面数 */
    WORD bBitCount; /* 每个像素的位数 */
    DWORD bCompression; /* 压缩类型 */
    DWORD bmpImageSize; /* 图像的大小,以字节为单位 */
    DWORD bXPelsPerMeter; /* 水平分辨率 */
    DWORD bYPelsPerMeter; /* 垂直分辨率 */
    DWORD bClrUsed; /* 使用的色彩数 */
    DWORD bClrImportant; /* 重要的颜色数 */
} BMPINF;

/*彩色表*/
typedef struct RGB_QUAD{/*以蓝绿红?顺序存储*/
    WORD rgbBlue; /* 蓝色强度 */
    WORD rgbGreen; /* 绿色强度 */
    WORD rgbRed; /* 红色强度 */
    //WORD rgbReversed; /* 保留值 */
} RGBQUADX;


void ReadOneBmpToBit(char *fileName){
	
	int fBug = 0;
	WORD temp;

	FILE *fp;
    BMPFILEHEADER fileHeader;
    BMPINF infoHeader;
	int i,j;
    /*********************/
	/*****读取bmp文件*****/
	/*********************/
    if((fp = fopen(fileName, "rb")) == NULL)
    {
        printf("Cann't open the file!\n");
        exit(0);
    }
    fseek(fp, 0, 0);
    fread(&fileHeader, sizeof(fileHeader), 1, fp);
    fread(&infoHeader, sizeof(infoHeader), 1, fp);
    int height = (int)infoHeader.bHeight ;//高64，宽128
    int width  = (int)infoHeader.bWidth ;

	RGBQUADX **bmpAllRGBTemp = (RGBQUADX **)malloc(sizeof(int) * height );//存储颜色 
	for(i = 0;i < height;i++){
		bmpAllRGBTemp[i] = (RGBQUADX *)malloc(sizeof(RGBQUADX) * width);
	}
	
	for(i = 0;i < height;i ++){
		for(j = 0;j < width ;j ++){
			bmpAllRGBTemp[i][j].rgbBlue = fgetc(fp);
			bmpAllRGBTemp[i][j].rgbGreen = fgetc(fp);
			bmpAllRGBTemp[i][j].rgbRed = fgetc(fp);
		}
		fseek(fp,0,1);//修正;针对不同色深(位数)的bmp图片
	}
    fclose(fp);
	
	FILE *fpw;
	fpw=fopen("test.dat","a");
	for(int i9 = height/4+4;i9 >= height/4+4 ;i9--){
        for(int j9 = 0;j9 < width;j9++){//width = 128
			if(bmpAllRGBTemp[i9][j9].rgbRed * bmpAllRGBTemp[i9][j9].rgbGreen * bmpAllRGBTemp[i9][j9].rgbBlue > 2097152){
				temp = temp*2;
				//fprintf(fpw,"0");//白色
				printf("0");
			}
			else{
				temp = temp*2 + 1;
				//fprintf(fpw,"1");//黑色
				printf("1");
			}
			if((j9+1)%16 == 0){
				fwrite(&temp,sizeof(temp),1,fpw);
				printf("\t%d",temp);
				printf("*\n");
				temp = 0;
			}
        }
		//fprintf(fpw,"\n");
    }
	//fprintf(fpw,"\n");
	fclose(fpw);
	
	

    for(int l = 0 ;l < height;l ++)
		free(bmpAllRGBTemp[l]);
	free(bmpAllRGBTemp);
}

int main()
{

		ReadOneBmpToBit("1384.bmp");

	getch();
	return 0;
} 