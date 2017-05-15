#include <cstdio>

#include <SDL2/SDL.h>

#include "World.h"
#include "Grid.h"

// https://www.libsdl.org/release/SDL-1.2.15/docs/html/guidevideoopengl.html
// http://lazyfoo.net/tutorials/SDL/50_SDL_and_opengl_2/
// http://lazyfoo.net/tutorials/SDL/51_SDL_and_modern_opengl/index.php
// http://lazyfoo.net/tutorials/OpenGL/index.php
// https://www.khronos.org/opengl/wiki/Array_Texture
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/
// https://bcmpinc.wordpress.com/2015/08/18/creating-an-opengl-4-5-context-using-sdl2-and-glad/


// Todo: Use EXT_Texture_Array
// Todo: Check tutorial particle thing, there might be something cool for rendering a lot of the same mesh
// Todo: Implement camera movement and increase world size
// Todo: Pause function
// Todo: Timescale control
// Todo: Show controls ingame
// Todo: Port project to Linux

// global declarations, prefixed with g_
SDL_Window * g_main_window;
World * g_world;
bool g_running;
bool g_quit;

// starts SDL and OpenGL
bool InitEnvironment()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return false;
	}

	// Create SDL window
	g_main_window = SDL_CreateWindow("Life", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (g_main_window == NULL) {
		printf("SDL_CreateWindow error: %s\n", SDL_GetError());
		SDL_Quit();
		return false;
	}

    // Set widow icon
    SDL_Surface * icon = SDL_LoadBMP("assets/icon.bmp");
    SDL_SetWindowIcon(g_main_window, icon);
    SDL_FreeSurface(icon);

	// Create an OpenGL context attached to the window
	SDL_GLContext GL_context = SDL_GL_CreateContext(g_main_window);
	if (GL_context == NULL) {
		printf("SDL_GL_CreateContext error: %s\n", SDL_GetError());
		SDL_DestroyWindow(g_main_window);
		SDL_Quit();
		return false;
	}

	// Enable VSync
	if (SDL_GL_SetSwapInterval(1) != 0) {
		printf("Error: can't enable vsync, SDL: %s\n", SDL_GetError());
		SDL_DestroyWindow(g_main_window);
		SDL_Quit();
		return false;
	}

	// Initialise the graphics module
	if (!InitGrid()) {
		printf("Error: Can't initialise grid\n");
		SDL_DestroyWindow(g_main_window);
		SDL_Quit();
		return false;
	}

	return true;
}

void HandleEvent(SDL_Event * e) {
    switch (e->type) {
        case SDL_QUIT:
            g_quit = true;
            break;
        case SDL_KEYDOWN:
            switch (e->key.keysym.sym) {
                case SDLK_q:
                case SDLK_ESCAPE:
                    g_quit = true;
                    break;
                // Move viewport
                case SDLK_a:
                case SDLK_d:
                case SDLK_w:
                case SDLK_s:
                    break;
                // pause
                case SDLK_p:
//                    g_running = !g_running;
                    break;
                // cursor
                case SDLK_LEFT:
                case SDLK_RIGHT:
                case SDLK_UP:
                case SDLK_DOWN:
                    break;
                // toggle
                case SDLK_e:
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void Update() {
    g_world->NextGeneration();
}

void Render() {
    Clear();
    for (u32 i = 0; i < 40; i += 1) {
        for (u32 j = 0; j < 30; j += 1) {
            if (g_world->GetCreature(i, j) == SENTIENT) {
                PaintTile(i, j, 50);
            }
        }
    }
    SDL_GL_SwapWindow(g_main_window);
}

int main(int argc, char ** argv) {
    if (!InitEnvironment()) {
		return 1;
	}

    g_world = new World(40, 30);

    g_world->ToggleCreature(1, 0);
    g_world->ToggleCreature(2, 1);
    g_world->ToggleCreature(0, 2);
    g_world->ToggleCreature(1, 2);
    g_world->ToggleCreature(2, 2);

    g_quit = false;
    g_running = true;

    SDL_Event event;

    while (!g_quit) {
        if (SDL_PollEvent(&event)) {
            HandleEvent(&event);
        }
//        Update();
//        Render();
        Clear();
        RenderAll();
        SDL_GL_SwapWindow(g_main_window);
    }

	SDL_DestroyWindow(g_main_window);
	SDL_Quit();
    delete g_world;
	return 0;
}
