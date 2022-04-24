#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <math.h>
enum boolean
{
    false = 0,
    true = 1
};
typedef enum boolean bool;

bool IsNKT (char*);
struct Nokta
{
    float x;
    float y;
    float z;
    int r;
    int g;
    int b;
};
struct NoktaRGBsiz
{
    float x;
    float y;
    float z;
};
typedef struct Nokta Nokta;
typedef struct NoktaRGBsiz NoktaRGBsiz;

struct Dosya
{
    char dosyaIsmi[50];
    char outputPath[50];
    char veri[150000][400];
    Nokta noktalar[200000];
    int satirSayisi;
    int noktaSayisi;
    int belirtilenNoktaSayisi;
    int versiyon;
    bool rgbMi;
    char tip[10];
    bool gecerliMi;
    bool binaryMi;

};

typedef struct Dosya Dosya;
void DosyalariKontrolEt();
void EnYakinveEnUzakNoktalar();
void Kup();
void Kure();
void NoktaUzakliklari();

void DosyaOku (char* yol);
void KlasordekiDosyalariSil();
int NoktaKontrol();
int GetDataTip();
int GetRGBmi();
bool ValidateVersiyon();
void ValidateAlanlar();
bool ValidateBelirtilenNoktaSayisi();
bool ValidateDataTip();
bool ValidateNokta(int,int);
bool ValidateNoktaRGB(int,int);
void ValidateNoktaSayisi();
void DosyaIceriginiAl();

char output1[50000];
char output2[50000];
FILE *dosya;
Dosya Dosyalar[20];

int dosyaSayisi=0;
int index;
char hataMesaji[100];
char trimvalue[100];
FILE *fp;
int main()
{
    int secim;
    KlasordekiDosyalariSil();
    do
    {
        printf("1-Dosya Kontrolu\n");
        printf("2-En Yakin/Uzak Noktalar\n");
        printf("3-Kup\n");
        printf("4-Kure\n");
        printf("5-Nokta Uzakliklari\n");
        printf("Seciminizi Giriniz:");
        scanf("%d", &secim);
        switch (secim)
        {
        case 1:
            DosyalariKontrolEt();
            break;
        case 2:
            EnYakinveEnUzakNoktalar();
            break;
        case 3:
            Kup();
            break;
        case 4:
            Kure();
            break;
        case 5:
            NoktaUzakliklari();
            exit(1);
            break;

        default:
            break;
        }
    }
    while (1);
    return 0;
}
void KlasordekiDosyalariSil()
{

    char fullpath[40];
    DIR *dirp;
    struct dirent *directory;
    // Directory target
    dirp = opendir("./output");
    if(dirp == NULL)
    {
        puts("Klasor acilamadi");
    }
    if(dirp)
    {

        while((directory=readdir(dirp))!=NULL)
        {
            strcpy(fullpath, "");
            strcat(fullpath, "./output/");
            strcat(fullpath, directory->d_name);
            remove(fullpath);
            strcpy(fullpath, "");
        }
        closedir(dirp);
    }
}

