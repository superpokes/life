#pragma once

#include <string>
#include "glad/glad.h"

GLuint LoadShader(std::string path, GLenum type)
{
	GLuint shader_id = glCreateShader(type);

	FILE * file = fopen(path.c_str(), "rb");

	if (file != NULL) {
		fseek(file, 0, SEEK_END);
		long file_size = ftell(file);
		fseek(file, 0, SEEK_SET);

        char * source = (char *) malloc(file_size + 1);
		fread(source, file_size, 1, file);
		fclose(file);
        source[file_size] = 0;

        GLint result = GL_FALSE;
        int log_length;

        glShaderSource(shader_id, 1, (const GLchar * const *) &source, NULL);
        glCompileShader(shader_id);

        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
        if (log_length > 0) {
            char * error_msg = (char *) malloc(log_length + 1);
            glGetShaderInfoLog(shader_id, log_length, NULL, error_msg);
            error_msg[log_length] = 0;
            printf("%s\n", error_msg);
        }
    } else {
		printf("Error! Unable to load shader\n");
	}

	return shader_id;
}

GLuint LoadShaders(std::string vertex_path, std::string fragment_path)
{
	// Create the shaders
	GLuint vertex_id = LoadShader(vertex_path, GL_VERTEX_SHADER);
	GLuint fragment_id = LoadShader(fragment_path, GL_FRAGMENT_SHADER);

	GLuint program_id = glCreateProgram();
	glAttachShader(program_id, vertex_id);
	glAttachShader(program_id, fragment_id);
	glLinkProgram(program_id);

	GLint result = GL_FALSE;
	int log_length;

	// Check the program
	glGetProgramiv(program_id, GL_LINK_STATUS, &result);
	glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);
	if (log_length > 0) {
		char * error_msg = (char *)malloc((size_t) (log_length + 1));
		glGetProgramInfoLog(program_id, log_length, NULL, error_msg);
		printf("%s\n", error_msg);
	}


	glDetachShader(program_id, vertex_id);
	glDetachShader(program_id, fragment_id);

	glDeleteShader(vertex_id);
	glDeleteShader(fragment_id);

	return program_id;
}