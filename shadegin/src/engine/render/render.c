#include <glad/glad.h>

#include "../global.h"
#include "render.h"
#include "render_internal.h"

#include "../game_objects/game_object.h"

static Render_State_Internal state = {0};

void render_init(void) {
    global.render.width = 800;
    global.render.height = 600;
    global.render.low_res_width = global.render.width / 5;
    global.render.low_res_height = global.render.height / 5;
    global.render.window = render_init_window(global.render.width, global.render.height);

    render_init_pixelated(&state.low_res_color, &state.low_res_depth, &state.low_res_fbo);

    render_init_quad(&state.vao_quad, &state.vbo_quad, &state.ebo_quad);
    render_init_square(&state.vao_square, &state.vbo_square, &state.ebo_square);
    render_init_poly(&state.vao_poly, &state.vbo_poly, &state.ebo_poly);
    render_init_line(&state.vao_line, &state.vbo_line, &state.ebo_line);
    render_init_screen(&state.vao_screen, &state.vbo_screen);
    render_init_shaders(&state);
    render_init_color_texture(&state.texture_color);
}

void render_shaders(Camera* camera) {
    glUniformMatrix4fv(glGetUniformLocation(state.shader_default, "view"), 1, GL_FALSE, &camera->view[0][0]);
}

void render_begin_pixelated(void) {
    glBindFramebuffer(GL_FRAMEBUFFER, state.low_res_fbo);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render_end_pixelated(void) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Bind the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Use the pixelated shader
    glUseProgram(state.shader_screen);

    // Set the low-resolution texture uniform in the shader
    glUniform1i(glGetUniformLocation(state.shader_screen, "lowResTexture"), 0);
    glUniform2f(glGetUniformLocation(state.shader_screen, "screenSize"), (float)global.render.low_res_width, (float)global.render.low_res_height);

    // Bind the color texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, state.low_res_color);

    // Render the screen quad
    glBindVertexArray(state.vao_screen);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}


void render_begin(void) {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render_end(void) {
    SDL_GL_SwapWindow(global.render.window);
}

void render_light(vec3 position) {
    glUseProgram(state.shader_default);
    GLfloat lightPosition[] = {position[0], position[1], position[2]}; // x, y, z
    GLfloat lightAmbient[] = {0.2f, 0.2f, 0.2f}; // r, g, b
    GLfloat lightDiffuse[] = {0, 1, 1}; // r, g, b
    GLfloat lightSpecular[] = {.2f, .2f, .2f}; // r, g, b

    glUniform3fv(glGetUniformLocation(state.shader_default, "light.position"), 1, lightPosition);
    glUniform3fv(glGetUniformLocation(state.shader_default, "light.ambient"), 1, lightAmbient);
    glUniform3fv(glGetUniformLocation(state.shader_default, "light.diffuse"), 1, lightDiffuse);
    glUniform3fv(glGetUniformLocation(state.shader_default, "light.specular"), 1, lightSpecular);
    glUniform3fv(glGetUniformLocation(state.shader_default, "light.constant"), 1, &(float){1.0f});
    glUniform3fv(glGetUniformLocation(state.shader_default, "light.linear"), 1, &(float){0.09f});
    glUniform3fv(glGetUniformLocation(state.shader_default, "light.quadratic"), 1, &(float){0.032f});

    GLfloat ambient[] = {.1f, .1f, .1f}; // x, y, z
    GLfloat diffuse[] = {1, 1, 1}; // r, g, b
    GLfloat specular[] = {0.5f, 0.5f, 0.5f}; // r, g, b
    GLfloat shininess = 0;

    glUniform3fv(glGetUniformLocation(state.shader_default, "material.ambient"), 1, ambient);
    glUniform3fv(glGetUniformLocation(state.shader_default, "material.diffuse"), 1, diffuse);
    glUniform3fv(glGetUniformLocation(state.shader_default, "material.specular"), 1, specular);
    glUniform3fv(glGetUniformLocation(state.shader_default, "material.shininess"), 1, &shininess);
}

void render_quad(vec3 pos, vec3 size, vec4 color, bool fill) {
    glUseProgram(state.shader_default);

    mat4x4 model;
    mat4x4_identity(model);

    mat4x4_translate(model, pos[0], pos[1], pos[2]);
    mat4x4_scale_aniso(model, model, size[0], size[1], size[2]);

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

void render_square(vec3 pos, vec3 size, vec4 color, bool fill) {
    glUseProgram(state.shader_default);

    mat4x4 model;
    mat4x4_identity(model);

    mat4x4_translate(model, pos[0], pos[1], pos[2]);
    // mat4x4_rotate_X(model, model, 0.785398);
    mat4x4_scale_aniso(model, model, size[0], size[1], size[2]);

    glUniformMatrix4fv(glGetUniformLocation(state.shader_default, "model"), 1, GL_FALSE, &model[0][0]);
    glUniform4fv(glad_glGetUniformLocation(state.shader_default, "color"), 1, color);

    glBindVertexArray(state.vao_square);
    if (fill)  {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    glBindTexture(GL_TEXTURE_2D, state.texture_color);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

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
