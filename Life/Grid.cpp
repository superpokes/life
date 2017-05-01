#include <cstdio>

#include "glad/glad.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include "SOIL.h"

#include "Grid.h"
#include "ShaderLoader.h"

GLuint program_id;
GLuint vbo_id;

GLint MVP_location;
glm::mat4 view_perspective_matrix;

GLuint tileset_id;
GLint sampler_uniform_id;
GLint layer_uniform_id;

//static const GLfloat vertex_data[] = {
//	-1.0f, -1.0f, 0.0f, // vertex 1 of triangle 1
//	1.0f, -1.0f, 0.0f, // vertex 2 of triangle 1
//	-1.0f, 1.0f, 0.0f, // vertex 3 of triangle 1
//	1.0f, -1.0f, 0.0f, // vertex 1 of triangle 2
//	1.0f, 1.0f, 0.0f,
//	-1.0f, 1.0f, 0.0f
//};

static const GLfloat vertex_data[] = {
    0.f, 0.f, 0.0f, // vertex 1 of triangle 1
    1.f, 0.f, 0.0f, // vertex 2 of triangle 1
    0.f, 1.f, 0.0f, // vertex 3 of triangle 1
    1.f, 0.f, 0.0f, // vertex 1 of triangle 2
    1.f, 1.f, 0.0f,
    0.f, 1.f, 0.0f
};

void LoadTextures() {
    int width, height, channels;
    unsigned char * tile_data = SOIL_load_image("assets/tileset.dds", &width, &height, &channels, SOIL_LOAD_RGB);

//    glGenTextures(1, &tileset_id);
//    glBindTexture(GL_TEXTURE_2D, tileset_id);
//    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB8, GL_UNSIGNED_BYTE, tile_data);
//
//    return;

    printf("width = %d, height = %d, channels = %d\n", width, height, channels);
    fflush(stdout);

//    for (int i = 0; i < 16; i++) {
//        for (int j = 0; j < 256 * 3; j += 3) {
//            printf("%.2hhX%.2hhX%.2hhX ", tile_data[256 * 3 * i + j], tile_data[256 * 3 * i + j + 1], tile_data[256 * 3 * i + j] + 2);
//        }
//        printf("\n");
//    }
//    printf("\n");
//    fflush(stdout);

    glGenTextures(1, &tileset_id);
    glBindTexture(GL_TEXTURE_2D_ARRAY, tileset_id);

    printf("1\n");
    fflush(stdout);

    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGB, 16, 16, 256);

    printf("2\n");
    fflush(stdout);

    // https://www.khronos.org/registry/OpenGL/extensions/EXT/EXT_texture_array.txt
    // https://www.khronos.org/opengl/wiki/Array_Texture
    // http://stackoverflow.com/questions/25349955/how-to-use-opengl-array-texture
    // https://ferransole.wordpress.com/2014/06/09/array-textures/
    // https://www.google.com/search?q=opengl+array+texture+tutorial&oq=opengl+array+texture&aqs=chrome.1.69i57j0l5.4519j0j7&sourceid=chrome&ie=UTF-8

//    for (int i = 0; i < 16; i++) {
//        for (int j = 0; j < 16; j++) {
//            printf(". ");
////            printf("%.3d ", 16 * i + j);
//            fflush(stdout);
////            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 16 * i + j, 16, 16, 1, GL_RGB8, GL_UNSIGNED_BYTE,
////                            tile_data + 16 * (j + i * 256));
////            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 16 * i, 16 * j, 16 * i + j, 16, 16, 1, GL_RGB8, GL_UNSIGNED_BYTE,
////                            tile_data);
//            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 16 * i + j, 16, 16, 1, GL_RGB8, GL_UNSIGNED_BYTE,
//                            tile_data + (i * 256 + j) * 3 * 16);
//        }
//        printf("|\n");
//        fflush(stdout);
//    }
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, 16, 16, 256, GL_RGB8, GL_UNSIGNED_BYTE, tile_data);

    printf("4\n");
}

bool InitGrid() {
	// Load glad functions
	if (!gladLoadGL()) {
        printf("Can't load OpenGL functions");
        exit(-1);
    }

	// Clear color is black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Load shaders
	program_id = LoadShaders("assets/vertex.glsl", "assets/fragment.glsl");
    glUseProgram(program_id);

	// Create VAO and set it as current
	GLuint vao_id;
	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);

	// Create VBO
	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

	// Buffer vertices
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data,
		GL_STATIC_DRAW);

    MVP_location = glGetUniformLocation(program_id, "MVP");

    view_perspective_matrix = glm::ortho(0.f, 40.f, 0.f, 30.f, 0.1f, 100.f) *
            glm::lookAt(glm::vec3(0.f, 0.f, 2.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));


//    textures[0] = SOIL_load_OGL_texture("assets/single2.dds", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID,
//                                        SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_INVERT_Y);
//    textures[1] = SOIL_load_OGL_texture("assets/single.dds", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID,
//                                        SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_INVERT_Y);

    LoadTextures();
    sampler_uniform_id = glGetUniformLocation(program_id, "textureSampler");

    layer_uniform_id = glGetUniformLocation(program_id, "textureLayer");

	return true;
}

void Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PaintTile(u32 x, u32 y, u32 tex_n) {
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    glm::mat4 trans = glm::translate(glm::vec3(x, y, 0));
    glUniformMatrix4fv(MVP_location, 1, GL_FALSE, glm::value_ptr(view_perspective_matrix * trans));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, tileset_id);
    glUniform1i(sampler_uniform_id, 0);

    glUniform1f(layer_uniform_id, (float) tex_n);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(0);
}

void RenderAll() {

}