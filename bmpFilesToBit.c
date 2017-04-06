#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include <math.h>

#pragma pack(2)

typedef unsigned short WORD;/*����WORDΪ�����ֽڵ�����*/
typedef unsigned long DWORD;/*����DWORDΪe�ĸ��ֽڵ�����*/
HANDLE  hConsole;// ����̨������

typedef struct BMP_FILE_HEADER{/*λͼ�ļ�ͷ*/
    WORD bType; /* �ļ���ʶ�� */
    DWORD bSize; /* �ļ��Ĵ�С */
    WORD bReserved1; /* ����ֵ,��������Ϊ0 */
    WORD bReserved2; /* ����ֵ,��������Ϊ0 */
    DWORD bOffset; /* �ļ�ͷ�����ͼ������λ��ʼ��ƫ���� */
} BMPFILEHEADER;

typedef struct BMP_INFO{/*λͼ��Ϣͷ*/
    DWORD bInfoSize; /* ��Ϣͷ�Ĵ�С */
    DWORD bWidth; /* ͼ��Ŀ�� */
    DWORD bHeight; /* ͼ��ĸ߶� */
    WORD bPlanes; /* ͼ���λ���� */
    WORD bBitCount; /* ÿ�����ص�λ�� */
    DWORD bCompression; /* ѹ������ */
    DWORD bmpImageSize; /* ͼ��Ĵ�С,���ֽ�Ϊ��λ */
    DWORD bXPelsPerMeter; /* ˮƽ�ֱ��� */
    DWORD bYPelsPerMeter; /* ��ֱ�ֱ��� */
    DWORD bClrUsed; /* ʹ�õ�ɫ���� */
    DWORD bClrImportant; /* ��Ҫ����ɫ�� */
} BMPINF;

/*��ɫ��*/
typedef struct RGB_QUAD{/*�����̺�?˳��洢*/
    WORD rgbBlue; /* ��ɫǿ�� */
    WORD rgbGreen; /* ��ɫǿ�� */
    WORD rgbRed; /* ��ɫǿ�� */
    //WORD rgbReversed; /* ����ֵ */
} RGBQUADX;


void ReadOneBmpToBit(char *fileName){

	


	FILE *fp;
    BMPFILEHEADER fileHeader;
    BMPINF infoHeader;
	int i,j;
    /*********************/
	/*****��ȡbmp�ļ�*****/
	/*********************/
    if((fp = fopen(fileName, "rb")) == NULL)
    {
        printf("Cann't open the file!\n");
        exit(0);
    }
    fseek(fp, 0, 0);
    fread(&fileHeader, sizeof(fileHeader), 1, fp);
    fread(&infoHeader, sizeof(infoHeader), 1, fp);
    int height = (int)infoHeader.bHeight ;
    int width = (int)infoHeader.bWidth ;

	RGBQUADX **bmpAllRGBTemp = (RGBQUADX **)malloc(sizeof(int) * height );//�洢��ɫ 
	for(i = 0;i < height;i++){
		bmpAllRGBTemp[i] = (RGBQUADX *)malloc(sizeof(RGBQUADX) * width);
	}
	
	for(i = 0;i < height;i ++){
		for(j = 0;j < width ;j ++){
			bmpAllRGBTemp[i][j].rgbBlue = fgetc(fp);
			bmpAllRGBTemp[i][j].rgbGreen = fgetc(fp);
			bmpAllRGBTemp[i][j].rgbRed = fgetc(fp);
		}
		fseek(fp,0,1);//����;��Բ�ͬɫ��(λ��)��bmpͼƬ
	}
    fclose(fp);

	FILE *fpw;
	fpw=fopen("test.dat","a");
	for(int i9 = height-1;i9 >= 0 ;i9--){
        for(int j9 = 0;j9 < width;j9++){
			if(bmpAllRGBTemp[i9][j9].rgbRed * bmpAllRGBTemp[i9][j9].rgbGreen * bmpAllRGBTemp[i9][j9].rgbBlue >= 2097152){
				fprintf(fp,"��");//��ɫ
			}
			else{
				fprintf(fp,"��");//��ɫ
			}
        }
		fprintf(fp,"\n");
    }
	fprintf(fp,"\n");
	fclose(fpw);
	
	

    for(int l = 0 ;l < height;l ++)
		free(bmpAllRGBTemp[l]);
	free(bmpAllRGBTemp);
}

int main()
{
	char fileName[30];
	char zFileName[40];
	char temp[10] = "bmp24\\\\";
	strcpy(zFileName,temp);
	//int count = 0;
	FILE *fpName;
/*	fpName=fopen("fileName.txt","r");
	if(fpName==NULL){
		fpName=fopen("fileName.txt","w");
		fclose(fpName);
		return 0;
	}
	fseek(fpName,0L,2);
	count=ftell(fpName);
	printf("%d\n",count);*/

	
	fpName=fopen("fileName.txt","r");
	for(int i = 1 ; i <= 7184;i++){
		fscanf(fpName,"%s\n",&fileName);

		strcat(zFileName,fileName);
		//puts(zFileName);
		ReadOneBmpToBit(zFileName);
		printf("%d_7184\n",i);
		memset(zFileName, 0, sizeof(zFileName));
		strcpy(zFileName,temp);
		//getch();
	}
	fclose(fpName);
	getch();
	return 0;
} 