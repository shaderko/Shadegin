#pragma once

#include <linmath.h>
#include <stdbool.h>

#include "../src/engine/render/render.h"

typedef struct Point Point;

typedef struct {
    Point* point;
} SimArgs;

typedef struct PointRelation {
    Point** points;
    int point_size;
    Point** exists;
    int exist_size;
} PointRelation;

struct Point {
    vec2 position;
    vec2 forceCapacity;
    vec2 velocity;
    vec2 gravitationalForce;
    float mass;
    PointRelation* connections;
};

Point create_point(float x, float y, float forceCapVertical, float forceCapHorizontal, float gravVertical, float gravHotizontal, float weight);
void draw_point(Point* point);
void draw_connection(Point* point);
void add_connection(Point* point, Point* connection);
Point** create_grid(int width, int height, int x, int y, int offset);
void simulate_gravity(void*);
void move(Point* point);
