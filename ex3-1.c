#include <stdio.h>
#include <stdlib.h>
#define MAX 100

void get_data();
void rgb_to_ybr(void);
void processing(void);
void ybr_to_rgb(void);
void put_data(void);
int rounds(double num);
//processing内で使う関数
void copy(void);//そのままコピーモード

unsigned char header[54];//bmpのヘッダー部分の情報を格納
unsigned char imgin[3][512][512];
unsigned char imgout[3][512][512];
int fre[256]={};//度数の個数を格納
int nor[256]={};//正規化後の度数格納
int maxFrequency=0;//度数の最大値
int minFrequency=0;//度数の最小値
int maxFrequencykey=0;//最大値の添字

int height,width,g;

int main(void){
    get_data();//画像情報の取得
    rgb_to_ybr();//rgbをycbcrに変換
    processing();//排出
    ybr_to_rgb();//画像再変換
    put_data();//書き出し
    return 0;
}

void ex3(void){
    FILE *fp;
    //画像を大きさ 16×16 のブロックに分割し,各ブロックの Y 信号の平均値を求めて,ファイルに書き出すプログラムを作成せよ.ファイルサイズは512*512
    int result[512][512] = {{}};
    for(int n=0;n<height/16;n++){
        for(int m=0;m<width/16;m++){
            int rs=0;
            for(int i=0;i<16;i++){
                for(int k=0;k<16;k++){
                    rs += imgin[0][16*m+k][16*n+i];
                }
            }
            result[m][n] = rounds(rs / (16*16));
        }
    }
    fp=fopen("ex3-1-result.txt","wb");
    if (fp==NULL){
        printf("ファイルをオープンできません.\n");
        exit (1);
    }
    printf("ファイルをオープンしました.\n");
    /*--- ファイルに書き出し ---*/
    for(int n=0;n<height/16;n++){
        for(int m=0;m<width/16;m++){
            fprintf(fp,"%d ",result[m][n]);
        }
        fprintf(fp,"\n");
    }
    printf("ファイルにデータを書き出しました.\n");
    /*--- ファイル・クローズ(書き出し用) ---*/
    fclose(fp);
    printf("ファイルをクローズしました.\n");
}


void processing(void){
    ex3();
    printf("\n入力画像データをコピーして出力画像データを作成しました。\n");
}

