#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include <math.h>

#pragma pack(2)

typedef unsigned short WORD;/*定义WORD为两个字节的类型*/
typedef unsigned long DWORD;/*定义DWORD为e四个字节的类型*/
HANDLE  hConsole;// 控制台输出句柄

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


HWND GetConsoleHwnd(void){// 获取控制台窗口句柄
	#define MY_BUFSIZE 1024 // Buffer size for console window titles.
	HWND hwndFound; // This is what is returned to the caller.
	char pszNewWindowTitle[MY_BUFSIZE]; // Contains fabricated
	char pszOldWindowTitle[MY_BUFSIZE]; // Contains original
	GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);//获取控制台标题存入pszOldWindowTitle中
	//设置控制台的标题为 时间数 进程ID
	wsprintf(pszNewWindowTitle,"%d/%d",//把获取的数目，和当前进程ID输出到缓冲区pszNewwindowtitle中
	GetTickCount(),//用于获取操作系统启动后的毫秒数
	GetCurrentProcessId());//获取当前进程ID
	SetConsoleTitle(pszNewWindowTitle);//更改控制台的标题为新内容
	Sleep(10);
	hwndFound=FindWindow(NULL, pszNewWindowTitle);//获取控制台的HWND号
	SetConsoleTitle(pszOldWindowTitle);//更改控制台的标题为原始内容
	return(hwndFound);//返回句柄
}

int main()
{
	system("mode con cols=80 lines=35"); 
    hConsole=GetStdHandle(STD_OUTPUT_HANDLE);//获取控制台输出句柄
    HWND hWnd=GetConsoleHwnd();//获取控制台窗口句柄
	CONSOLE_CURSOR_INFO cursorInfo = { 1, FALSE };  // 光标信息
    SetConsoleCursorInfo(hConsole, &cursorInfo);  // 设置光标隐藏
    FILE *fp;
    BMPFILEHEADER fileHeader;
    BMPINF infoHeader;
	int i,j;
    /*********************/
	/*****读取bmp文件*****/
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
	HDC hDC=GetDC(hWnd);
	CONSOLE_SCREEN_BUFFER_INFO bInfo; // 窗口信息
	HANDLE Hout=GetStdHandle(STD_OUTPUT_HANDLE);//获取控制台句柄
	GetConsoleScreenBufferInfo(Hout, &bInfo );//获取控制台信息
	SetBkMode(hDC, TRANSPARENT);
    for(i = 0;i < height ;i++){
        for(j = 0;j < width;j++){
			SetPixelV(hDC,j,abs(height-i),RGB(bmpAllRGBTemp[i][j].rgbRed,bmpAllRGBTemp[i][j].rgbGreen,bmpAllRGBTemp[i][j].rgbBlue));
        }
    }
	
	
	
    /*****存储bmp文件*****/
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