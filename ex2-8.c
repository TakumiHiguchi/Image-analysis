#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX 100

//入力ファイルの数
#define FILECOUNT 1

//ブロックマッチング法っブロック大きさ
#define BLOCK 16

void get_data(int fileNo);
void rgb_to_ybr(int fileNo);
void processing(int fileNo);
void ybr_to_rgb(int fileNo);
void put_data(int fileNo);
int rounds(double num);
//processing内で使う関数
void copy(int fileNo);//そのままコピーモード
void ex3(int fileNo);

unsigned char header[FILECOUNT][54];//bmpのヘッダー部分の情報を格納
unsigned char imgin[FILECOUNT][3][512][512];
unsigned char imgout[FILECOUNT][3][512][512];
int fre[256]={};//度数の個数を格納
int nor[256]={};//正規化後の度数格納
int maxFrequency=0;//度数の最大値
int minFrequency=0;//度数の最小値
int maxFrequencykey=0;//最大値の添字

int height,width,g;

int main(void){
    for(int i=0; i<FILECOUNT; i++){
        get_data(i);//画像情報の取得
        rgb_to_ybr(i);//rgbをycbcrに変換
        processing(i);//排出
        ybr_to_rgb(i);//画像再変換
    }
    
    ex3(0);
    for(int i=0; i<FILECOUNT; i++){
        put_data(i);//書き出し
    }

    return 0;
}

void ex3(fileNo){
    //ブロック内画素の差分絶対値和を用いて注目画像,参照画像を読み込み,注目画像中に設定した対象ブロックに対するマッチングブロックを求めるプログラムを作成
    
    for(int i=0;i<32;i++){
        for(int m=0;m<32;m++){
            imgout[fileNo][0][m][i] = 255;
            imgout[fileNo][1][m][i] = 255;
            imgout[fileNo][2][m][i] = 0;
        }
    }

}


void processing(fileNo){
    copy(fileNo);
    printf("\n入力画像データをコピーして出力画像データを作成しました。\n");
}

void rgb_to_ybr(fileNo){
    /*
     for(int k=0;k<3;k++){
     if(k==0){printf("--- R ---\n");}else if(k==1){printf("--- G ---\n");}else if(k==2){printf("--- B ---\n");}
     for(int i=0;i<height;i++){
     for(int m=0;m<width;m++){
     printf("%.2X ",imgin[k][m][i]);
     }
     printf("\n");
     }
     }
     */
    for(int i=0;i<height;i++){
        for(int m=0;m<width;m++){
            int p[3]={0,0,0};
            p[0]=rounds(0.2990*imgin[fileNo][0][m][i]+0.5870*imgin[fileNo][1][m][i]+0.1140*imgin[fileNo][2][m][i]);
            p[1]=rounds(0.1687*(-1)*imgin[fileNo][0][m][i]+0.3313*(-1)*imgin[fileNo][1][m][i]+0.5*imgin[fileNo][2][m][i])+128;
            p[2]=rounds(0.5*imgin[fileNo][0][m][i]+0.4187*(-1)*imgin[fileNo][1][m][i]+0.0813*(-1)*imgin[fileNo][2][m][i])+128;
            
            //１バイトの範囲外の処理
            for(int k=0;k<3;k++){
                if(p[k]>255){
                    p[k]=255;
                }else if(p[k]<0){
                    p[k]=0;
                }
            }
            imgin[fileNo][0][m][i]=p[0];
            imgin[fileNo][1][m][i]=p[1];
            imgin[fileNo][2][m][i]=p[2];
        }
    }
    /*
     for(int k=0;k<3;k++){
     if(k==0){printf("--- Y ---\n");}else if(k==1){printf("--- Cb ---\n");}else if(k==2){printf("--- Cr ---\n");}
     for(int i=0;i<height;i++){
     for(int m=0;m<width;m++){
     printf("%.2X ",imgin[k][m][i]);
     }
     printf("\n");
     }
     }
     */
}

void copy(fileNo){
    for(int k=0;k<3;k++){
        for(int i=0;i<height;i++){
            for(int m=0;m<width;m++){
                imgout[fileNo][k][m][i]=imgin[fileNo][k][m][i];
            }
        }
    }
}

