#include <linmath.h>

typedef struct wall {
    vec2 position;
    vec2 size;
} Wall;

Wall create_wall(float x, float y, float width, float height);
vec2* segments(Wall* walls, int size);
Wall random_wall();
