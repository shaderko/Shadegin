// #include "renderer.h"
// #include "game_object.h"
// #include "../render/render.h"

// static void square_init(Renderer* renderer, const void* init_data) {
//     renderer->type = RENDERER_SQUARE;
//     renderer->data.square = *(const SquareRenderer *)init_data;
// }

// static void circle_init(Renderer* renderer, const void* init_data) {
//     renderer->type = RENDERER_CIRCLE;
//     renderer->data.circle = *(const CircleRenderer *)init_data;
// }

// static RendererInitFunctions renderer_init_functions[3] = {NULL, square_init, circle_init};

// static void square_render(GameObject* object) {
//     render_square(object->position, object->renderer->data.square.size, (vec4){.5, 1, 1, 1}, true);
// }

// static void circle_render(GameObject* object) {
//     render_square(object->position, (vec3){10, 10, 10}, (vec4){0, 1, 1, 1}, false);
// }

// static RendererRenderFunctions renderer_render_functions[3] = {NULL, square_render, circle_render};

// Renderer* renderer_init(RendererType type, const void *init_data) {
//     if (type < 0 || type >= sizeof(renderer_init_functions) / sizeof(RendererInitFunctions)) {
//         return NULL; // Invalid collider type
//     }
//     Renderer* renderer = calloc(1, sizeof(Renderer));
//     if (!renderer) return NULL;

//     RendererInitFunctions init_func = renderer_init_functions[type];
//     if (init_func) {
//         init_func(renderer, init_data);
//     }

//     return renderer;
// }

// void renderer_render(GameObject* object) {
//     RendererType type = object->renderer->type;

//     RendererRenderFunctions render_func = renderer_render_functions[type];

//     if (render_func) {
//         render_func(object);
//     }
// }
