#include "grid.h"

#include "GL/glew.h"

GLuint program_id;
GLuint vbo_id;
static const GLfloat vertex_data[] = {
    -1.0f, -1.0f, 0.0f, // vertex 1 of triangle 1
    1.0f, -1.0f, 0.0f, // vertex 2 of triangle 1
    -1.0f, 1.0f, 0.0f, // vertex 3 of triangle 1
    1.0f, -1.0f, 0.0f, // vertex 1 of triangle 2
    1.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f
}

bool InitGrid()
{
    // Load glew functions
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        printf("Error: can't initialise glew\n");
        return false;
    }

    // Clear color is black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Load shaders
    program_id = LoadShaders("assets/vertex.glsl", "assets/fragment.glsl");

    // Create VAO and set it as current
    GLuint vao_id;
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    // Create VBO
    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

    // Buffer vertices
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data,
        GL_STATIC_DRAW)

    // Todo: deal with UVs somehow, check the tutorial, use your wits

    // Todo: etc
}
void load_atlas(char * filename, int w, int h);
void paint_tile(int id, int x, int y);
void update_screen();
