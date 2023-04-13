#include <glad/glad.h>
#include <stdio.h>

#include "../util.h"
#include "../io/io.h"
#include "render_internal.h"

u32 render_shader_create(const char *path_vert, const char *path_frag) {
    int success;
    char log[512];

    File file_vertex = io_file_read(path_vert);
    if (!file_vertex.is_valid) {
        ERROR_EXIT("error reading shader: %s\n", path_vert);
    }

    u32 shader_vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader_vertex, 1, (const char *const *)&file_vertex, NULL);
    glCompileShader(shader_vertex);
    glGetShaderiv(shader_vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader_vertex, 512, NULL, log);
        ERROR_EXIT("error compiling vertex shader. %s\n", log);
    }

    File file_fragment = io_file_read(path_frag);
    if (!file_fragment.is_valid) {
        ERROR_EXIT("error reading shader: %s\n", path_frag);
    }

    u32 shader_fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader_fragment, 1, (const char *const *)&file_fragment, NULL);
    glCompileShader(shader_fragment);
    glGetShaderiv(shader_fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader_fragment, 512, NULL, log);
        ERROR_EXIT("error compiling fragment shader. %s\n", log);
    }

    u32 shader = glCreateProgram();
    glAttachShader(shader, shader_vertex);
    glAttachShader(shader, shader_fragment);
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader, 512, NULL, log);
        ERROR_EXIT("error linking shader. %s\n", log);
    }

    free(file_vertex.data);
    free(file_fragment.data);

    return shader;
}

// Path to the file without extension with file name.
// This function will create shaders for path.vert, path.frag using render_shader_create()
u32 render_shader_create_name(char *path) {
    size_t len = strlen(path);
  
    char vertex_shader[len + 6];
    memset(vertex_shader, 0, sizeof(vertex_shader));
    strcat(vertex_shader, path);
    strcat(vertex_shader, ".vert");
    
    char fragment_shader[len + 6];
    memset(fragment_shader, 0, sizeof(fragment_shader));
    strcat(fragment_shader, path);
    strcat(fragment_shader, ".frag");

    return render_shader_create(vertex_shader, fragment_shader);
}
