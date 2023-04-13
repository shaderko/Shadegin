#pragma once

#include <linmath.h>
#include "../src/engine/game_objects/game_object.h"

typedef struct {
    float speed;
    GameObject* object;
} Player;

void player_init(vec2 position, float speed);
GameObject *get_player();
void move_player(vec2 position);
void render_player();
