/**
 * @file player.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-05-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <linmath.h>

#include "../src/engine/object/object.h"

typedef struct Player Player;
struct Player
{
    /**
     * @brief Speed of players movement and rotation
     */
    float speed;

    /**
     * @brief Game object of player
     */
    Object *object;
};

struct APlayer
{
    /**
     * @brief Create player
     */
    Player *(*Init)(vec2 position, float speed);

    /**
     * @brief Get player
     */
    Player *(*Get)();

    /**
     * @brief Move player
     */
    void (*Move)(Player *player, vec2 position);

    /**
     * @brief Render all players
     */
    void (*Render)();
};

extern struct APlayer APlayer[1];

#endif