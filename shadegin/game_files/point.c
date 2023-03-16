#include "point.h"
#include <stdlib.h>
#include <float.h>

Point create_point(float x, float y, float forceCapVertical, float forceCapHorizontal, float gravVertical, float gravHotizontal, float weight) {
    Point point = {{x, y}, {forceCapVertical, forceCapHorizontal}, {0, 0}, {gravVertical, gravHotizontal}, weight};
    PointRelation* relation = (PointRelation*)malloc(sizeof(PointRelation));
    relation->exists = malloc(0);
    relation->points = malloc(0);
    point.connections = relation;
    return point;
}

void draw_point(Point* point) {
    render_quad(point->position, (vec2){10, 10}, (vec4){1, 0, 0, 1}, true);
}

void draw_connection(Point* point) {
    for (int i = 0; i < point->connections->point_size; i++) {
        render_line(point->position, point->connections->points[i]->position, (vec4){1, 1, 1, 1});
    }
}

void add_connection(Point* point, Point* connection) {
    point->connections->point_size++;
    point->connections->points = realloc(point->connections->points, point->connections->point_size * sizeof(Point*));
    point->connections->points[point->connections->point_size - 1] = connection;

    connection->connections->exist_size++;
    connection->connections->exists = realloc(connection->connections->exists, connection->connections->exist_size * sizeof(Point*));
    connection->connections->exists[connection->connections->exist_size - 1] = point;
}

Point** create_grid(int width, int height, int x, int y, int offset) {
    Point** points_y = malloc(sizeof(Point*) * height);
    for (int pos_y = 0; pos_y < height; pos_y++) {
        Point* points_x = malloc(sizeof(Point) * width); 
        Point* pointer_point = NULL;

        for (int pos_x = 0; pos_x < width; pos_x++) {
            float gravFV = 0;
            float gravFH = -.1;
            if (pos_y <= 0) {
                gravFH = 0;
                gravFV = 0;
            }
            Point point = create_point(x + offset * pos_x, y - offset * pos_y, 1, 25, gravFV, gravFH, fmax(((float)rand()/(float)(RAND_MAX)) * .1, .1));
            memcpy(points_x + pos_x, &point, sizeof(Point));
            Point* current_point = &points_x[pos_x];
            if (pointer_point != NULL && pos_x < width) {
                add_connection(current_point, pointer_point);
            }
            if (pos_y > 0) {
                add_connection(current_point, &points_y[pos_y - 1][pos_x]);
                add_connection(&points_y[pos_y - 1][pos_x], current_point);
            }
            if (pos_x > 0) {
                add_connection(pointer_point, current_point);
            }
            pointer_point = &points_x[pos_x];
        }
        points_y[pos_y] = points_x;
    }
    return points_y;
}

int sim(Point* point, Point* connected_point, float* velocity_x, float* velocity_y) {
    float stretch_amount = 10;
    float tension = 1;
    float bounciness = .5;
    
    float angle = atan2(point->position[1] - connected_point->position[1], point->position[0] - connected_point->position[0]);
    float dx = point->position[0] - connected_point->position[0];
    float dy = point->position[1] - connected_point->position[1];
    float current_distance = sqrt(dx*dx + dy*dy);

    float accel_y = (sin(angle) * (current_distance / stretch_amount)) / (tension * point->mass);
    accel_y -= -((point->gravitationalForce[1] * point->mass) / bounciness) * (*velocity_y);
    (*velocity_y) += (accel_y * (point->gravitationalForce[1] * point->mass));
    (*velocity_y) *= (1 + ((point->gravitationalForce[1] * point->mass) / bounciness));

    float accel_x = (cos(angle) * (current_distance / stretch_amount)) / (tension * point->mass);
    accel_x -= -((point->gravitationalForce[1] * point->mass) / bounciness) * (*velocity_x);
    (*velocity_x) += (accel_x * (point->gravitationalForce[1] * point->mass));
    (*velocity_x) *= (1 + ((point->gravitationalForce[1] * point->mass) / bounciness));

    float force_y = (*velocity_y) * point->mass * (current_distance / stretch_amount) * tension;
    // printf("%f\n", force_y);
    if (force_y > point->forceCapacity[1] || force_y < -point->forceCapacity[1]) {
        return 1;
    }

    float force_x = (*velocity_x) * point->mass * (current_distance / stretch_amount) * tension;
    if (force_x > point->forceCapacity[0] || force_x < -point->forceCapacity[0]) {
        return 1;
    }
    return 0;
}

void reorder(Point* point, Point* delete) {
    int index = 0;
    while (index < point->connections->point_size && point->connections->points[index] != delete) {
        index++;
    }
    if (index == point->connections->point_size && point->connections->points[index] != delete) {
        printf("point not found\n");
        return;
    }
    printf("point deleted\n");
    point->connections->point_size--;
    for (int i = index; i < point->connections->point_size; i++) {
        point->connections->points[i] = point->connections->points[i + 1];
    }
}

void simulate_gravity(void* args) {
    SimArgs* sim_args = (SimArgs*)args;
    Point* point = sim_args->point;

    float velocity_x = point->velocity[0];
    float velocity_y = (point->velocity[1] + (point->gravitationalForce[1] * point->mass));

    for (int i = 0; i < point->connections->point_size; i++) {
        if (sim(point, point->connections->points[i], &velocity_x, &velocity_y)) {
            Point* delete_point = point->connections->points[i];
            reorder(point, delete_point);
            reorder(delete_point, point);
        }
    }
    point->velocity[1] = velocity_y;
    point->velocity[0] = velocity_x;

    move(point);
}

void move(Point* point) {
    float x = point->position[0];
    float y = point->position[1];

    point->position[0] = x + point->velocity[0];
    point->position[1] = y + point->velocity[1];
}
