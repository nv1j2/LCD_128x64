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


HWND GetConsoleHwnd(void){// ��ȡ����̨���ھ��
	#define MY_BUFSIZE 1024 // Buffer size for console window titles.
	HWND hwndFound; // This is what is returned to the caller.
	char pszNewWindowTitle[MY_BUFSIZE]; // Contains fabricated
	char pszOldWindowTitle[MY_BUFSIZE]; // Contains original
	GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);//��ȡ����̨�������pszOldWindowTitle��
	//���ÿ���̨�ı���Ϊ ʱ���� ����ID
	wsprintf(pszNewWindowTitle,"%d/%d",//�ѻ�ȡ����Ŀ���͵�ǰ����ID�����������pszNewwindowtitle��
	GetTickCount(),//���ڻ�ȡ����ϵͳ������ĺ�����
	GetCurrentProcessId());//��ȡ��ǰ����ID
	SetConsoleTitle(pszNewWindowTitle);//���Ŀ���̨�ı���Ϊ������
	Sleep(10);
	hwndFound=FindWindow(NULL, pszNewWindowTitle);//��ȡ����̨��HWND��
	SetConsoleTitle(pszOldWindowTitle);//���Ŀ���̨�ı���Ϊԭʼ����
	return(hwndFound);//���ؾ��
}

int main()
{
	system("mode con cols=80 lines=35"); 
    hConsole=GetStdHandle(STD_OUTPUT_HANDLE);//��ȡ����̨������
    HWND hWnd=GetConsoleHwnd();//��ȡ����̨���ھ��
	CONSOLE_CURSOR_INFO cursorInfo = { 1, FALSE };  // �����Ϣ
    SetConsoleCursorInfo(hConsole, &cursorInfo);  // ���ù������
    FILE *fp;
    BMPFILEHEADER fileHeader;
    BMPINF infoHeader;
	int i,j;
    /*********************/
	/*****��ȡbmp�ļ�*****/
	/*********************/
    if((fp = fopen("9.bmp", "rb")) == NULL)
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
	HDC hDC=GetDC(hWnd);
	CONSOLE_SCREEN_BUFFER_INFO bInfo; // ������Ϣ
	HANDLE Hout=GetStdHandle(STD_OUTPUT_HANDLE);//��ȡ����̨���
	GetConsoleScreenBufferInfo(Hout, &bInfo );//��ȡ����̨��Ϣ
	SetBkMode(hDC, TRANSPARENT);
    for(i = 0;i < height ;i++){
        for(j = 0;j < width;j++){
			SetPixelV(hDC,j,abs(height-i),RGB(bmpAllRGBTemp[i][j].rgbRed,bmpAllRGBTemp[i][j].rgbGreen,bmpAllRGBTemp[i][j].rgbBlue));
        }
    }
	
	
	
    /*****�洢bmp�ļ�*****/
/*	FILE *fpOut=fopen("3.bmp","wb");
	fwrite(&fileHeader,sizeof(BMPFILEHEADER),1,fpOut);
	fwrite(&infoHeader,sizeof(BMPINF),1,fpOut);
	
	for(i = 0;i < height;i ++){
		for(j = 0;j <= width ;j ++){
			fwrite(&bmpAllRGBTemp[i][j].rgbBlue,1,1,fpOut);
			fwrite(&bmpAllRGBTemp[i][j].rgbGreen,1,1,fpOut);
			fwrite(&bmpAllRGBTemp[i][j].rgbRed,1,1,fpOut);
		}
		
	}*/

    for(int l = 0 ;l < height;l ++)
		free(bmpAllRGBTemp[l]);
	free(bmpAllRGBTemp);

	getch();
return 0;
} 