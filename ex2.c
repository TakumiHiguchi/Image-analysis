#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX 100

void get_data();
void rgb_to_ybr(void);
void processing(void);
void ybr_to_rgb(void);
void put_data(void);
int rounds(double num);
void maxElement(int* array,int num);
void minElement(int* array,int num);
//processing内で使う関数
void inversion(void);//左右反転
void replace(void);//固定値(128)に置き換えモード
void copy(void);//そのままコピーモード
void blackout(void);//左下と右上を黒く塗りつぶすモード
void quantization(void);
void frequency(int output);//度数表示もーど
void normalization(void);//度数の正規化
void histogram(void);//ヒストグラムを左下に作るモード
void contrastImp(void);//コントラスト改善
void expansion_and_shrink(void);//拡大縮小
void filter(void);//フィルター
void tes(void);
void grayscale(void);//濃淡変換
void grayscale2(void);//濃淡変換

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

void grayscale2(void){
    double filter[25][25] = {{0}};
    //フィルター
    for(int i=-12;i<13;i++){
        for(int k=-12;k<13;k++){
            double x = -1*(i*i+k*k)/(2*2.5*2.5);
            filter[i+12][k+12] = (1/(2*3.14*2.5*2.5))*exp(x);
        }
    }
    copy();
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            double result = 0;
            for(int n=-12;n<13;n++){
                for(int m=-12;m<13;m++){
                    int y=i+n;
                    int x=j+m;
                    if(y >= height)
                        y = height*2 - y - 1;
                    if(x >= width)
                        x = width*2 - x - 1;
                    if(y < 0)
                        y = abs(y) - 1;
                    if(x < 0)
                        x = abs(x) - 1;
                    result += imgin[0][x][y] * filter[n+12][m+12];
                }
            }
            imgout[0][j][i] = rounds(result);
        }
    }
}
void grayscale(void){
    int con[256]={};//度数の個数
    int count = 0;
    int level=height*width/256;
    int rank = 0;
    for(int i=0;i<height;i++){
        for(int m=0;m<width;m++){
            con[imgin[0][m][i]]++;
        }
    }
    copy();
    for(int i=0;i<256;i++){
        count+=con[i];
        for(int s=0;s<height;s++){
            for(int l=0;l<width;l++){
                if(imgin[0][s][l]==i){
                    imgout[0][s][l] = rank;
                }
            }
        }
        do{
            if(count>level){
                count-=level;
                rank++;
            }
        }while(count>level);
    }
}

