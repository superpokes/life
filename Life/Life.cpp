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
bool g_on_menu;
int g_inverse_speed;
int g_ticks;
char g_bottom_string[80] =
        "Speed: 1/4   +: Faster -: Slower P: Pause                                     ";
u32 g_cursor_pos_x;
u32 g_cursor_pos_y;

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
  // Bug: Doesn't work on my linux for some reason
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
                // Move world / the window of the world we see
                case SDLK_a:
                    g_world->MoveFocus(-1, 0);
                    break;
                case SDLK_d:
                    g_world->MoveFocus(1, 0);
                    break;
                case SDLK_w:
                    g_world->MoveFocus(0, 1);
                    break;
                case SDLK_s:
                    g_world->MoveFocus(0, -1);
                    break;
                // pause
                case SDLK_p:
                    g_running = !g_running;
                    break;
                // menu
                case SDLK_m:
                    g_on_menu = !g_on_menu;
                    break;
                // cursor
                case SDLK_LEFT:
                    if (g_cursor_pos_x > 0) {
                        g_cursor_pos_x -= 1;
                    }
                    break;
                case SDLK_RIGHT:
                    if (g_cursor_pos_x < 39) {
                        g_cursor_pos_x += 1;
                    }
                    break;
                case SDLK_UP:
                    if (g_cursor_pos_y < 29) {
                        g_cursor_pos_y += 1;
                    }
                    break;
                case SDLK_DOWN:
                    if (g_cursor_pos_y > 1) {
                        g_cursor_pos_y -= 1;
                    }
                    break;
                // toggle
                case SDLK_e:
                    g_world->ToggleCreature(30 + g_cursor_pos_x, 34 + g_cursor_pos_y);
                    break;
                // timescale
                case SDLK_KP_PLUS:
                    if (g_inverse_speed > 1) {
                        g_inverse_speed /= 2;
                        sprintf(g_bottom_string + 9, "%d", g_inverse_speed);
                    }
                    break;
                case SDLK_KP_MINUS:
                    if (g_inverse_speed < 128) {
                        g_inverse_speed *= 2;
                        sprintf(g_bottom_string + 9, "%d", g_inverse_speed);
                    }
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
    if (g_ticks % g_inverse_speed == 0) {
        g_world->NextGeneration();
    }
    g_ticks += 1;
}

void RenderString(const char * render_str, const u32 str_size, const u32 x_pos, const u32 y_pos) {
    for (u32 i = 0; i < str_size; i++) {
        g_renderer->PaintChar(8 * (i + x_pos), 16 * y_pos, render_str[i] - ' ');
    }
}

void Render() {
    g_renderer->Clear();

//    g_renderer->PaintChar(8 * 0, 16 * 29, 17);
//    g_renderer->PaintChar(8 * 1, 16 * 29, 67);
//    g_renderer->PaintChar(8 * 2, 16 * 29, 64);
//    g_renderer->PaintChar(8 * 3, 16 * 29, 53);
    RenderString(g_bottom_string, 80, 0, 0);

    if (g_on_menu) {
        // Render menu box
        g_renderer->PaintTile(16 * 5, 16 * 5, 106);
        g_renderer->PaintTile(16 * 34, 16 * 5, 107);
        g_renderer->PaintTile(16 * 5, 16 * 25, 104);
        g_renderer->PaintTile(16 * 34, 16 * 25, 105);
        for (u32 i = 5; i < 35; i++) {
            g_renderer->PaintTile(16 * i, 16 * 5, 101);
            g_renderer->PaintTile(16 * i, 16 * 25, 101);
        }
        for (u32 j = 6; j < 25; j++) {
            g_renderer->PaintTile(16 * 5, 16 * j, 102);
            g_renderer->PaintTile(16 * 34, 16 * j, 102);
        }
        for (u32 i = 6; i < 34; i++) {
            for (u32 j = 6; j < 25; j++) {
                g_renderer->PaintTile(16 * i, 16 * j, 252);
            }
        }
        // Render text
        RenderString("Life", 4, 38, 24);
        RenderString("Controls:", 9, 18, 23);
        RenderString("WASD: Camera control", 20, 22, 22);
        RenderString("wwww: Cursor movement", 21, 22, 21);
        RenderString("E: Toggle creature", 18, 22, 20);
        RenderString("P: Pause", 8, 22, 19);
        RenderString("+/-: Faster/slower simulation", 28, 22, 18);
        RenderString("M: Open/close menu", 18, 22, 17);
        RenderString("ESC: Close game", 15, 22, 16);
    }

    for (u32 i = 0; i < 40; i += 1) {
        for (u32 j = 1; j < 30; j += 1) {
            if (g_cursor_pos_x == i && g_cursor_pos_y == j) {
                g_renderer->PaintTile(16 * i, 16 * j, 233);
            // Volatile: The GetCreature call needs to change if World size is changed from 100
            } else if (g_world->GetCreature(30 + i, 34 + j) == SENTIENT) {
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

    g_world = new World(100, 100);

    g_world->ToggleCreature(31, 35);
    g_world->ToggleCreature(32, 36);
    g_world->ToggleCreature(30, 37);
    g_world->ToggleCreature(31, 37);
    g_world->ToggleCreature(32, 37);

    g_quit = false;
    g_running = false;
    g_on_menu = false;
    g_inverse_speed = 4;
    g_ticks = 0;
    g_cursor_pos_x = 19;
    g_cursor_pos_y = 14;

    SDL_Event event;

    Render();
    while (!g_quit) {
        if (SDL_PollEvent(&event)) {
            HandleEvent(&event);
        }
        if (g_running) {
            Update();
        }
        Render();
        // Hack: For some reason SDL_GL_SetSwapInterval doesn't work on my
        // linux laptop
        #if __linux__
        SDL_Delay(17);
        #endif
    }

	SDL_DestroyWindow(g_main_window);
	SDL_Quit();
    delete g_world;
    delete g_renderer;
	return 0;
}
