#include "player.h"
#include "../src/engine/render/render.h"

Player player = {0};

void player_init(vec2 position, float speed) {
    player.speed = speed;
    GameObject* object = add_object((vec3){position[0], position[1], 1}, (vec3){20, 20, 20}, (vec3){0, 0, 0}, 3, true, COLIDER_SQUARE, RENDERER_SQUARE, &(vec3){5, 5, 5});
    player.object = object;
}

GameObject *get_player() {
    return player.object;
}

void move_player(vec2 position) {
    player.object->velocity[0] = fmin(fmax(position[0] - player.object->position[0], -player.speed), player.speed);
    player.object->velocity[1] = fmin(fmax(position[1] - player.object->position[1], -player.speed), player.speed);

    player.object->position[0] += player.object->velocity[0];
    player.object->position[1] += player.object->velocity[1];
}

void render_player() {
    return;
}