void tes(void){
    double n=0.0;
    int ave;
    for(int i=height/4;i<=height/2;i+=2){
        for(int m=0;m<=width;m+=2){
            int ave=rounds((double)(imgin[0][m][i+1]+imgin[0][m+1][i]+imgin[0][m][i]+imgin[0][m+1][i+1])/4);
            
            imgout[0][m][i+1]=ave;
            imgout[0][m+1][i]=ave;
            imgout[0][m][i]=ave;
            imgout[0][m+1][i+1]=ave;
        }
    }
    for(int i=height/2;i<=height/4*3;i+=4){
        for(int m=0;m<=width;m+=4){
            for(int f=0;f<4;f++){
                for(int g=0;g<4;g++){
                    n+=(double)(imgin[0][m+f][i+g]);
                }
            }
            ave=rounds(n/16.0);
            n=0.0;
            for(int f=0;f<4;f++){
                for(int g=0;g<4;g++){
                    imgout[0][m+f][i+g]=ave;
                }
            }
        }
    }
    for(int i=height/4*3;i<=height;i+=8){
        for(int m=0;m<=width;m+=8){
            for(int f=0;f<8;f++){
                for(int g=0;g<8;g++){
                    n+=(double)(imgin[0][m+f][i+g]);
                }
            }
            ave=rounds(n/64.0);
            n=0.0;
            for(int f=0;f<8;f++){
                for(int g=0;g<8;g++){
                    imgout[0][m+f][i+g]=ave;
                }
            }
        }
    }

    //おきかえ
    int p[3]={1,0,0};
    for(int ks=0;ks<3;ks++){
        for(int is=0;is<height;is++){
            for(int ms=0;ms<width;ms++){
                if(p[ks]==0){
                    imgout[ks][ms][is]=128;
                }
            }
        }
    }
}
void filter(void){
    double result=0;
    double filterCoefficient[3][3]={{0,-1,0},{-1,5,-1},{0,-1,0}};
    printf("<フィルタ係数>\n");
    for(int i=0;i<3;i++){
        for(int k=0;k<3;k++){
            printf("%f ",filterCoefficient[i][k]);
        }
        printf("\n");
    }
    for(int x=0;x<width;x++){
        for(int y=0;y<height;y++){
            result=0;
            if(x==0||x==width-1||y==0||y==height-1){
                imgout[0][x][y]=16;
                imgout[1][x][y]=128;
                imgout[2][x][y]=128;
            }else{
                for(int i=-1;i<2;i++){
                    for(int k=-1;k<2;k++){
                        result+=imgin[0][x+i][y+k]*filterCoefficient[1+i][1+k];
                    }
                }
                int res=rounds(result);
                if(res<0){
                    res=0;
                }else if(res>255){
                    res=255;
                }
                imgout[0][x][y]=res;
                imgout[1][x][y]=128;
                imgout[2][x][y]=128;
            }
        }
    }
}
void expansion_and_shrink(void){
    double es;
    printf("拡大縮小率を入力して下さい:");
    scanf("%lf",&es);
    for(int i=0;i<height;i++){
        for(int m=0;m<width;m++){
            if(0<=m/es&&m/es<(width-1)&&0<=i/es&&i/es<(height-1)){
                imgout[0][m][i]=imgin[0][rounds((double)m/es)][rounds((double)i/es)];
                imgout[1][m][i]=imgin[1][rounds((double)m/es)][rounds((double)i/es)];
                imgout[2][m][i]=imgin[2][rounds((double)m/es)][rounds((double)i/es)];
            }else{
                imgout[0][m][i]=16;
                imgout[1][m][i]=128;
                imgout[2][m][i]=128;
            }
        }
    }
}
void maxElement(int* array,int num){
    int max=0;
    for(int i=0;i<num;i++){
        if(max<array[i]){
            max=array[i];
            maxFrequencykey=i;
        }
    }
    maxFrequency=max;
}
void minElement(int* array,int num){
    int min;
    for(int i=0;i<num;i++){
        if(i==0)min=array[i];
        if(min>array[i]){
            min=array[i];
        }
    }
    minFrequency=min;
}
void contrastImp(void){
    int max=0;
    int min;
    for(int i=0;i<height;i++){
        for(int m=0;m<width;m++){
            if(i==0&&m==0)
                min=imgin[0][m][i];
            if(max<imgin[0][m][i])
                max=imgin[0][m][i];
            if(min>imgin[0][m][i])
                min=imgin[0][m][i];
        }
    }
    printf("最大画素数：%d\n最小画素数：%d\n\n",max,min);
    for(int i=0;i<height;i++){
        for(int m=0;m<width;m++){
            imgout[0][m][i]=rounds((double)255/(double)(max-min)*(imgin[0][m][i]-min));
            imgout[1][m][i]=imgin[1][m][i];
            imgout[2][m][i]=imgin[2][m][i];
        }
    }
}
void histogram(void){
    normalization();
    copy();
    for(int i=height;i>(height-100);i--){
        for(int m=0;m<256;m++){
            imgout[0][m][i-1]=0x00;
            imgout[1][m][i-1]=0x80;
            imgout[2][m][i-1]=0x80;
        }
    }
    for(int i=0;i<256;i++){
        for(int r=height;r>(height-nor[i]);r--){
            imgout[2][i][r-1]=0xff;
        }
    }
}
void inversion(void){
    for(int k=0;k<3;k++){
        for(int i=0;i<height;i++){
            for(int m=width;m>0;m--){
                imgout[k][m][i]=imgin[k][width-m][i];
            }
        }
    }
}
void normalization(void){
    frequency(0);
    maxElement(fre,256);
    printf("<最大度数>\n画素値%d：%d\n",maxFrequencykey,maxFrequency);
    for(int i=0;i<256;i++){
        printf("画素値 %d：%d --> %d\n",i,fre[i],rounds((double)fre[i]/(double)maxFrequency*100));
        nor[i]=rounds((double)fre[i]/(double)maxFrequency*100);
    }
}
void frequency(int output){
    for(int i=0;i<height;i++){
        for(int m=0;m<width;m++){
            int k=imgin[0][m][i];
            fre[k]+=1;
        }
    }
    if(output==0){
        for(int f=0;f<256;f++){
            printf("画素値 %d：%d\n",f,fre[f]);
        }
    }
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
void processing(void){
    grayscale2();
    
    printf("\n入力画像データをコピーして出力画像データを作成しました。\n");
}
void blackout(void){
    for(int i=height/2;i<=height;i++){
        for(int m=0;m<=width/2;m++){
            imgin[0][m][i]=16;
            imgin[1][m][i]=128;
            imgin[2][m][i]=128;
        }
    }//左下
    for(int i=0;i<=height/2;i++){
        for(int m=width/2;m<=width;m++){
            imgin[0][m][i]=16;
            imgin[1][m][i]=128;
            imgin[2][m][i]=128;
        }
    }//右上
    copy();
}
void quantization(void){
    int size;
    printf("出力画像の量子化レベル数を入力して下さい(2,4,8,16):");
    scanf("%d",&size);
    for(int i=0;i<height;i++){//量子化
        for(int j=0;j<width;j++){
            if(size==2){
                if(imgout[0][j][i]<128){
                    imgout[0][j][i]=64;
                }else if(imgout[0][j][i]>127){
                    imgout[0][j][i]=192;
                }
            }else if(size==4){
                if(imgout[0][j][i]<64){
                    imgout[0][j][i]=32;
                }else if(imgout[0][j][i]>63&&imgout[0][j][i]<128){
                    imgout[0][j][i]=96;
                }else if(imgout[0][j][i]>128&&imgout[0][j][i]<192){
                    imgout[0][j][i]=160;
                }else if(imgout[0][j][i]>191){
                    imgout[0][j][i]=224;
                }
            }else if(size==8){
                if(imgout[0][j][i]<32){
                    imgout[0][j][i]=16;
                }else if(imgout[0][j][i]>32-1&&imgout[0][j][i]<64){
                    imgout[0][j][i]=48;
                }else if(imgout[0][j][i]>64-1&&imgout[0][j][i]<96){
                    imgout[0][j][i]=80;
                }else if(imgout[0][j][i]>96-1&&imgout[0][j][i]<128){
                    imgout[0][j][i]=112;
                }else if(imgout[0][j][i]<128-1&&imgout[0][j][i]<160){
                    imgout[0][j][i]=144;
                }else if(imgout[0][j][i]>160-1&&imgout[0][j][i]<191){
                    imgout[0][j][i]=176;
                }else if(imgout[0][j][i]>191-1&&imgout[0][j][i]<223){
                    imgout[0][j][i]=208;
                }else if(imgout[0][j][i]>224){
                    imgout[0][j][i]=240;
                }
            }else if(size==16){
                if(imgout[0][j][i]<16){
                    imgout[0][j][i]=8;
                }else if(imgout[0][j][i]>16-1&&imgout[0][j][i]<32){
                    imgout[0][j][i]=24;
                }else if(imgout[0][j][i]>32-1&&imgout[0][j][i]<48){
                    imgout[0][j][i]=40;
                }else if(imgout[0][j][i]<48&&imgout[0][j][i]<64){
                    imgout[0][j][i]=56;
                }else if(imgout[0][j][i]>64-1&&imgout[0][j][i]<80){
                    imgout[0][j][i]=72;
                }else if(imgout[0][j][i]>191-1&&imgout[0][j][i]<96){
                    imgout[0][j][i]=88;
                }else if(imgout[0][j][i]>96-1&&imgout[0][j][i]<112){
                    imgout[0][j][i]=104;
                }else if(imgout[0][j][i]>112-1&&imgout[0][j][i]<128){
                    imgout[0][j][i]=120;
                }else if(imgout[0][j][i]>128-1&&imgout[0][j][i]<144){
                    imgout[0][j][i]=136;
                }else if(imgout[0][j][i]>144-1&&imgout[0][j][i]<160){
                    imgout[0][j][i]=152;
                }else if(imgout[0][j][i]>160-1&&imgout[0][j][i]<176){
                    imgout[0][j][i]=168;
                }else if(imgout[0][j][i]>176-1&&imgout[0][j][i]<192){
                    imgout[0][j][i]=184;
                }else if(imgout[0][j][i]<192-1&&imgout[0][j][i]<200){
                    imgout[0][j][i]=200;
                }else if(imgout[0][j][i]>200-1&&imgout[0][j][i]<208){
                    imgout[0][j][i]=216;
                }else if(imgout[0][j][i]>208-1&&imgout[0][j][i]<224){
                    imgout[0][j][i]=232;
                }else if(imgout[0][j][i]>240){
                    imgout[0][j][i]=248;
                }
            }
        }
    }
    for(int k=1;k<3;k++){//cb,crを固定値に
        for(int i=0;i<height;i++){
            for(int j=0;j<width;j++){
                imgout[k][j][i]=128;
            }
        }
    }
    
}
void replace(void){
    int p[3]={0,0,0};
    printf("コピーモードを入力して下さい\n. (コピーする場合:1,固定値に置き換える場合:0)\n");
    printf("Y:");
    scanf("%d",&p[0]);
    printf("Cb:");
    scanf("%d",&p[1]);
    printf("Cr:");
    scanf("%d",&p[2]);
    
    for(int k=0;k<3;k++){
        for(int i=0;i<height;i++){
            for(int m=0;m<width;m++){
                if(p[k]==1){
                    imgout[k][m][i]=imgin[k][m][i];
                }else{
                    imgout[k][m][i]=128;
                }
            }
        }
    }
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




