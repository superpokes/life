#pragma once

#include "../Life/glad/glad.h"

// Todo: change pointers to references hear and inside ImageLoader
bool GL_LoadBMP(const char * path, GLuint * texture_id);
bool GL_LoadDDS(const char * path, GLuint & texture_id);