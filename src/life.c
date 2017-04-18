#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"

#include "SDL.h"
#include "GL/glew.h"
#include "world.h"

// Helpful material
// http://lazyfoo.net/tutorials/SDL/50_SDL_and_opengl_2/
// http://lazyfoo.net/tutorials/OpenGL/index.php
// https://www.khronos.org/opengl/wiki/Array_Texture

// global declarations, prefixed with g_
SDL_Window * g_main_window;
World * g_world;
bool g_running;
bool g_quit;

// starts SDL and OpenGL
bool init_environment() {
    // Initialize SDL
    // Only using video and events subsystem
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
        printf("SDL_Init error: %s\n", SDL_GetError());
        return false;
    }

    // Using OpenGL 3.0
    // It's required to specify a version in order to create the window (or
    // so I was told by the internet)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Create SDL window
    g_main_window = SDL_CreateWindow("Life", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 320, 320, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (g_main_window == NULL) {
        printf("SDL_CreateWindow error: %s\n", SDL_GetError());
        return false;
    }

    // Create an OpenGL context attached to the window
    SDL_GLContext GL_context = SDL_GL_CreateContext(g_main_window);
    if (GL_context == NULL) {
        printf("SDL_GL_CreateContext error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}


bool configure_opengl() {
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


void render() {
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
    // SDL_GL_SwapWindow(g_main_window);
}


void update() {
    // Todo
}


void handle_event(SDL_Event * e) {
    // Todo
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

    SDL_Event * event;

    while (!g_quit) {
        if (SDL_PollEvent(event)) {
            // handling an event at 60 fps might be too much sensitivity for
            // things like moving the viewport and toggling a cell
            handle_event(event);
        }
        // Todo: maybe only render whenever an actual update happens (won't be
        // updating the game 60 times a second, probably)
        update();
        render();
        // Todo: delay (60fps?)
    }

    destruct_world(g_world);
    SDL_DestroyWindow(g_main_window);
    SDL_Quit();

    return 0;
}
