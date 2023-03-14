#pragma once

#include <linmath.h>
#include <stdbool.h>

#include "../src/engine/render/render.h"

typedef struct Point Point;

typedef struct PointRelation {
    Point* point;
    float initialDistance;
} PointRelation;

struct Point {
    vec2 position;
    vec2 forceCapacity;
    vec2 velocity;
    vec2 gravitationalForce;
    float mass;
    PointRelation* connections;
    int connectionsSize;
};

Point create_point(float x, float y, float forceCapVertical, float forceCapHorizontal, float gravVertical, float gravHotizontal, float weight);
void draw_point(Point* point);
void draw_connection(Point* point);
void add_connection(Point* point, Point* connection);
Point** create_grid(int width, int height, int x, int y, int offset);
void simulate_gravity(Point** points, int width, int height);
void move(Point* point);
