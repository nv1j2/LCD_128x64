#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

typedef unsigned char BIT;

HANDLE  hConsole;// ����̨������

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
void gotoxy(int x,int y){//xΪ������,yΪ������,zΪ��x��ˣ���Ϊ"��"�ַ�ռ��λ����һ��zֵΪ1��2
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
	

	hConsole=GetStdHandle(STD_OUTPUT_HANDLE);//��ȡ����̨������
	HWND hWnd=GetConsoleHwnd();//��ȡ����̨���ھ�����������û�����ɫ
	CONSOLE_CURSOR_INFO cursorInfo = { 1, FALSE };  // �����Ϣ
    SetConsoleCursorInfo(hConsole, &cursorInfo);  // ���ù������
	SetConsoleTextAttribute(hConsole,0x00); //������ɫ
	system("mode con cols=128 lines=64");               //���ÿ���̨���,����
	system("color f0");               //���ÿ���̨��ɫ
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