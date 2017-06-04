#pragma once

#include "glad/glad.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include "Types.h"

class Renderer
{
public:
    Renderer(u32 screen_width, u32 screen_height);
    bool Fail();
    void Clear();
    void PaintTile(u32 x, u32 y, u32 tex_n);
    void PaintChar(u32 x, u32 y, u32 char_n);

private:
    void LoadTextures();

    // Shaders
    GLuint program_id;

    // Vertex
    static const GLfloat quad_vertex_data[];
    GLuint quad_vbo_id;

    // Matrices
    GLint MVP_uniform_id;
    glm::mat4 view_perspective_matrix;

    // Textures
    GLuint tileset_id;
    GLuint chars_id;
    GLint sampler_uniform_id;
    GLint layer_uniform_id;


    bool fail;
};
