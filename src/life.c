#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"

#include "SDL.h"
#include "GL/glew.h"
#include "png.h"

#include "world.h"
#include "grid.h"

// Helpful material
// https://www.libsdl.org/release/SDL-1.2.15/docs/html/guidevideoopengl.html
// http://lazyfoo.net/tutorials/SDL/50_SDL_and_opengl_2/
// http://lazyfoo.net/tutorials/SDL/51_SDL_and_modern_opengl/index.php
// http://lazyfoo.net/tutorials/OpenGL/index.php
// https://www.khronos.org/opengl/wiki/Array_Texture
//  http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/

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

    // Initialise the graphics engine
    if (!InitGrid()) {
        printf("Error: Can't initialise grid\n");
        SDL_DestroyWindow(g_main_window);
        SDL_Quit();
        return false;
    }

    return true;
}


bool configure_opengl()
{
    // Used for storing OpenGL error codes
    GLenum GL_error = GL_NO_ERROR;

    // Initialize projection matrix
    // Controls how perspective works in OpenGL
    // We set it to the identity matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GL_error = glGetError();
    if (GL_error != GL_NO_ERROR) {
        printf("OpenGL initialization error: %s\n", gluErrorString(GL_error));
        return false;
    }

    // Same thing with the model view matrix
    // Controls how rendered objects are viewed and placed
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    GL_error = glGetError();
    if (GL_error != GL_NO_ERROR) {
        printf("OpenGL initialization error: %s\n", gluErrorString(GL_error));
        return false;
    }

    // Initialize clear color to black (color used when clearing the screen)
    glClearColor(0, 0, 0, 1);
    GL_error = glGetError();
    if (GL_error != GL_NO_ERROR) {
        printf("OpenGL initialization error: %s\n", gluErrorString(GL_error));
        return false;
    }

    return true;
}

typedef struct {
    GLubyte * pxdata;
    GLuint width;
    GLuint height;
    bool hasAlpha;
} loaded_png;

loaded_png * loadPNGImage(const char * path_to_file)
{
    loaded_png * ret = (loaded_png *)std::malloc(sizeof(loaded_png));

    FILE * f = std::fopen(path_to_file, "rb");
    if (f == NULL) {
        return NULL;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
        NULL, NULL);
    if (png_ptr == NULL) {
        fclose(f);
        return NULL;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fclose(f);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return NULL;
    }

    png_infop end_info = png_create_info_struct(png_ptr);
    if (end_info == NULL) {
        fclose(f);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return NULL;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        fclose(f);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return NULL;
    }

    png_init_io(png_ptr, f);
    png_set_sig_bytes(png_ptr, 0);

    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16
        | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type;

    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);

    ret->width = width;
    ret->height = height;
    ret->hasAlpha = color_type == PNG_COLOR_TYPE_RGB_ALPHA || color_type == PNG_COLOR_MASK_ALPHA;

    uint32_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    ret->pxdata = (GLubyte *)malloc(row_bytes * ret->height);

    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
    for (GLuint i = 0; i < ret->height; i++) {
        memcpy(ret->pxdata + (row_bytes * (ret->height - 1 - i)), row_pointers[i],
            row_bytes);
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    fclose(f);

    return ret;
}

void render()
{
    // Todo

    // // Clear color buffer
    // glClear(GL_COLOR_BUFFER_BIT);
    //
    // // Render quad in center of screen
    // if (render_quad) {
    //     glBegin(GL_QUADS);
    //         glVertex2f(-0.5f, -0.5f);
    //         glVertex2f(0.5f, -0.5f);
    //         glVertex2f(0.5f, 0.5f);
    //         glVertex2f(-0.5f, 0.5f);
    //     glEnd();
    // }
    //
    // // I think I need to set OpenGL to double buffer rendering mode through
    // // SDL, it's in a tutorial somewhere. Also, color depth and something
    // // like that.
    // SDL_GL_SwapWindow(g_main_window);
}


void update()
{
    g_world = next_generation(g_world);
}


void handle_event(SDL_Event * e)
{
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
                    // move_viewport(life_gui, -1, 0);
                    break;
                case SDLK_d:
                    // move_viewport(life_gui, 1, 0);
                    break;
                case SDLK_w:
                    // move_viewport(life_gui, 0, 1);
                    break;
                case SDLK_s:
                    // move_viewport(life_gui, 0, -1);
                    break;
                // pause
                case SDLK_p:
                    g_running = !g_running;
                    break;
                // cursor
                case SDLK_LEFT:
                    // life_gui->c_x -= 1;
                    break;
                case SDLK_RIGHT:
                    // life_gui->c_x += 1;
                    break;
                case SDLK_UP:
                    // life_gui->c_y += 1;
                    break;
                case SDLK_DOWN:
                    // life_gui->c_y -= 1;
                    break;
                case SDLK_e:
                    // toggle_creature(world, life_gui->c_x, life_gui->c_y);
                    break;
            }
            break;
    }
}


int main(int argc, char const * argv[])
{
    if (!init_environment() || !configure_opengl()) {
        return 1;
    }

    // Create world (game of life logic)
    g_world = init_world(100, 100);
    if (g_world == NULL) {
        printf("Error: init_world.\n");
        return 1;
    }

    // Create a spaceship in a corner (for testing purposes)
    set_creature(g_world, 2, 1, SENTIENT);
    set_creature(g_world, 3, 2, SENTIENT);
    set_creature(g_world, 1, 3, SENTIENT);
    set_creature(g_world, 2, 3, SENTIENT);
    set_creature(g_world, 3, 3, SENTIENT);

    g_running = false;
    g_quit = false;
    // Every nth frame, the game updates and renders again, every other frame is
    // for responding to input
    // Todo: control this with a key, allowing for faster or slower simulation
    int timescale = 60;
    int ticks = 0;

    SDL_Event event;

    while (!g_quit) {
        if (SDL_PollEvent(&event)) {
            // handling an event at 60 fps might be too much sensitivity for
            // things like moving the viewport and toggling a cell
            handle_event(&event);
        }

        ticks++;
        if (ticks % timescale == 0) {
            update();
            render();
            ticks = 0;
        }
        // Todo: probably a finer delay, taking into account the ms it took for
        // the last frame to update and render and delaying 16.6 minus that time
        SDL_Delay(17);
    }

    destruct_world(g_world);
    SDL_DestroyWindow(g_main_window);
    SDL_Quit();

    return 0;
}