void DosyalariKontrolEt()
{
    index=0;
    dosyaSayisi=0;
    DIR *dirp;
    char outputPath[40];
    struct dirent *directory;
    // Directory target
    dirp = opendir(".");
    if(dirp == NULL)
    {
        puts("Klasor acilamadi");
    }
    if(dirp)
    {
        while((directory=readdir(dirp))!=NULL)
        {
            if(IsNKT(directory->d_name))  //nkt dosyasý olup olmadýðý kontrol ediliyor
            {
                strcpy(outputPath, "");
                strcat(outputPath, "./output/");
                strcat(outputPath, "output_");
                strcat(outputPath, directory->d_name);
                strcpy(Dosyalar[index].outputPath,outputPath);
                strcpy(outputPath, "");

                strcpy(Dosyalar[index].dosyaIsmi,directory->d_name);
                DosyaOku(directory->d_name);

                fp=fopen(Dosyalar[index].outputPath, "w+");
                DosyaIceriginiAl();
                fclose(fp);
                index++;
            }
        }
        closedir(dirp);
        dosyaSayisi=index;
        index=0;
    }


}
void EnYakinveEnUzakNoktalar()
{
    float buyukUzaklik;
    float kucukUzaklik;
    for(index=0; index<dosyaSayisi; index++)
    {
        if(!Dosyalar[index].gecerliMi)
            continue;

        if(Dosyalar[index].binaryMi!=true)
        {
            fp=fopen(Dosyalar[index].outputPath, "a");
        }
        else
        {
            fp=fopen(Dosyalar[index].outputPath, "ab");
        }

        fprintf(fp,"\nSECIM 2");


        printf("\n------Dosya ismi %s-------", Dosyalar[index].dosyaIsmi);

        kucukUzaklik= sqrt(pow(Dosyalar[index].noktalar[0].x-Dosyalar[index].noktalar[1].x, 2)
                           +pow(Dosyalar[index].noktalar[0].y-Dosyalar[index].noktalar[1].y, 2)
                           +pow(Dosyalar[index].noktalar[0].z-Dosyalar[index].noktalar[1].z, 2));
        int k, j, a=0, b=1;
        for(k=0; k<Dosyalar[index].noktaSayisi; k++)
        {
            for(j=k+1; j<Dosyalar[index].noktaSayisi; j++)
            {
                if(
                    sqrt(pow(Dosyalar[index].noktalar[k].x-Dosyalar[index].noktalar[j].x, 2)
                         +pow(Dosyalar[index].noktalar[k].y-Dosyalar[index].noktalar[j].y, 2)
                         +pow(Dosyalar[index].noktalar[k].z-Dosyalar[index].noktalar[j].z, 2))
                    <kucukUzaklik)
                {
                    kucukUzaklik= sqrt(pow(Dosyalar[index].noktalar[k].x-Dosyalar[index].noktalar[j].x, 2)
                                       +pow(Dosyalar[index].noktalar[k].y-Dosyalar[index].noktalar[j].y, 2)
                                       +pow(Dosyalar[index].noktalar[k].z-Dosyalar[index].noktalar[j].z, 2));

                    a=k;
                    b=j;

                }

            }
        }
        buyukUzaklik= sqrt(pow(Dosyalar[index].noktalar[0].x-Dosyalar[index].noktalar[1].x, 2)
                           +pow(Dosyalar[index].noktalar[0].y-Dosyalar[index].noktalar[1].y, 2)
                           +pow(Dosyalar[index].noktalar[0].z-Dosyalar[index].noktalar[1].z, 2));
        int c=0;
        int d=1;
        for(k=0; k<Dosyalar[index].noktaSayisi; k++)
        {
            for(j=k+1; j<Dosyalar[index].noktaSayisi; j++)
            {
                if(
                    sqrt(pow(Dosyalar[index].noktalar[k].x-Dosyalar[index].noktalar[j].x, 2)
                         +pow(Dosyalar[index].noktalar[k].y-Dosyalar[index].noktalar[j].y, 2)
                         +pow(Dosyalar[index].noktalar[k].z-Dosyalar[index].noktalar[j].z, 2))
                    >buyukUzaklik)
                {
                    buyukUzaklik= sqrt(pow(Dosyalar[index].noktalar[k].x-Dosyalar[index].noktalar[j].x, 2)
                                       +pow(Dosyalar[index].noktalar[k].y-Dosyalar[index].noktalar[j].y, 2)
                                       +pow(Dosyalar[index].noktalar[k].z-Dosyalar[index].noktalar[j].z, 2));

                    c=k;
                    d=j;

                }

            }
        }

        printf("\nBirbirine en yakin noktalar\n%d. nokta:(%s) ve %d. nokta:(%s)\n",a+1,Dosyalar[index].veri[a+4],
               b+1,Dosyalar[index].veri[b+4]);

        printf("\nBirbirine en uzak noktalar\n%d. nokta:(%s) ve %d. nokta:(%s)\n",c+1,Dosyalar[index].veri[c+4],
               d+1,Dosyalar[index].veri[d+4]);




        if(Dosyalar[index].binaryMi!=true)
        {
            char yazi[100];
            strcpy(yazi,"");
            sprintf(yazi,"\n%s\n%s",  Dosyalar[index].veri[a+4],Dosyalar[index].veri[b+4]);
            fprintf(fp,yazi);

            sprintf(yazi,"\n%s\n%s", Dosyalar[index].veri[c+4],Dosyalar[index].veri[d+4]);
            fprintf(fp,yazi);
        }
        else
        {
            fprintf(fp,"\n");
            if(Dosyalar[index].rgbMi!=true)
            {
                fwrite(&Dosyalar[index].noktalar[a], sizeof(struct NoktaRGBsiz), 1, fp);
                fwrite(&Dosyalar[index].noktalar[b], sizeof(struct NoktaRGBsiz), 1, fp);
            }
            else
            {
                fwrite(&Dosyalar[index].noktalar[c], sizeof(struct Nokta), 1, fp);
                fwrite(&Dosyalar[index].noktalar[d], sizeof(struct Nokta), 1, fp);
            }
        }



        fclose(fp);
    }
}
void Kup()
{

    for(index=0; index<dosyaSayisi; index++)
    {
        if(!Dosyalar[index].gecerliMi)
            continue;

        if(Dosyalar[index].binaryMi!=true)
        {
            fp=fopen(Dosyalar[index].outputPath, "a");
        }
        else
        {
            fp=fopen(Dosyalar[index].outputPath, "ab");
        }
        fprintf(fp,"\nSECIM 3");
        printf("\n------Dosya ismi %s-------", Dosyalar[index].dosyaIsmi);

        float z_max=Dosyalar[index].noktalar[0].z;
        float z_min=Dosyalar[index].noktalar[0].z;
        float y_max=Dosyalar[index].noktalar[0].y;
        float y_min=Dosyalar[index].noktalar[0].y;
        float x_max=Dosyalar[index].noktalar[0].x;
        float x_min=Dosyalar[index].noktalar[0].x;
        int i, j;

        for(i=0; i<Dosyalar[index].noktaSayisi; i++)
        {
            if(Dosyalar[index].noktalar[i].z>z_max)
                z_max=Dosyalar[index].noktalar[i].z;
            if(Dosyalar[index].noktalar[i].y>y_max)
                y_max=Dosyalar[index].noktalar[i].y;
            if(Dosyalar[index].noktalar[i].x>x_max)
                x_max=Dosyalar[index].noktalar[i].x;
            if(Dosyalar[index].noktalar[i].z<z_min)
                z_min=Dosyalar[index].noktalar[i].z;
            if(Dosyalar[index].noktalar[i].y<y_min)
                y_min=Dosyalar[index].noktalar[i].y;
            if(Dosyalar[index].noktalar[i].x<x_min)
                x_min=Dosyalar[index].noktalar[i].x;
        }
        float z_boy=z_max-z_min;
        float y_boy=y_max-y_min;
        float x_boy=x_max-x_min;
        float dizi[2];
        float kenar=z_boy;
        dizi[0]=z_boy;
        dizi[1]=y_boy;
        dizi[2]=x_boy;

        for(i=0; i<3; i++)
        {
            for(j=i+1; j<3; j++)
            {
                if(dizi[j]>dizi[i])
                    kenar=dizi[j];
            }
        }

        float z_boyFark=kenar-z_boy;
        float y_boyFark=kenar-y_boy;
        float x_boyFark=kenar-x_boy;


        printf("\n1.KOSE=> X: %f | Y: %f | Z: %f", x_min, y_max+y_boyFark, z_min);
        printf("\n2.KOSE=> X: %f | Y: %f | Z: %f", x_min, y_max+y_boyFark, z_max+z_boyFark);
        printf("\n3.KOSE=> X: %f | Y: %f | Z: %f", x_max+x_boyFark, y_max, z_max+z_boyFark);
        printf("\n4.KOSE=> X: %f | Y: %f | Z: %f", x_max+x_boyFark, y_max, z_min);
        printf("\n5.KOSE=> X: %f | Y: %f | Z: %f", x_min, y_min, z_max+z_boyFark);
        printf("\n6.KOSE=> X: %f | Y: %f | Z: %f", x_min, y_min, z_min);
        printf("\n7.KOSE=> X: %f | Y: %f | Z: %f", x_max+x_boyFark, y_min, z_max+z_boyFark);
        printf("\n8.KOSE=> X: %f | Y: %f | Z: %f\n\n", x_max+x_boyFark, y_min, z_min);




        if(Dosyalar[index].binaryMi!=true)
        {
            char yazi[100];
            sprintf(yazi,"\n%f %f %f", x_min, y_max+y_boyFark, z_min);
            fprintf(fp,yazi);
            sprintf(yazi,"\n%f %f %f", x_min, y_max+y_boyFark, z_max+z_boyFark);
            fprintf(fp,yazi);
            sprintf(yazi,"\n%f %f %f", x_max+x_boyFark, y_max, z_max+z_boyFark);
            fprintf(fp,yazi);
            sprintf(yazi,"\n%f %f %f", x_max+x_boyFark, y_max, z_min);
            fprintf(fp,yazi);
            sprintf(yazi,"\n%f %f %f", x_min, y_min, z_max+z_boyFark);
            fprintf(fp,yazi);
            sprintf(yazi,"\n%f %f %f", x_min, y_min, z_min);
            fprintf(fp,yazi);
            sprintf(yazi,"\n%f %f %f", x_max+x_boyFark, y_min, z_max+z_boyFark);
            fprintf(fp,yazi);
            sprintf(yazi,"\n%f %f %f", x_max+x_boyFark, y_min, z_min);
            fprintf(fp,yazi);
        }
        else
        {
            fprintf(fp,"\n");
            NoktaRGBsiz n[8];
            n[0].x=x_min;
            n[0].y=y_max+y_boyFark;
            n[0].z=z_min;
            n[1].x=x_min;
            n[1].y=y_max+y_boyFark;
            n[1].z=z_max+z_boyFark;
            n[2].x=x_max+x_boyFark;
            n[2].y=y_max;
            n[2].z=z_max+z_boyFark;
            n[3].x=x_max+x_boyFark;
            n[3].y=y_max;
            n[3].z=z_min;
            n[4].x=x_min;
            n[4].y=y_min;
            n[4].z=z_max+z_boyFark;
            n[5].x=x_min;
            n[5].y=y_min;
            n[5].z=z_min;
            n[6].x=x_max+x_boyFark;
            n[6].y=y_min;
            n[6].z=z_max+z_boyFark;
            n[7].x=x_max+x_boyFark;
            n[7].y=y_min;
            n[7].z=z_min;
            fwrite(&n[0], sizeof(struct Nokta), 1, fp);
            fwrite(&n[1], sizeof(struct Nokta), 1, fp);
            fwrite(&n[2], sizeof(struct Nokta), 1, fp);
            fwrite(&n[3], sizeof(struct Nokta), 1, fp);
            fwrite(&n[4], sizeof(struct Nokta), 1, fp);
            fwrite(&n[5], sizeof(struct Nokta), 1, fp);
            fwrite(&n[6], sizeof(struct Nokta), 1, fp);
            fwrite(&n[7], sizeof(struct Nokta), 1, fp);

        }



        fclose(fp);
    }

}
void Kure()
{
    float x, y, z, r;
    int q;
    printf("kure merkezinin x koordinatini giriniz: ");
    scanf("%f", &x);
    printf("kure merkezinin y koordinatini giriniz: ");
    scanf("%f", &y);
    printf("kure merkezinin z koordinatini giriniz: ");
    scanf("%f", &z);
    printf("kurenin yaricapini giriniz: ");
    scanf("%f", &r);
    for(index=0; index<dosyaSayisi; index++)
    {
        if(!Dosyalar[index].gecerliMi)
            continue;

        if(Dosyalar[index].binaryMi!=true)
        {
            fp=fopen(Dosyalar[index].outputPath, "a");
        }
        else
        {
            fp=fopen(Dosyalar[index].outputPath, "ab");
        }
        fprintf(fp,"\nSECIM 4");
        printf("\n------Dosya ismi %s-------", Dosyalar[index].dosyaIsmi);
        printf("\nKurenin icinde kalan noktalar: ");
        fprintf(fp,"\ncx=%f \ncy=%f \ncz=%f \ncr=%f", x,y,z,r);
        for(q=0; q<Dosyalar[index].noktaSayisi; q++)
        {
            if(sqrt(pow(Dosyalar[index].noktalar[q].x-x, 2)
                    +pow(Dosyalar[index].noktalar[q].y-y, 2)
                    +pow(Dosyalar[index].noktalar[q].z-z, 2)) < r)
            {
                printf("\n%d. Nokta: %s", q+1, Dosyalar[index].veri[q+4]);




                if(Dosyalar[index].binaryMi!=true)
                {
                    fprintf(fp,"\n%s", Dosyalar[index].veri[q+4]);
                }
                else
                {
                    fprintf(fp,"\n");
                    if(Dosyalar[index].rgbMi!=true)
                    {
                        fwrite(&Dosyalar[index].noktalar[q], sizeof(struct NoktaRGBsiz), 1, fp);
                    }
                    else
                    {
                        fwrite(&Dosyalar[index].noktalar[q], sizeof(struct Nokta), 1, fp);
                    }
                }






            }
        }
        printf("\n");

        fclose(fp);
    }
    printf("\n");
}
void NoktaUzakliklari()
{
    int k, j;
    float kombinasyon=0.0f;
    float toplam;
    float uzaklik;
    float ortalama;
    for(index=0; index<dosyaSayisi; index++)
    {
        if(!Dosyalar[index].gecerliMi)
            continue;

        if(Dosyalar[index].binaryMi!=true)
        {
            fp=fopen(Dosyalar[index].outputPath, "a");
        }
        else
        {
            fp=fopen(Dosyalar[index].outputPath, "ab");
        }
        printf("\n------Dosya ismi %s-------", Dosyalar[index].dosyaIsmi);
        kombinasyon=0;
        toplam=0;

        for(k=0; k<Dosyalar[index].noktaSayisi; k++)
        {
            for(j=k+1; j<Dosyalar[index].noktaSayisi; j++)
            {

                uzaklik = sqrt(
                               (Dosyalar[index].noktalar[k].x-Dosyalar[index].noktalar[j].x)*(Dosyalar[index].noktalar[k].x-Dosyalar[index].noktalar[j].x)
                               +(Dosyalar[index].noktalar[k].y-Dosyalar[index].noktalar[j].y)*(Dosyalar[index].noktalar[k].y-Dosyalar[index].noktalar[j].y)
                               +(Dosyalar[index].noktalar[k].z-Dosyalar[index].noktalar[j].z)*(Dosyalar[index].noktalar[k].z-Dosyalar[index].noktalar[j].z));
                 kombinasyon+=1.0f;
                if(toplam<0)
                    continue;
                toplam =toplam+ uzaklik;

                uzaklik=0;


            }

        }

        ortalama=(float)toplam/kombinasyon;
        printf("\nHer bir noktanin birbirine olan uzakliklarinin ortalamasi: %f\n", ortalama);



        if(Dosyalar[index].binaryMi!=true)
        {
            fprintf(fp,"\nSECIM 5\n%f\n", ortalama);
        }
        else
        {
            fprintf(fp,"\nSECIM 5");

            fprintf(fp,"\n");
            fwrite(&ortalama, sizeof(ortalama), 1, fp);
        }



        fclose(fp);
    }
}
bool IsNKT (char* dosyaIsmi)
{
    char gecici[200];
    int uzunluk = strlen(dosyaIsmi);
    int i;
    for(i=0; i<4; i++)
    {
        gecici[i]=dosyaIsmi[uzunluk-4+i];
    }
    if( gecici[0]=='.' && gecici[1]=='n' && gecici[2]=='k' && gecici[3]=='t' )
        return true;
    else
        return false;
}

