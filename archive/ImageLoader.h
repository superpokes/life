#pragma once

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

// Todo: change pointers to references in BMP
bool LoadBMP(const char * path, unsigned char ** data, unsigned int * width, unsigned int * height);
bool LoadDDS(const char * path, unsigned char *& data, unsigned int & width, unsigned int & height, unsigned int & fourCC,
             unsigned int & mipMapCount);