void ybr_to_rgb(fileNo){
    /*
     printf("<出力信号(YCbCr)>\n");
     for(int k=0;k<3;k++){
     if(k==0){printf("--- Y ---\n");}else if(k==1){printf("--- Cb ---\n");}else if(k==2){printf("--- Cr ---\n");}
     for(int i=0;i<height;i++){
     for(int m=0;m<width;m++){
     printf("%.2X ",imgout[k][m][i]);
     }
     printf("\n");
     }
     }
     
     printf("<出力信号(RGB)>\n");
     */
    for(int i=0;i<height;i++){
        for(int m=0;m<width;m++){
            int p[3]={0,0,0};
            p[0]=rounds(imgout[fileNo][0][m][i]+1.402*(imgout[fileNo][2][m][i]-128));
            p[1]=rounds(imgout[fileNo][0][m][i]+(-1)*0.3441*(imgout[fileNo][1][m][i]-128)+(-1)*0.7141*(imgout[fileNo][2][m][i]-128));
            p[2]=rounds(imgout[fileNo][0][m][i]+1.7720*(imgout[fileNo][1][m][i]-128));
            
            //１バイトの範囲外の処理
            for(int k=0;k<3;k++){
                if(p[k]>255){
                    p[k]=255;
                }else if(p[k]<0){
                    p[k]=0;
                }
            }
            imgout[fileNo][0][m][i]=p[0];
            imgout[fileNo][1][m][i]=p[1];
            imgout[fileNo][2][m][i]=p[2];
        }
    }
    /*
     for(int k=0;k<3;k++){
     if(k==0){printf("--- R ---\n");}else if(k==1){printf("--- G ---\n");}else if(k==2){printf("--- B ---\n");}
     for(int i=0;i<height;i++){
     for(int m=0;m<width;m++){
     printf("%.2X ",imgout[k][m][i]);
     }
     printf("\n");
     }
     }
     */
}


int rounds(double num){
    double offset;
    if (num>=0.0)
        offset=0.5;
    else
        offset=-0.5;
    return (int)(num+offset);
    
}

