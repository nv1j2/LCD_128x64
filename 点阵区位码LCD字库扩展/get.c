#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEIGHT 27 //0~47(63-16)
#define WIDTH 47  //0~111(127-16)

typedef unsigned char BIT;



FILE *pfile;
unsigned char hanzi[32]; /* 16*16点阵 */
void read_hz(unsigned char hz[2]);
void draw_hz();

int main()
{

    unsigned char str[] = "的";
    unsigned char *p = str;
    printf("%s\n",str);

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
    return 0;
}
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
	BIT LCD[64][16];
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
 /*   for(i=0;i<16;++i)
    {
        for(j=0x80;j;j>>=1)
        {
            printf("%c ",(j&hanzi[2*i])?'*':' ');
        }
        for(j=0x80;j;j>>=1)
        {
            printf("%c ",(j&hanzi[2*i+1])?'*':' ');
        }
        printf("\n");
    }*/
	


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

	FILE *fpw;
	fpw = fopen("te.dat", "ab");

	fwrite(&LCD,sizeof(LCD),1,fpw);



	fclose(fpw);

/*	for(i = 0 ; i < 64; i++){
		for(j = 0 ; j < 16 ; j++){
			if((i >= HEIGHT) && (i < (HEIGHT + 16)) && (j >= WIDTH/8 ) && ( ( (WIDTH%8 != 0)  && (j <= (WIDTH+16)/8)  )  || ( (WIDTH%8 == 0)  && (j < (WIDTH+16)/8)  ))  ){
				for(k = 0 ; k < 8 ; k++){
				}
				printf("*");
				
			}
			else
				printf(" ");
		}
		printf("\n");
	}
*/








/*	for(i=0;i<16;i++){
		for(j=0;j<16;j++)
			printf("%d",tmp[i*16+j]);
		printf("\n");
	}*/
/*	for(i=0;i<64;i++){

			if( !tmp[i*4] && !tmp[i*4+1] && !tmp[i*4+2] && !tmp[i+3] )printf("0");
			else if( !tmp[i*4] && !tmp[i*4+1] && !tmp[i*4+2] && tmp[i+3] )printf("1");
			else if( !tmp[i*4] && !tmp[i*4+1] && tmp[i*4+2] && !tmp[i+3] )printf("2");
			else if( !tmp[i*4] && !tmp[i*4+1] && tmp[i*4+2] && tmp[i+3] )printf("3");
			else if( !tmp[i*4] && tmp[i*4+1] && !tmp[i*4+2] && !tmp[i+3] )printf("4");
			else if( !tmp[i*4] && tmp[i*4+1] && !tmp[i*4+2] && tmp[i+3] )printf("5");
			else if( !tmp[i*4] && tmp[i*4+1] && tmp[i*4+2] && !tmp[i+3] )printf("6");
			else if( !tmp[i*4] && tmp[i*4+1] && tmp[i*4+2] && tmp[i+3] )printf("7");
			else if( tmp[i*4] && !tmp[i*4+1] && !tmp[i*4+2] && !tmp[i+3] )printf("8");
			else if( tmp[i*4] && !tmp[i*4+1] && !tmp[i*4+2] && tmp[i+3] )printf("9");
			else if( tmp[i*4] && !tmp[i*4+1] && tmp[i*4+2] && !tmp[i+3] )printf("A");
			else if( tmp[i*4] && !tmp[i*4+1] && tmp[i*4+2] && tmp[i+3] )printf("B");
			else if( tmp[i*4] && tmp[i*4+1] && !tmp[i*4+2] && !tmp[i+3] )printf("C");
			else if( tmp[i*4] && tmp[i*4+1] && !tmp[i*4+2] && tmp[i+3] )printf("D");
			else if( tmp[i*4] && tmp[i*4+1] && tmp[i*4+2] && !tmp[i+3] )printf("E");
			else printf("F");
				
		
	}*/


    printf("\n\n\n");
}