void GetDataLine(char* yol)
{
    dosya = fopen(yol, "r");
    if (dosya == NULL)
    {
        printf("\nnull");
        fclose(dosya);
    }
    else
    {
        char line[500];
        int status;
        int i = 0;
        while (status != EOF)
        {
            if (fgets(line, sizeof (line), dosya))
            {

                status = fscanf(dosya,"%[^\n]", Dosyalar[index].veri[i]);
                i++;
                if(i==5)
                    break;
            }
        }
        fclose(dosya);
    }

}
void DosyaOku (char* yol)
{
    GetDataLine(yol);
    int deger=GetDataTip();
    if(deger==1)
    {

        dosya = fopen(yol, "r");
        if (dosya == NULL)
        {
            printf("\nnull");
            fclose(dosya);
        }
        else
        {
            char line[200];
            int status;
            int i = 0;
            while (status != EOF)
            {
                if (fgets(line, sizeof (line), dosya))
                {

                    status = fscanf(dosya,"%[^\n]", Dosyalar[index].veri[i]);
                    i++;
                }
            }
            Dosyalar[index].satirSayisi=i-1;
            fclose(dosya);
        }
    }
    else if(deger==2)
    {
        dosya = fopen(yol,"rb");
        if (dosya == NULL)
        {
            printf("\nnull");
            fclose(dosya);
        }
        else
        {
            struct Nokta s;
            struct NoktaRGBsiz sRGBsiz;

            char line[200];
            int status;
            int i = 0;
            float x,y,z;
            int r,g,b;

            while (fgets(line,200, dosya))
            {

                if(GetRGBmi()==1)
                {
                    sscanf(line,"%f %f %f",&x,&y,&z);
                }
                else if(GetRGBmi()==2)
                {
                    sscanf(line,"%f %f %f %d %d %d",&x,&y,&z,&r,&g,&b);
                }

                if(i==4)
                {
                    if(GetRGBmi()==1)
                    {
                        while (1)
                        {
                            size_t sayi = fread(&sRGBsiz, sizeof(struct NoktaRGBsiz), 1, dosya);
                            if (sayi < 1)
                                break;   // okunamadiysa (dosya bitmisse vb.) donguyu durdur
                            sprintf(Dosyalar[index].veri[i],"%f %f %f", sRGBsiz.x,sRGBsiz.y,sRGBsiz.z);

                            // printf("%f %f %f\n", sRGBsiz.x,sRGBsiz.y,sRGBsiz.z);
                            i++;
                        }
                    }
                    else if(GetRGBmi()==2)
                    {
                        while (1)
                        {
                            size_t sayi = fread(&s, sizeof(struct Nokta), 1, dosya);
                            if (sayi < 1)
                                break;   // okunamadiysa (dosya bitmisse vb.) donguyu durdur

                            sprintf(Dosyalar[index].veri[i],"%f %f %f %d %d %d", s.x,s.y,s.z,s.r,s.g,s.b);
                            //printf("%f %f %f %d %d %d\n", s.x,s.y,s.z,s.r,s.g,s.b);

                            i++;
                        }
                    }
                }
                i++;
            }
            //}
            Dosyalar[index].satirSayisi=i-1;
            fclose(dosya);
        }
    }
}



