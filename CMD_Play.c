#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

typedef unsigned char BIT;

HANDLE  hConsole;// 控制台输出句柄

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
void gotoxy(int x,int y){//x为列坐标,y为行坐标,z为与x相乘（因为"■"字符占两位），一般z值为1或2
	COORD pos={x,y};
	HANDLE hOut=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut,pos);
}
int main(){
	
	int i,j,k,count;
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
	

	hConsole=GetStdHandle(STD_OUTPUT_HANDLE);//获取控制台输出句柄
	HWND hWnd=GetConsoleHwnd();//获取控制台窗口句柄，用于设置画笔颜色
	CONSOLE_CURSOR_INFO cursorInfo = { 1, FALSE };  // 光标信息
    SetConsoleCursorInfo(hConsole, &cursorInfo);  // 设置光标隐藏
	SetConsoleTextAttribute(hConsole,0x00); //设置颜色
	system("mode con cols=128 lines=64");               //设置控制台宽度,长度
	system("color f0");               //设置控制台颜色
	gotoxy(0,0);



	fseek(fpr1,(sizeof(getBit1)*1000),1);
	fseek(fpr2,(sizeof(getBit2)*1000),1);

	fread(&getBit1,sizeof(getBit1),1,fpr1);

	for(i = 0 ; i < 64 ; i++){
		for(j = 0 ; j < 8;j++){
			gotoxy(j*16,i);
			for(k = 0 ; k < 8 ; k ++){
				if(getBit1[i][j*2] & ((0x80)>>k)){
					printf("*");
				}
				else{
					printf(" ");
				}
			}
			for(k = 0 ; k < 8 ; k ++){
				if(getBit1[i][j*2+1] & ((0x80)>>k)){
					printf("*");
				}
				else{
					printf(" ");
				}
			}
			
		}
	}


	

	for(count = 0 ;count < 1000 ; count ++){
		fread(&getBit1,sizeof(getBit1),1,fpr1);
		fread(&getBit2,sizeof(getBit2),1,fpr2);
		for(i = 0 ; i < 64 ; i++){
			for(j = 0 ; j < 8;j++){
				
				if(getBit2[i] & ((0x80)>>j)){



					gotoxy(j*16,i);
					for(k = 0 ; k < 8 ; k ++){
						if(getBit1[i][j*2] & ((0x80)>>k)){
							printf("*");
						}
						else{
							printf(" ");
						}
					}
					for(k = 0 ; k < 8 ; k ++){
						if(getBit1[i][j*2+1] & ((0x80)>>k)){
							printf("*");
						}
						else{
							printf(" ");
						}
					}
				
				}



			}
		}
	}
	fclose(fpr1);
	fclose(fpr2);
	return 0;
}