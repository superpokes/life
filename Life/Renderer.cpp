#include <cstdio>

#include "SOIL.h"

#include "Renderer.h"
#include "ShaderLoader.h"

const GLfloat Renderer::vertex_data[] = {
        0.f, 0.f, 0.f, // vertex 1 of triangle 1
        1.f, 0.f, 0.f, // vertex 2 of triangle 1
        0.f, 1.f, 0.f, // vertex 3 of triangle 1
        1.f, 0.f, 0.f, // vertex 1 of triangle 2
        1.f, 1.f, 0.f,
        0.f, 1.f, 0.f
};

void Renderer::LoadTextures() {
    int width, height, channels;
    unsigned char * tile_data = SOIL_load_image("assets/tileset.dds", &width, &height, &channels,
            SOIL_LOAD_AUTO);

    glGenTextures(1, &tileset_id);
    glBindTexture(GL_TEXTURE_2D_ARRAY, tileset_id);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

    int tilesWide = 16;
    int tilesHigh = 16;

    int tileWidth = width / tilesWide;
    int tileHeight = height / tilesHigh;

    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGB8, tileWidth, tileHeight, 256);

    glPixelStorei(GL_UNPACK_ROW_LENGTH, width);

    for (int y = 0; y < tilesHigh; y++) {
        for (int x = 0; x < tilesWide; x++) {
            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, tilesWide * y + x, tileWidth,
                    tileHeight, 1, GL_RGB, GL_UNSIGNED_BYTE,
                    tile_data + (y * tileHeight * width + x * tileWidth) * channels);
        }
    }

    free(tile_data);

    // Characters tileset
    unsigned char * char_data = SOIL_load_image("assets/chars.dds", &width, &height, &channels,
            SOIL_LOAD_AUTO);

    glGenTextures(1, &chars_id);
    glBindTexture(GL_TEXTURE_2D_ARRAY, chars_id);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

    tilesWide = 16;
    tilesHigh = 8;
    tileWidth = width / tilesWide;
    tileHeight = height / tilesHigh;

    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGB8, tileWidth, tileHeight, 128);

    glPixelStorei(GL_UNPACK_ROW_LENGTH, width);

    for (int y = 0; y < tilesHigh; y++) {
        for (int x = 0; x < tilesWide; x++) {
            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, tilesWide * y + x, tileWidth,
                    tileHeight, 1, GL_RGB, GL_UNSIGNED_BYTE,
                    char_data + (y * tileHeight * width + x * tileWidth) * channels);
        }
    }

    free(char_data);

    return;
}

Renderer::Renderer() {
	// Load glad functions
	if (!gladLoadGL()) {
        printf("Can't load OpenGL functions");
        fail = true;
    } else {
        fail = false;
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

    MVP_uniform_id = glGetUniformLocation(program_id, "MVP");

    view_perspective_matrix = glm::ortho(0.f, 640.f, 0.f, 480.f, 0.1f, 100.f) *
            glm::lookAt(glm::vec3(0.f, 0.f, 2.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));

    LoadTextures();

    sampler_uniform_id = glGetUniformLocation(program_id, "textureSampler");

    layer_uniform_id = glGetUniformLocation(program_id, "textureLayer");
}

void Renderer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::PaintTile(u32 x, u32 y, u32 tex_n) {
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    glm::mat4 model = glm::translate(glm::vec3(16 * x, 16 * y, -10.f)) *
            glm::scale(glm::vec3(16.f, 16.f, 1.f));
    glUniformMatrix4fv(MVP_uniform_id, 1, GL_FALSE, glm::value_ptr(view_perspective_matrix * model));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, tileset_id);
    glUniform1i(sampler_uniform_id, 0);

    glUniform1i(layer_uniform_id, tex_n);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(0);
}

void Renderer::PaintChar(u32 x, u32 y, u32 char_n) {
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    glm::mat4 model = glm::translate(glm::vec3(8 * x, 16 * y, -3.)) *
            glm::scale(glm::vec3(8.f, 16.f, 1.f));
    glUniformMatrix4fv(MVP_uniform_id, 1, GL_FALSE, glm::value_ptr(view_perspective_matrix * model));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, chars_id);
    glUniform1i(sampler_uniform_id, 0);

    glUniform1i(layer_uniform_id, char_n);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(0);
}

bool Renderer::Fail() {
    return fail;
}