void rgb_to_ybr(void){
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
            p[0]=rounds(0.2990*imgin[0][m][i]+0.5870*imgin[1][m][i]+0.1140*imgin[2][m][i]);
            p[1]=rounds(0.1687*(-1)*imgin[0][m][i]+0.3313*(-1)*imgin[1][m][i]+0.5*imgin[2][m][i])+128;
            p[2]=rounds(0.5*imgin[0][m][i]+0.4187*(-1)*imgin[1][m][i]+0.0813*(-1)*imgin[2][m][i])+128;
            
            //１バイトの範囲外の処理
            for(int k=0;k<3;k++){
                if(p[k]>255){
                    p[k]=255;
                }else if(p[k]<0){
                    p[k]=0;
                }
            }
            imgin[0][m][i]=p[0];
            imgin[1][m][i]=p[1];
            imgin[2][m][i]=p[2];
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

void copy(void){
    for(int k=0;k<3;k++){
        for(int i=0;i<height;i++){
            for(int m=0;m<width;m++){
                imgout[k][m][i]=imgin[k][m][i];
            }
        }
    }
}

void ybr_to_rgb(void){
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
            p[0]=rounds(imgout[0][m][i]+1.402*(imgout[2][m][i]-128));
            p[1]=rounds(imgout[0][m][i]+(-1)*0.3441*(imgout[1][m][i]-128)+(-1)*0.7141*(imgout[2][m][i]-128));
            p[2]=rounds(imgout[0][m][i]+1.7720*(imgout[1][m][i]-128));
            
            //１バイトの範囲外の処理
            for(int k=0;k<3;k++){
                if(p[k]>255){
                    p[k]=255;
                }else if(p[k]<0){
                    p[k]=0;
                }
            }
            imgout[0][m][i]=p[0];
            imgout[1][m][i]=p[1];
            imgout[2][m][i]=p[2];
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

void get_data(){
    FILE *fp;
    int c,hightBite=0,widthBite=0,cfBite=0;
    char fname[100];
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
            header[i]=c;
            c=fgetc(fp);
        }
        /*printf("<ファイルタイプ>\n");
         printf("header[0]=%x,header[1]=%x\n",header[0],header[1]);*/
        printf("<ファイルサイズ>\n");
        //printf("header[2]=%x,header[3]=%x,header[4]=%x,header[5]=%x\n",header[2],header[3],header[4],header[5]);
        printf("%dバイト\n",header[2]+header[3]*256+header[4]*256*256+header[5]*256*256*256);
        /*printf("<予約領域>\n");
         printf("header[6]=%x,header[7]=%x,header[8]=%x,header[9]=%x\n",header[6],header[7],header[8],header[9]);*/
        printf("<オフセット>\n");
        //printf("header[10]=%x,header[11]=%x,header[12]=%x,header[13]=%x\n",header[10],header[11],header[12],header[13]);
        printf("%dバイト\n",header[10]+header[11]*256+header[12]*256*256+header[13]*256*256*256);
        /*
         printf("<情報ヘッダサイズ>\n");
         printf("header[14]=%x,header[15]=%x,header[16]=%x,header[17]=%x\n",header[14],header[15],header[16], header[17]);*/
        printf("<画像の幅>\n");
        //printf("header[18]=%x,header[19]=%x,header[20]=%x,header[21]=%x\n",header[18],header[19],header[20],header[21]);
        printf("%d画素\n",header[18]+header[19]*256+header[20]*256*256+header[21]*256*256*256);
        printf("<画像の高さ>\n");
        //printf("header[22]=%x,header[23]=%x,header[24]=%x,header[25]=%x\n",header[22],header[23],header[24],header[25]);
        printf("%dライン\n",header[22]+header[23]*256+header[24]*256*256+header[25]*256*256*256);
        /* printf("<色プレーン数>\n");
         printf("header[26]=%x,header[27]=%x\n",header[26],header[27]);*/
        printf("<1画素あたりのビット数>\n");
        //printf("header[28]=%x,header[29]=%x\n",header[28],header[29]);
        printf("%dビット\n",header[28]+header[29]*256);
        /*  printf("<圧縮方式>\n");
         printf("header[30]=%x,header[31]=%x,header[32]=%x,header[33]=%x\n",header[30],header[31],header[32],header[33]);
         printf("<画像データサイズ>\n");
         printf("header[34]=%x,header[35]=%x,header[36]=%x,header[37]=%x\n",header[34],header[35],header[36],header[37]);
         printf("<水平解像度>\n");
         printf("header[38]=%x,header[39]=%x,header[40]=%x,header[41]=%x\n",header[38],header[39],header[40],header[41]);
         printf("<垂直解像度>\n");
         printf("header[42]=%x,header[43]=%x,header[44]=%x,header[45]=%x\n",header[42],header[43],header[44],header[45]);
         fclose(fp);
         printf("<色数>\n");
         printf("header[46]=%x,header[47]=%x,header[48]=%x,header[49]=%x\n",header[46],header[47],header[48],header[49]);
         printf("<重要な色数>\n");
         printf("header[50]=%x,header[51]=%x,header[52]=%x,header[53]=%x\n",header[50],header[51],header[52],header[53]);*/
        printf("<挿入ビット数>\n");
        hightBite=header[22]+header[23]*256+header[24]*256*256+header[25]*256*256*256;
        widthBite=header[18]+header[19]*256+header[20]*256*256+header[21]*256*256*256;
        cfBite=54+hightBite*widthBite*(header[28]+header[29]*256)/8;
        printf("%dバイト\n",cfBite%4);
        printf("ファイルをクローズしました\n");
    }
    height=header[22]+header[23]*0x100+header[24]*0x10000+header[25]*0x100000;
    width=header[18]+header[19]*0x100+header[20]*0x10000+header[21]*0x100000;
    for(int a=height-1;a>=0;a--){
        for(int b=0;b<width;b++){
            for(int f=2;f>-1;f--){
                imgin[f][b][a]=g;
                g=fgetc(fp);
            }
        }
    }
    for(int a=height-1;a>=0;a--){
        for(int b=0;b<width;b++){
            for(int g=0;g<3;g++){
                imgout[g][b][a]=imgin[g][b][a];
            }
        }
    }
}
void put_data(void){
    char filename[MAX];
    FILE *fp;
    printf("ファイル名を入力:");
    scanf("%s",filename);
    fp=fopen(filename,"wb");
    if(fp==NULL)
    {
        printf("ファイルをオープンできません\n");
    }
    printf("ファイルをオープンしました\n");
    for(int i=0;i<54;i++){
        fputc(header[i],fp);
    }
    for(int i=height-1;i>=0;i--){
        for(int j=0;j<width;j++){
            for(int k=2;k>=0;k--){
                fputc(imgout[k][j][i],fp );
            }
        }
    }
    fclose(fp);
    printf("ファイルをクローズしました\n");
}




