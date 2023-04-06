#pragma once

#include <linmath.h>

typedef struct {
    vec2 position;
    float speed;
} Player;

void player_init(vec2 position, float speed);
Player get_player();
void move_player(vec2 position);
void render_player();