void NoktalariOku()
{
    int i=4;
    int j=0;

    for(i=4; i<Dosyalar[index].satirSayisi; i++)
    {
        if(ValidateNokta(i,j))
        {
            sscanf(Dosyalar[index].veri[i], "%f %f %f", &Dosyalar[index].noktalar[j].x, &Dosyalar[index].noktalar[j].y, &Dosyalar[index].noktalar[j].z);
        }
        j++;

    }
    Dosyalar[index].noktaSayisi=j;

    if(!strcmp(Dosyalar[index].veri[i-1],"") )
    {
        Dosyalar[index].noktaSayisi=j-1;
    }
}
void NoktalariOkuRGB()
{
    int i=4;
    int j=0;
    for(i=4; i<Dosyalar[index].satirSayisi; i++)
    {
        if(ValidateNoktaRGB(i,j))
        {
            sscanf(Dosyalar[index].veri[i], "%f %f %f %d %d %d", &Dosyalar[index].noktalar[j].x, &Dosyalar[index].noktalar[j].y,
                   &Dosyalar[index].noktalar[j].z, &Dosyalar[index].noktalar[j].r, &Dosyalar[index].noktalar[j].g,
                   &Dosyalar[index].noktalar[j].b);

        }
        j++;
    }
    Dosyalar[index].noktaSayisi=j;

    if(!strcmp(Dosyalar[index].veri[i-1],"") )
    {
        Dosyalar[index].noktaSayisi=j-1;
    }
}
void DosyaIceriginiAl()
{
    Dosyalar[index].gecerliMi=true;
    printf("\n\n------Dosya ismi %s-------", Dosyalar[index].dosyaIsmi);

    fprintf(fp, "SECIM 1");

    char c[100];
    if( ValidateVersiyon())
    {
        sscanf(Dosyalar[index].veri[0], "%s %d", c, &Dosyalar[index].versiyon);
    }

    if( ValidateBelirtilenNoktaSayisi())
    {
        sscanf(Dosyalar[index].veri[2], "%s %d", c, &Dosyalar[index].belirtilenNoktaSayisi);
    }
    if( ValidateDataTip())
    {
        sscanf(Dosyalar[index].veri[3], "%s %s", c, Dosyalar[index].tip);
    }
    Dosyalar[index].noktaSayisi=Dosyalar[index].satirSayisi-4;

    ValidateAlanlar();

    if(!strcmp(Dosyalar[index].veri[1],"ALANLAR x y z"))
    {
        Dosyalar[index].rgbMi=false;
        NoktalariOku();
    }
    else if(!strcmp(Dosyalar[index].veri[1],"ALANLAR x y z r g b"))
    {
        Dosyalar[index].rgbMi=true;
        NoktalariOkuRGB();
    }


    ValidateNoktaSayisi();
    if(Dosyalar[index].gecerliMi==true)
    {
        printf("\nDosyadaki tum veriler uyumludur.");
        fprintf(fp, "\nDosyadaki tum veriler uyumludur.");
    }
    printf("\n\n");
}

