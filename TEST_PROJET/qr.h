#ifndef QR_H_INCLUDED
#define QR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>
#include <MYSQL/mysql.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <malloc.h>
#include <time.h>
#include "qrcodegen.h"
#include <gtk/gtk.h>

#pragma pack(push, 1)

#define _height (size+border*2)*16
#define _width (size+border*2)*16
#define _bitsperpixel 24
#define _planes 1
#define _compression 0
#define _pixelbytesize _height*_width*_bitsperpixel/8
#define _filesize _pixelbytesize+sizeof(bitmap)
#define _xpixelpermeter 0x130B //2835 , 72 DPI
#define _ypixelpermeter 0x130B //2835 , 72 DPI

typedef struct {
    /*uint8_t*/char signature[2];
    uint32_t filesize;
    uint32_t reserved;
    uint32_t fileoffset_to_pixelarray;
} fileheader;

typedef struct {
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

void doBasicDemo(const char *mail, int id, char* PATH);
void printQr(const uint8_t qrcode[], char* PATH);

#endif // QR_H_INCLUDED
