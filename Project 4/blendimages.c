#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned int LONG;
struct tagBITMAPFILEHEADER
{
    WORD bfType;  //specifies the file type
    DWORD bfSize;  //specifies the size in bytes of the bitmap file
    WORD bfReserved1;  //reserved; must be 0
    WORD bfReserved2;  //reserved; must be 0
    DWORD bfOffBits;  //species the offset in bytes from 
                        //the bitmapfileheader to
                        //the bitmap bits
};
struct tagBITMAPINFOHEADER
{
    DWORD biSize;  //specifies the number of bytes required by the struct
    LONG biWidth;  //specifies width in pixels
    LONG biHeight;  //species height in pixels
    WORD biPlanes; //specifies the number of color planes, must be 1
    WORD biBitCount; //specifies the number of bit per pixel DWORD biCompression;//spcifies the type of compression
    DWORD biSizeImage; //size of image in bytes
    LONG biXPelsPerMeter; //number of pixels per meter in x axis LONG biYPelsPerMeter; //number of pixels per meter in y axis DWORD biClrUsed; //number of colors used by th ebitmap
    DWORD biClrImportant;  //number of colors that are important
};

void blend(unsigned char *arr1, unsigned char *arr2, float ratio, FILE *dest, LONG width, LONG height)
{
    LONG x;
    LONG y;
    unsigned char resb;
    unsigned char resg;
    unsigned char resr;
    unsigned char result[width*height];
    for(x = 0; x < width; x+=3)
    {
        for(y = 0; y < height; y+=3)
        {
            resb = arr1[x+y]*ratio + arr2[x+y]*(1-ratio);
            result[x+y] = resb;
            resg = arr1[x+1+y+1]*ratio + arr2[x+1+y+1]*(1-ratio);
            result[x+1+y+1] = resg;
            resr = arr1[x+2+y+2]*ratio + arr2[x+2+y+2]*(1-ratio);
            result[x+2+y+2] = resg;
        }
    }
    fwrite(result, sizeof(char), width*height, dest);
}

int main(int argc, char *argv[])
{
    if(argc != 5)
    {
        printf("Please enter coreect command line arguments\n1.) file 1\n2.) file 2\n3.) ratio\n4.) destination");
        return -1;
    }
    FILE *f1ptr;
    FILE *f2ptr;
    float ratio = atof(argv[3]);

    f1ptr = fopen(argv[1], "rb");
    f2ptr = fopen(argv[2], "rb");
    if(f1ptr == NULL || f2ptr == NULL)
    {
        printf("Please enter valid files");
        fclose(f1ptr);
        fclose(f2ptr);
        return -1;
    }
    if(ratio > 1 || ratio <= 0)
    {
        printf("Please enter a valid ratio");
        fclose(f1ptr);
        fclose(f2ptr);
        return -1;
    }
    struct tagBITMAPFILEHEADER *fheader1 = (struct tagBITMAPFILEHEADER*) malloc(sizeof(struct tagBITMAPFILEHEADER));
    struct tagBITMAPFILEHEADER *fheader2 = (struct tagBITMAPFILEHEADER*) malloc(sizeof(struct tagBITMAPFILEHEADER));
    
    fread(fheader1, sizeof(struct tagBITMAPFILEHEADER), 1, f1ptr);
    fread(fheader2, sizeof(struct tagBITMAPFILEHEADER), 1, f2ptr);

    if(fheader1->bfType != "BM" || fheader2->bfType != "BM")
    {
        printf("Please enter valid files");
        fclose(f1ptr);
        fclose(f2ptr);
        return -1;
    }

    struct tagBITMAPINFOHEADER *iheader1 = (struct tagBITMAPINFOHEADER*) malloc(sizeof(struct tagBITMAPINFOHEADER));
    struct tagBITMAPINFOHEADER *iheader2 = (struct tagBITMAPINFOHEADER*) malloc(sizeof(struct tagBITMAPINFOHEADER));

    fread(iheader1, sizeof(struct tagBITMAPINFOHEADER), 1, f1ptr);
    fread(iheader2, sizeof(struct tagBITMAPINFOHEADER), 1, f2ptr);

    if((iheader1->biWidth == iheader2->biWidth) && (iheader1->biHeight == iheader2->biHeight))
    {
        unsigned char *arr1 = (unsigned char*) malloc(sizeof(iheader1->biSizeImage));
        unsigned char *arr2 = (unsigned char*) malloc(sizeof(iheader2->biSizeImage));
        
        fseek(f1ptr, fheader1->bfOffBits, SEEK_SET);
        fseek(f2ptr, fheader2->bfOffBits, SEEK_SET);
        fread(arr1, iheader1->biSizeImage, 1, f1ptr);
        fread(arr2, iheader2->biSizeImage, 1, f2ptr);
        FILE *dest;
        fopen(argv[4], "wb");
        fwrite(fheader1, sizeof(struct tagBITMAPFILEHEADER), 1, dest);
        fwrite(iheader1, sizeof(struct tagBITMAPINFOHEADER), 1, f1ptr);
        fseek(dest, fheader1->bfOffBits, SEEK_SET);
        blend(arr1, arr2, ratio, dest, iheader1->biWidth, iheader1->biHeight);

        fclose(dest);


    }
    
    fclose(f1ptr);
    fclose(f2ptr);
    free(iheader1);
    free(iheader1);
    



}