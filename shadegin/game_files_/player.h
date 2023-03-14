#pragma once

#include <linmath.h>

typedef struct player {
    vec2 position;
    vec2 velocity;
} Player;

void move(Player* player);
void update_player_position(Player* player, vec2 velocity);
