#include "ImageLoader.h"

#include "cstdio"
#include "cstring"
#include "cstdlib"

bool LoadBMP(const char * path, unsigned char ** data, unsigned int * width, unsigned int * height) {
    FILE * file = fopen(path, "rb");
    if (file == nullptr) {
        printf("Image could not be opened: %s\n", path);
        return false;
    }

    unsigned char header[54];
    if (fread(header, 1, 54, file) != 54) {
        printf("Not a correct BMP file: %s\n", path);
        return false;
    }
    if (header[0] != 'B' || header[1] != 'M') {
        printf("Not a correct BMP file: %s\n", path);
        return false;
    }

    width = (unsigned int *) malloc(sizeof(unsigned int));
    height = (unsigned int *) malloc(sizeof(unsigned int));

    unsigned int dataPos = (unsigned int) *(int *) &(header[0x0A]);
    unsigned int imageSize = (unsigned int) *(int *) &(header[0x22]);
    *width = (unsigned int) *(int *) &(header[0x12]);
    *height = (unsigned int) *(int *) &(header[0x16]);

    if (imageSize == 0) {
        imageSize = *width * *height * 3; // w * h * RGB
    }
    if (dataPos == 0) {
        dataPos = 54;
    }

    data = (unsigned char **) malloc(sizeof(unsigned char *));
    *data = (unsigned char *) malloc(imageSize * sizeof(char));
    fread(data, 1, imageSize, file);
    fclose(file);

    return true;
}

bool LoadDDS(const char * path, unsigned char *& data, unsigned int & width, unsigned int & height, unsigned int & fourCC,
             unsigned int & mipMapCount) {
    FILE * fp = fopen(path, "rb");
    if (fp == NULL) {
        return false;
    }

    char filecode[4];
    fread(filecode, 1, 4, fp);
    if (strncmp(filecode, "DDS ", 4) != 0) {
        fclose(fp);
        return false;
    }

    unsigned char header[124];
    fread(&header, 124, 1, fp);

    height = *(unsigned int *) &(header[8]);
    width = *(unsigned int *) &(header[12]);
    unsigned int linearSize = *(unsigned int *) &(header[16]);
    mipMapCount = *(unsigned int *) &(header[24]);
    fourCC = *(unsigned int *) &(header[80]);

    unsigned int bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;

    data = (unsigned char *) malloc(bufsize * sizeof(unsigned char));


    if (fread(data, 1, bufsize, fp) != bufsize) {
        fclose(fp);
        return false;
    }

    fclose(fp);

    return true;
}