#include <cstdio>

#include <SDL2/SDL.h>

#include "World.h"
#include "Renderer.h"

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
Renderer * g_renderer;
bool g_running;
bool g_quit;

// starts SDL and OpenGL
bool InitEnvironment()
{
    u32 screen_width = 640, screen_height = 480;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return false;
	}

	// Create SDL window
	g_main_window = SDL_CreateWindow("Life", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
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
    g_renderer = new Renderer(screen_width, screen_height);

	if (g_renderer->Fail()) {
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
                    g_running = !g_running;
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
    g_renderer->Clear();

    g_renderer->PaintChar(8 * 0, 16 * 29, 17);
    g_renderer->PaintChar(8 * 1, 16 * 29, 67);
    g_renderer->PaintChar(8 * 2, 16 * 29, 64);
    g_renderer->PaintChar(8 * 3, 16 * 29, 53);

    for (u32 i = 0; i < 40; i += 1) {
        for (u32 j = 0; j < 30; j += 1) {
            if (g_world->GetCreature(i, j) == SENTIENT) {
                g_renderer->PaintTile(16 * i, 16 * j, 86);
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
    g_running = false;

    SDL_Event event;

    Render();
    while (!g_quit) {
        if (SDL_PollEvent(&event)) {
            HandleEvent(&event);
        }
        if (g_running) {
            Update();
            Render();
        }
    }

	SDL_DestroyWindow(g_main_window);
	SDL_Quit();
    delete g_world;
    delete g_renderer;
	return 0;
}