int isnumeric( char *str )
{
    while(*str)
    {
        if(!isdigit(*str))
            return 0;
        str++;
    }
    return 1;
}
int isDouble(char *buffer)
{
    int i=0,dec_pt=0;
    for(i=0, dec_pt=0;  i < 50 && buffer[i] != '\0'; ++i)
        if( buffer[i] == '.')
            ++dec_pt;
        else if( !isdigit( (int) buffer[i]) )
            return 0;

    return 1;
}

bool ValidateVersiyon()
{
    char yazi[100];
    char sayi[100];
    sscanf(Dosyalar[index].veri[0], "%s %s", yazi, sayi);
    if(!strcmp(yazi,"VERSION")&& isnumeric(sayi))
    {
        return true;
    }
    else
    {
        Dosyalar[index].gecerliMi=false;
        strcpy( hataMesaji,"\nVERSION satiri hatali verilmistir");
        printf(hataMesaji);
        fprintf(fp, hataMesaji);
        return false;
    }
}
bool ValidateBelirtilenNoktaSayisi()
{
    char yazi[100];
    char sayi[100];
    sscanf(Dosyalar[index].veri[2], "%s %s", yazi, sayi);
    if(!strcmp(yazi,"NOKTALAR")&& isnumeric(sayi))
    {
        return true;
    }
    else
    {
        Dosyalar[index].gecerliMi=false;
        strcpy( hataMesaji,"\nNOKTALAR satiri hatali verilmistir");
        printf(hataMesaji);
        fprintf(fp, hataMesaji);
        return false;
    }
}
bool ValidateDataTip()
{
    char yazi[100];
    char tip[100];
    sscanf(Dosyalar[index].veri[3], "%s %s", yazi, tip);
    if(!strcmp(yazi,"DATA"))
    {
        if (!strcmp(tip,"ascii")|| !strcmp(tip,"binary"))
        {
            return true;
        }
        else
        {

            return true;
        }
    }
    else
    {
        Dosyalar[index].gecerliMi=false;
        strcpy( hataMesaji,"\nDATA tipi(ascii,binary) satiri hatali verilmistir");
        printf(hataMesaji);
        fprintf(fp, hataMesaji);
        return false;
    }
}
void ValidateAlanlar()
{
    if(!strcmp(Dosyalar[index].veri[1],"ALANLAR x y z")|| !strcmp(Dosyalar[index].veri[1],"ALANLAR x y z r g b") )
    {
        return;
    }
    else
    {
        Dosyalar[index].gecerliMi=false;
        strcpy( hataMesaji,"\nALANLAR satirinda nokta formati hatali verilmistir");
        printf(hataMesaji);
        fprintf(fp, hataMesaji);
        return ;
    }
    return ;
}
bool ValidateNokta(int i,int j)
{
    char x[100];
    char y[100];
    char z[100];
    sscanf(Dosyalar[index].veri[i], "%s %s %s", x,y,z);
    if(strcmp( x,"") && strcmp( y,"")&& strcmp( z,"")&& isDouble(x)&&isDouble(y)&&isDouble(z))
    {
        return true;
    }
    else
    {

        Dosyalar[index].gecerliMi=false;
        char str [50];
        sprintf(str,"\n%d%s (%d. satir)",j+1,". nokta verisi formati yanlistir",i+2);
        strcpy( hataMesaji,str);
        printf(hataMesaji);
        fprintf(fp, hataMesaji);
        return false;
    }
}
bool ValidateNoktaRGB(int i, int j)
{
    char x[100];
    char y[100];
    char z[100];
    char r[100];
    char g[100];
    char b[100];
    sscanf(Dosyalar[index].veri[i], "%s %s %s %s %s %s", x,y,z,r,g,b);

    if(isDouble(x)&&isDouble(y)&&isDouble(z))
    {
        if(isnumeric(r)&&isnumeric(g)&&isnumeric(b))
        {
            return true;
        }
        else
        {
            Dosyalar[index].gecerliMi=false;
            char str [50];
            sprintf(str,"\n%d%s (%d. satir)",j+1,". nokta verisi rgb bilgisi eksiktir.",i+2);
            strcpy( hataMesaji,str);
            printf(hataMesaji);
            fprintf(fp, hataMesaji);
            return false;
        }

    }
    else
    {
        Dosyalar[index].gecerliMi=false;
        char str [50];
        sprintf(str,"\n%d%s (%d. satir)",j,". nokta verisi formati yanlistir",i+2);
        strcpy( hataMesaji,str);
        printf(hataMesaji);
        fprintf(fp, hataMesaji);
        return false;
    }
}
void ValidateNoktaSayisi()
{
    if(Dosyalar[index].belirtilenNoktaSayisi==Dosyalar[index].noktaSayisi)
    {
        return ;
    }
    else
    {
        Dosyalar[index].gecerliMi=false;

        char str [100];
        sprintf(str,"\nDosyadaki nokta sayisi gecerli degildir. Gercek nokta sayisi: %d",Dosyalar[index].noktaSayisi);
        strcpy( hataMesaji,str);
        printf(hataMesaji);
        fprintf(fp, hataMesaji);
        return ;
    }
}
int GetDataTip()
{
    char yazi[100];
    char tip[100];
    sscanf(Dosyalar[index].veri[3], "%s %s", yazi, tip);
    if(!strcmp(yazi,"DATA"))
    {
        if (!strcmp(tip,"ascii"))
        {
            Dosyalar[index].binaryMi=false;
            return 1;
        }
        else if (!strcmp(tip,"binary"))
        {
            Dosyalar[index].binaryMi=true;
            return 2;
        }
    }
    return -1;
}

int GetRGBmi()
{
    if(!strcmp(Dosyalar[index].veri[1],"ALANLAR x y z") )
    {
        return 1;
    }
    if(!strcmp(Dosyalar[index].veri[1],"ALANLAR x y z r g b") )
    {
        return 2;
    }
    else
    {
        return -1;
    }
}
