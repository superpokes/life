#ifndef ERROR_LOGS_H
#define ERROR_LOGS_H

#include "SDL.h"
#include "stdio.h"

void log_sdl_error(const char * source)
{
    printf("%s error: %s\n", source, SDL_GetError());
}

#endif /* end of include guard: ERROR_LOGS_H */
