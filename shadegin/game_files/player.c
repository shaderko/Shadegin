#include "player.h"
#include "../src/engine/render/render.h"

Player player = {0};

void player_init(vec2 position, float speed) {
    player.speed = speed;
    player.position[0] = position[0];
    player.position[1] = position[1];
}

Player get_player() {
    return player;
}

void move_player(vec2 position) {
    player.position[0] += fmin(fmax(position[0] - player.position[0], -player.speed), player.speed);
    player.position[1] += fmin(fmax(position[1] - player.position[1], -player.speed), player.speed);
}

void render_player() {
    render_square((vec3){player.position[0], player.position[1], 1}, (vec3){20, 20, 20}, (vec4){1, .5, 1, 1}, true);
}