void get_data(fileNo){
    FILE *fp;
    int c,hightBite=0,widthBite=0,cfBite=0;
    char fname[100];
    printf("<入力画像 No.%d>\n",fileNo+1);
    printf("ファイル名を入力してください:");
    scanf("%s",fname);
    fp=fopen(fname,"rb");
    if(fp==NULL){
        printf("ファイルをオープンできません.\n");
        exit(1);
    }else{
        printf("ファイルをオープンしました.\n");
        c=fgetc(fp);
        for(int i=0;i<54;i++){
            header[fileNo][i]=c;
            c=fgetc(fp);
        }
        /*printf("<ファイルタイプ>\n");
         printf("header[fileNo][0]=%x,header[fileNo][1]=%x\n",header[fileNo][0],header[fileNo][1]);*/
        printf("<ファイルサイズ>\n");
        //printf("header[fileNo][2]=%x,header[fileNo][3]=%x,header[fileNo][4]=%x,header[fileNo][5]=%x\n",header[fileNo][2],header[fileNo][3],header[fileNo][4],header[fileNo][5]);
        printf("%dバイト\n",header[fileNo][2]+header[fileNo][3]*256+header[fileNo][4]*256*256+header[fileNo][5]*256*256*256);
        /*printf("<予約領域>\n");
         printf("header[fileNo][6]=%x,header[fileNo][7]=%x,header[fileNo][8]=%x,header[fileNo][9]=%x\n",header[fileNo][6],header[fileNo][7],header[fileNo][8],header[fileNo][9]);*/
        printf("<オフセット>\n");
        //printf("header[fileNo][10]=%x,header[fileNo][11]=%x,header[fileNo][12]=%x,header[fileNo][13]=%x\n",header[fileNo][10],header[fileNo][11],header[fileNo][12],header[fileNo][13]);
        printf("%dバイト\n",header[fileNo][10]+header[fileNo][11]*256+header[fileNo][12]*256*256+header[fileNo][13]*256*256*256);
        /*
         printf("<情報ヘッダサイズ>\n");
         printf("header[fileNo][14]=%x,header[fileNo][15]=%x,header[fileNo][16]=%x,header[fileNo][17]=%x\n",header[fileNo][14],header[fileNo][15],header[fileNo][16], header[fileNo][17]);*/
        printf("<画像の幅>\n");
        //printf("header[fileNo][18]=%x,header[fileNo][19]=%x,header[fileNo][20]=%x,header[fileNo][21]=%x\n",header[fileNo][18],header[fileNo][19],header[fileNo][20],header[fileNo][21]);
        printf("%d画素\n",header[fileNo][18]+header[fileNo][19]*256+header[fileNo][20]*256*256+header[fileNo][21]*256*256*256);
        printf("<画像の高さ>\n");
        //printf("header[fileNo][22]=%x,header[fileNo][23]=%x,header[fileNo][24]=%x,header[fileNo][25]=%x\n",header[fileNo][22],header[fileNo][23],header[fileNo][24],header[fileNo][25]);
        printf("%dライン\n",header[fileNo][22]+header[fileNo][23]*256+header[fileNo][24]*256*256+header[fileNo][25]*256*256*256);
        /* printf("<色プレーン数>\n");
         printf("header[fileNo][26]=%x,header[fileNo][27]=%x\n",header[fileNo][26],header[fileNo][27]);*/
        printf("<1画素あたりのビット数>\n");
        //printf("header[fileNo][28]=%x,header[fileNo][29]=%x\n",header[fileNo][28],header[fileNo][29]);
        printf("%dビット\n",header[fileNo][28]+header[fileNo][29]*256);
        /*  printf("<圧縮方式>\n");
         printf("header[fileNo][30]=%x,header[fileNo][31]=%x,header[fileNo][32]=%x,header[fileNo][33]=%x\n",header[fileNo][30],header[fileNo][31],header[fileNo][32],header[fileNo][33]);
         printf("<画像データサイズ>\n");
         printf("header[fileNo][34]=%x,header[fileNo][35]=%x,header[fileNo][36]=%x,header[fileNo][37]=%x\n",header[fileNo][34],header[fileNo][35],header[fileNo][36],header[fileNo][37]);
         printf("<水平解像度>\n");
         printf("header[fileNo][38]=%x,header[fileNo][39]=%x,header[fileNo][40]=%x,header[fileNo][41]=%x\n",header[fileNo][38],header[fileNo][39],header[fileNo][40],header[fileNo][41]);
         printf("<垂直解像度>\n");
         printf("header[fileNo][42]=%x,header[fileNo][43]=%x,header[fileNo][44]=%x,header[fileNo][45]=%x\n",header[fileNo][42],header[fileNo][43],header[fileNo][44],header[fileNo][45]);
         fclose(fp);
         printf("<色数>\n");
         printf("header[fileNo][46]=%x,header[fileNo][47]=%x,header[fileNo][48]=%x,header[fileNo][49]=%x\n",header[fileNo][46],header[fileNo][47],header[fileNo][48],header[fileNo][49]);
         printf("<重要な色数>\n");
         printf("header[fileNo][50]=%x,header[fileNo][51]=%x,header[fileNo][52]=%x,header[fileNo][53]=%x\n",header[fileNo][50],header[fileNo][51],header[fileNo][52],header[fileNo][53]);*/
        printf("<挿入ビット数>\n");
        hightBite=header[fileNo][22]+header[fileNo][23]*256+header[fileNo][24]*256*256+header[fileNo][25]*256*256*256;
        widthBite=header[fileNo][18]+header[fileNo][19]*256+header[fileNo][20]*256*256+header[fileNo][21]*256*256*256;
        cfBite=54+hightBite*widthBite*(header[fileNo][28]+header[fileNo][29]*256)/8;
        printf("%dバイト\n",cfBite%4);
        printf("ファイルをクローズしました\n");
    }
    height=header[fileNo][22]+header[fileNo][23]*0x100+header[fileNo][24]*0x10000+header[fileNo][25]*0x100000;
    width=header[fileNo][18]+header[fileNo][19]*0x100+header[fileNo][20]*0x10000+header[fileNo][21]*0x100000;
    for(int a=height-1;a>=0;a--){
        for(int b=0;b<width;b++){
            for(int f=2;f>-1;f--){
                imgin[fileNo][f][b][a]=g;
                g=fgetc(fp);
            }
        }
    }
    for(int a=height-1;a>=0;a--){
        for(int b=0;b<width;b++){
            for(int g=0;g<3;g++){
                imgout[fileNo][g][b][a]=imgin[fileNo][g][b][a];
            }
        }
    }
}
void put_data(fileNo){
    char filename[MAX];
    FILE *fp;
    printf("<出力画像 No.%d>\n",fileNo+1);
    printf("ファイル名を入力:");
    scanf("%s",filename);
    fp=fopen(filename,"wb");
    if(fp==NULL)
    {
        printf("ファイルをオープンできません\n");
    }
    printf("ファイルをオープンしました\n");
    for(int i=0;i<54;i++){
        fputc(header[fileNo][i],fp);
    }
    for(int i=height-1;i>=0;i--){
        for(int j=0;j<width;j++){
            for(int k=2;k>=0;k--){
                fputc(imgout[fileNo][k][j][i],fp );
            }
        }
    }
    fclose(fp);
    printf("ファイルをクローズしました\n");
    printf("\n");
}




