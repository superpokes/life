#include "GLImageLoader.h"

#include "ImageLoader.h"
#include "cstdlib"

bool GL_LoadBMP(const char * path, GLuint * texture_id) {
    unsigned int width, height;
    unsigned char * data;
    if (!LoadBMP(path, &data, &width, &height)) {
        return false;
    }

    glGenTextures(1, texture_id);

    glBindTexture(GL_TEXTURE_2D, *texture_id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    return true;
}

#include "cstdio"

bool GL_LoadDDS(const char * path, GLuint & texture_id) {
    unsigned int width, height, fourCC, mipMapCount;
    unsigned char * data;
    if (!LoadDDS(path, data, width, height, fourCC, mipMapCount)) {
        return false;
    }

    unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
    unsigned int format;
    switch (fourCC) {
        case FOURCC_DXT1:
            format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            break;
        case FOURCC_DXT3:
            format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            break;
        case FOURCC_DXT5:
            format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            break;
        default:
            free(data);
            return 0;
    }

    glGenTextures(1, &texture_id);

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);



    unsigned int blockSize = format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ? 8 : 16;
    unsigned int offset = 0;

    for (unsigned int level = 0; level <= mipMapCount && (width || height); level++) {

        printf("1\n");
        fflush(stdout);

        unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
                               0, size, data + offset);

        offset += size;
        width /= 2;
        height /= 2;

        if (width < 1) {
            width = 1;
        }
        if (height < 1) {
            height = 1;
        }

    }

    free(data);

    return true;
}