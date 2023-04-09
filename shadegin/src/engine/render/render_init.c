#include <glad/glad.h>
#include <SDL2/SDL.h>

#include "../util.h"
#include "../global.h"

#include "render.h"
#include "render_internal.h"
#include "camera.h"

SDL_Window *render_init_window(u32 width, u32 height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        ERROR_EXIT("couldn't initialize SDL: %s\n", SDL_GetError());
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window *window = SDL_CreateWindow(
        "Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        global.render.width,
        global.render.height,
        SDL_WINDOW_OPENGL);

    if (!window)
    {
        ERROR_EXIT("window creation failed: %s\n", SDL_GetError());
    }

    if (SDL_GL_CreateContext(window) == NULL) {
        ERROR_EXIT("failed to create window: %s\n", SDL_GetError());
    }
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        ERROR_EXIT("failed to load GL: %s\n", SDL_GetError());
    }

    puts("openGL loaded");
    printf("vendor:        %s\n", glGetString(GL_VENDOR));
    printf("renderer:      %s\n", glGetString(GL_RENDERER));
    printf("version:       %s\n", glGetString(GL_VERSION));
    printf("glsl version:  %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    return window;
}

void render_init_shaders(Render_State_Internal *state) {
    state->shader_default = render_shader_create("./shaders/default.vert", "./shaders/default.frag");

    camera_init(&state->projection, 500, (vec3){0, 1, 2});
    Camera* camera = get_camera();

    glUseProgram(state->shader_default);
    glEnable(GL_DEPTH_TEST);
    glUniformMatrix4fv(glGetUniformLocation(state->shader_default, "projection"), 1, GL_FALSE, &state->projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(state->shader_default, "view"), 1, GL_FALSE, &camera->view[0][0]);
}

void render_init_color_texture(u32 *texture) {
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);

    u8 solid_white[4] = {255, 255, 255, 255};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, solid_white);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void render_init_quad(u32 *vao, u32 *vbo, u32 *ebo) {
    f32 vertices[] = {
        0.5,  0.5,  0, 0, 0,
        0.5,  -0.5, 0, 0, 1,
        -0.5, -0.5, 0, 1, 1,
        -0.5, 0.5,  0, 1, 0,
    };

    u32 indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    glGenVertexArrays(1, vao);
    glGenBuffers(1, vbo);
    glGenBuffers(1, ebo);

    glBindVertexArray(*vao);

    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), NULL);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);
}

void render_init_square(u32 *vao, u32 *vbo, u32 *ebo) {
    f32 vertices[] = {
        // positions     // texture coordinates
        -0.5, -0.5,  0.5, 0, 0,
         0.5, -0.5,  0.5, 1, 0,
         0.5,  0.5,  0.5, 1, 1,
        -0.5,  0.5,  0.5, 0, 1,

        -0.5, -0.5, -0.5, 0, 0,
         0.5, -0.5, -0.5, 1, 0,
         0.5,  0.5, -0.5, 1, 1,
        -0.5,  0.5, -0.5, 0, 1,
    };

    u32 indices[] = {
         0,  1,  2,  2,  3,  0,
         4,  5,  6,  6,  7,  4,
         1,  5,  6,  6,  2,  1,
         0,  4,  7,  7,  3,  0,
         3,  7,  6,  6,  2,  3,
         0,  1,  5,  5,  4,  0
    };

    glGenVertexArrays(1, vao);
    glGenBuffers(1, vbo);
    glGenBuffers(1, ebo);

    glBindVertexArray(*vao);

    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), NULL);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);
}

void render_init_poly(u32 *vao, u32 *vbo, u32 *ebo) {
    glGenVertexArrays(1, vao);
    glGenBuffers(1, vbo);
    glGenBuffers(1, ebo);

    glBindVertexArray(*vao);

    glBindBuffer(GL_ARRAY_BUFFER, *vbo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), NULL);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);
}

void render_init_line(u32 *vao, u32 *vbo, u32 *ebo) {
    u32 indices[] = {
        0, 1
    };

    glGenVertexArrays(1, vao);
    glGenBuffers(1, vbo);
    glGenBuffers(1, ebo);

    glBindVertexArray(*vao);

    glBindBuffer(GL_ARRAY_BUFFER, *vbo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), NULL);
    glEnableVertexAttribArray(0);
}
