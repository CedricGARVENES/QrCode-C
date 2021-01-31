#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qrcodegen.h"
#include "qrcodegen.c"
#include <stdint.h>
#include <malloc.h>

#define _height (size+border*2)*16
#define _width (size+border*2)*16
#define _bitsperpixel 24
#define _planes 1
#define _compression 0
#define _pixelbytesize _height*_width*_bitsperpixel/8
#define _filesize _pixelbytesize+sizeof(bitmap)
#define _xpixelpermeter 0x130B //2835 , 72 DPI
#define _ypixelpermeter 0x130B //2835 , 72 DPI

#pragma pack(push,1)

typedef struct{
    /*uint8_t*/char  signature[2];
    uint32_t filesize;
    uint32_t reserved;
    uint32_t fileoffset_to_pixelarray;
} fileheader;
typedef struct{
    uint32_t dibheadersize;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bitsperpixel;
    uint32_t compression;
    uint32_t imagesize;
    uint32_t ypixelpermeter;
    uint32_t xpixelpermeter;
    uint32_t numcolorspallette;
    uint32_t mostimpcolor;
} bitmapinfoheader;
typedef struct {
    fileheader fileheader;
    bitmapinfoheader bitmapinfoheader;
} bitmap;
#pragma pack(pop)
//---------------------------------------------------

// Function prototypes
static void doBasicDemo(void);
static void printQr(const uint8_t qrcode[]);


// The main application program.
int main(int argc,char **argv) {
	doBasicDemo();
	return 0;
}

/*---- Demo suite ----*/

// Creates a single QR Code, then prints it to the console.
static void doBasicDemo(void) {
	const char *text = "Issou Cyrille ! Sa va le C ? Demain on fait du Java";                // User-supplied text
	enum qrcodegen_Ecc errCorLvl = qrcodegen_Ecc_LOW;  // Error correction level

	// Make and print the QR Code symbol
	uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
	uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
	bool ok = qrcodegen_encodeText(text, tempBuffer, qrcode, errCorLvl,
		qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
	if (ok)
		printQr(qrcode);
}

/*---- Utilities ----*/

// Prints the given QR Code to the console.
static void printQr(const uint8_t qrcode[]) {

    int size = qrcodegen_getSize(qrcode);
	int border = 2;

    FILE *fp = fopen("QrCode.bmp","wb");
    bitmap *pbitmap  = (bitmap*)calloc(1,sizeof(bitmap));
    /*uint8_t*/char *pixelbuffer = (/*uint8_t*/char*)malloc(_pixelbytesize);
    strcpy(pbitmap->fileheader.signature,"BM");
    pbitmap->fileheader.filesize = _filesize;
    pbitmap->fileheader.fileoffset_to_pixelarray = sizeof(bitmap);
    pbitmap->bitmapinfoheader.dibheadersize =sizeof(bitmapinfoheader);
    pbitmap->bitmapinfoheader.width = _width;
    pbitmap->bitmapinfoheader.height = _height;
    pbitmap->bitmapinfoheader.planes = _planes;
    pbitmap->bitmapinfoheader.bitsperpixel = _bitsperpixel;
    pbitmap->bitmapinfoheader.compression = _compression;
    pbitmap->bitmapinfoheader.imagesize = _pixelbytesize;
    pbitmap->bitmapinfoheader.ypixelpermeter = _ypixelpermeter ;
    pbitmap->bitmapinfoheader.xpixelpermeter = _xpixelpermeter ;
    pbitmap->bitmapinfoheader.numcolorspallette = 0;
    fwrite (pbitmap, 1, sizeof(bitmap),fp);

	char codetest[(size+border*2)*(size+border*2)];

    int w=0;
	for (int y = -border; y < size + border; y++) {
		for (int x = -border; x < size + border; x++) {
            if(qrcodegen_getModule(qrcode, x, y)){
                codetest[w]='n';
            } else {
                codetest[w]='b';
            }
            w++;
		}
	}

    w=0;
	long long k=0;
	for (int i=0; i<(size+border*2); i++) {
        for(int m=0; m<(_height/(size+border*2)); m++) {
            for (int j=0; j<(size+border*2); j++) {

                if(codetest[w]=='n'){
                    for(int n=0; n<(_height/(size+border*2)); n++) {
                        pixelbuffer[k]=0x00;
                        pixelbuffer[k+1]=0x00;
                        pixelbuffer[k+2]=0x00;
                        k=k+3;
                    }
                } else {
                    for(int n=0; n<(_height/(size+border*2)); n++) {
                        pixelbuffer[k]=0xFF;
                        pixelbuffer[k+1]=0xFF;
                        pixelbuffer[k+2]=0xFF;
                        k=k+3;
                    }
                }
                w++;
            }
            w=(size+border*2)*i;
		}
	}

    fwrite(pixelbuffer,1,_pixelbytesize,fp);

    fclose(fp);
    free(pbitmap);
    free(pixelbuffer);
}
