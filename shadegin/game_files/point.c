#include "point.h"
#include <stdlib.h>
#include <float.h>

Point create_point(float x, float y, float forceCapVertical, float forceCapHorizontal, float grav, float weight) {
    Point point = {{x, y}, {forceCapVertical, forceCapHorizontal}, {0, 0}, grav, weight};
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
    for (int i = 0; i < fmin(2, point->connections->point_size); i++) {
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
            float mass = .7;
            float gravFV = -.005;
            if (pos_y <= 0) {
                gravFV = 0;
            }
            // if (pos_y == 1 && pos_x == 1) {
            //     mass = .1;
            // }
            Point point = create_point(x + offset * pos_x, y - offset * pos_y, 1, 1, gravFV, fmax(((float)rand()/(float)(RAND_MAX)) * .1, mass));
            memcpy(points_x + pos_x, &point, sizeof(Point));
            Point* current_point = &points_x[pos_x];
            if (pointer_point != NULL) {
                add_connection(current_point, pointer_point);
                add_connection(pointer_point, current_point);
            }
            if (pos_y > 0) {
                add_connection(current_point, &points_y[pos_y - 1][pos_x]);
                add_connection(&points_y[pos_y - 1][pos_x], current_point);
            }
            pointer_point = &points_x[pos_x];
        }
        points_y[pos_y] = points_x;
    }
    return points_y;
}

float sign(float num) {
    if (num <= -FLT_MIN) {
        return -1;
    }
    return 1;
}

float get_overall_mass(Point* point) {
    float mass = 0;
    for (int i = 0; i < point->connections->point_size; i++) {
        if (point->connections->points[i]->position[1] < point->position[1]) {
            mass = get_overall_mass(point->connections->points[i]);
        }
    }

    return point->mass + mass;
}

int sim(Point* point, Point* connected_point, float* velocity_x, float* velocity_y) {
    float stifness = 10;
    float distance = 2;
    float dampen = .97;
 
    float angle = atan2(point->position[1] - connected_point->position[1], point->position[0] - connected_point->position[0]);
    float dx = point->position[0] - connected_point->position[0];
    float dy = point->position[1] - connected_point->position[1];
    float current_distance = sqrt(dx*dx + dy*dy);
    float stretch = fmax((current_distance - distance) * stifness, FLT_MIN);

    float force_dir_y = sin(angle);
    float force_y = force_dir_y * point->gravitationalForce * point->mass * connected_point->mass * (stretch);
    float accel_y = (force_y / point->mass);
    (*velocity_y) += accel_y;
    (*velocity_y) *= dampen;

    float force_dir_x = cos(angle);
    float force_x = force_dir_x * point->gravitationalForce * point->mass * connected_point->mass * (stretch);
    float accel_x = (force_x / point->mass);
    (*velocity_x) += accel_x;
    (*velocity_x) *= dampen;

    if (force_y > point->forceCapacity[1] || force_y < -point->forceCapacity[1]) {
        return 1;
    }

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

float point_radius(vec2 position, int radius, Point* point) {
    float dx = point->position[0] - position[0];
    float dy = point->position[1] - position[1];
    float distance = sqrt(dx*dx + dy*dy);

    float ratio = distance / radius;
    return fmax(fmin(1 - ratio, 1), 0);
}

void simulate_wind(vec2 position, int radius, int power, Point* point) {
    float ratio = point_radius(position, radius, point);
    if (ratio <= 0) {
        return;
    }

    float angle = atan2(point->position[1] - position[1], point->position[0] - position[0]);

    point->velocity[0] += cos(angle) * ratio * power * point->mass;
    point->velocity[1] += sin(angle) * ratio * power * point->mass;
}

void simulate_gravity(void* args) {
    SimArgs* sim_args = (SimArgs*)args;
    Point* point = sim_args->point;

    float velocity_x = point->velocity[0];
    float velocity_y = (point->velocity[1] + (point->gravitationalForce * get_overall_mass(point)));

    if ((point->gravitationalForce * point->mass) == 0) {
        return;
    }

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
