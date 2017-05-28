#pragma once

#include "glad/glad.h"
#include "glad/glad.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include "Types.h"

class Renderer {
public:
    Renderer();
    bool Fail();
    void Clear();
    void PaintTile(u32 x, u32 y, u32 tex_n);
    void PaintChar(u32 x, u32 y, u32 char_n);

private:
    void LoadTextures();

    GLuint program_id;
    GLuint vbo_id;

    GLint MVP_uniform_id;
    glm::mat4 view_perspective_matrix;

    GLuint tileset_id;
    GLuint chars_id;
    GLint sampler_uniform_id;
    GLint layer_uniform_id;

    static const GLfloat vertex_data[];

    bool fail;
};
