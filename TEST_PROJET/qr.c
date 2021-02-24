#include "qr.h"

void doBasicDemo(const char *email, int idUser,char* PATH) {
    char *value = (char *) malloc(256);
    sprintf(value, "https://51.77.221.39/verif.php?email='%s'", email);
    enum qrcodegen_Ecc errCorLvl = qrcodegen_Ecc_LOW;  // Error correction level

    // Make and print the QR Code symbol
    uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
    uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
    bool ok = qrcodegen_encodeText(value, tempBuffer, qrcode, errCorLvl,
                                   qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
    if (ok)
        printQr(qrcode,PATH);
}

void printQr(const uint8_t qrcode[],char *PATH) {

    int size = qrcodegen_getSize(qrcode);
    int border = 2;
    FILE *fp = fopen(PATH, "wb");
    bitmap *pbitmap = (bitmap *) calloc(1, sizeof(bitmap));
    /*uint8_t*/char *pixelbuffer = (/*uint8_t*/char *) malloc(_pixelbytesize);
    strcpy(pbitmap->fileheader.signature, "BM");
    pbitmap->fileheader.filesize = _filesize;
    pbitmap->fileheader.fileoffset_to_pixelarray = sizeof(bitmap);
    pbitmap->bitmapinfoheader.dibheadersize = sizeof(bitmapinfoheader);
    pbitmap->bitmapinfoheader.width = _width;
    pbitmap->bitmapinfoheader.height = _height;
    pbitmap->bitmapinfoheader.planes = _planes;
    pbitmap->bitmapinfoheader.bitsperpixel = _bitsperpixel;
    pbitmap->bitmapinfoheader.compression = _compression;
    pbitmap->bitmapinfoheader.imagesize = _pixelbytesize;
    pbitmap->bitmapinfoheader.ypixelpermeter = _ypixelpermeter;
    pbitmap->bitmapinfoheader.xpixelpermeter = _xpixelpermeter;
    pbitmap->bitmapinfoheader.numcolorspallette = 0;
    fwrite(pbitmap, 1, sizeof(bitmap), fp);

    char codetest[(size + border * 2) * (size + border * 2)];

    int w = 0;
    for (int y = -border; y < size + border; y++) {
        for (int x = -border; x < size + border; x++) {
            //fputs((qrcodegen_getModule(qrcode, x, y) ? "# " : "  "), stdout);
            if (qrcodegen_getModule(qrcode, x, y)) {
                codetest[w] = 'n';
            } else {
                codetest[w] = 'b';
            }
            w++;
        }
    }
    w = 0;
    long long k = 0;
    for (int i = 0; i < (size + border * 2); i++) {
        for (int m = 0; m < (_height / (size + border * 2)); m++) {
            for (int j = 0; j < (size + border * 2); j++) {
                //printf("%c ",codetest[w]);

                if (codetest[w] == 'n') {
                    for (int n = 0; n < (_height / (size + border * 2)); n++) {
                        pixelbuffer[k] = 0x00;
                        pixelbuffer[k + 1] = 0x00;
                        pixelbuffer[k + 2] = 0x00;

                        k = k + 3;
                    }
                } else {
                    for (int n = 0; n < (_height / (size + border * 2)); n++) {
                        pixelbuffer[k] = 0xFF;
                        pixelbuffer[k + 1] = 0xFF;
                        pixelbuffer[k + 2] = 0xFF;

                        k = k + 3;
                    }
                }
                w++;
            }
            w = (size + border * 2) * i;
        }
        //fputs("\n", stdout);
    }

    fwrite(pixelbuffer, 1, _pixelbytesize, fp);

    fclose(fp);
    free(pbitmap);
    free(pixelbuffer);
}



