#include "stdio.h"
#include "SDL.h"

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    printf("Todo correcto\n");

    SDL_Quit();
    return 0;
}
