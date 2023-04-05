#include <glad/glad.h>
#include <cglm/cglm.h>

#include "../global.h"
#include "render.h"
#include "render_internal.h"

static Render_State_Internal state = {0};

void render_init(void) {
    global.render.width = 1400;
    global.render.height = 1000;
    global.render.window = render_init_window(global.render.width, global.render.height);

    render_init_quad(&state.vao_quad, &state.vbo_quad, &state.ebo_quad);
    render_init_poly(&state.vao_poly, &state.vbo_poly, &state.ebo_poly);
    render_init_line(&state.vao_line, &state.vbo_line, &state.ebo_line);
    render_init_shaders(&state);
    render_init_color_texture(&state.texture_color);
}

void render_begin(void) {
    glClearColor(0.08, 0.1, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void render_end(void) {
    SDL_GL_SwapWindow(global.render.window);
}

void render_quad(vec2 pos, vec2 size, vec4 color, bool fill) {
    glUseProgram(state.shader_default);

    mat4x4 model;
    mat4x4_identity(model);

    mat4x4_translate(model, pos[0], pos[1], 0);
    mat4x4_scale_aniso(model, model, size[0], size[1], 1);

    glUniformMatrix4fv(glGetUniformLocation(state.shader_default, "model"), 1, GL_FALSE, &model[0][0]);
    glUniform4fv(glad_glGetUniformLocation(state.shader_default, "color"), 1, color);

    glBindVertexArray(state.vao_quad);
    if (fill)  {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    glBindTexture(GL_TEXTURE_2D, state.texture_color);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);
}

void render_poly(vec2 *vertices, size_t num_vertices, vec4 color) {
    glUseProgram(state.shader_default);

    f32 *vertex_array = (f32*) malloc(num_vertices * 5 * sizeof(f32));

    for (int i = 0; i < num_vertices; i++) {
        int j = i * 5;
        vertex_array[j] = vertices[i][0];
        vertex_array[j+1] = vertices[i][1];
        vertex_array[j+2] = 0;
        vertex_array[j+3] = 1;
        vertex_array[j+4] = 1;
    }

    u32 *indices = (u32*) malloc(num_vertices * 3 * sizeof(u32));

    for (int i = 0; i < num_vertices - 1; i++) {
        int j = i * 3;
        indices[j] = 0;
        indices[j+1] = i+1;
        indices[j+2] = i+2;
    }

    indices[(num_vertices - 1) * 3 - 1] = 1;

    glBindBuffer(GL_ARRAY_BUFFER, state.vbo_poly);
    glBufferData(GL_ARRAY_BUFFER, num_vertices * 5 * sizeof(f32), vertex_array, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, state.ebo_poly);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * (num_vertices - 1) * sizeof(u32), indices, GL_STATIC_DRAW);

    mat4x4 model = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };

    glUniformMatrix4fv(glGetUniformLocation(state.shader_default, "model"), 1, GL_FALSE, &model[0][0]);
    glUniform4fv(glad_glGetUniformLocation(state.shader_default, "color"), 1, color);

    glBindVertexArray(state.vao_poly);

    glBindTexture(GL_TEXTURE_2D, state.texture_color);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_TRIANGLES, num_vertices * 3, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);
    free(vertex_array);
    free(indices);
}

void render_line(vec2 src_pos, vec2 dst_pos, vec4 color) {
    glUseProgram(state.shader_default);

    vec3 vertices[] = {
        {src_pos[0], src_pos[1], 0},
        {dst_pos[0], dst_pos[1], 0}
    };

    glBindBuffer(GL_ARRAY_BUFFER, state.vbo_line);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    mat4x4 model = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };

    glUniformMatrix4fv(glGetUniformLocation(state.shader_default, "model"), 1, GL_FALSE, &model[0][0]);
    glUniform4fv(glad_glGetUniformLocation(state.shader_default, "color"), 1, color);

    glBindVertexArray(state.vao_line);

    glBindTexture(GL_TEXTURE_2D, state.texture_color);
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);
}
