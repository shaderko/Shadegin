#include "point.h"
#include <stdlib.h>
#include <float.h>

Point create_point(float x, float y, float forceCapVertical, float forceCapHorizontal, float gravVertical, float gravHotizontal, float weight) {
    Point point = {{x, y}, {forceCapVertical, forceCapHorizontal}, {0, 0}, {gravVertical, gravHotizontal}, weight, 0, 0};
    point.connections = (PointRelation*)malloc(0);
    return point;
}

void draw_point(Point* point) {
    render_quad(point->position, (vec2){10, 10}, (vec4){1, 0, 0, 1}, true);
    // printf("pos %f\n", point->position[1]);
}

void draw_connection(Point* point) {
    for (int i = 0; i < point->connectionsSize; i++) {
        render_line(point->position, point->connections[i].point->position, (vec4){1, 1, 1, 1});
    }
}

void add_connection(Point* point, Point* connection) {
    int newConSize = point->connectionsSize + 1;
    point->connections = realloc(point->connections, newConSize * sizeof(PointRelation));
    point->connections[point->connectionsSize].point = connection;
    float dx = point->position[0] - connection->position[0];
    float dy = point->position[1] - connection->position[1];
    float distance = sqrt(dx*dx + dy*dy);
    point->connections[point->connectionsSize].initialDistance = distance;

    point->connectionsSize++;
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
            Point point = create_point(x + offset * pos_x, y - offset * pos_y, 1, max(((float)rand()/(float)(RAND_MAX)) * 15, 10), gravFV, gravFH, max(((float)rand()/(float)(RAND_MAX)) * .5, .5));
            if (pointer_point != NULL && pos_x < width) {
                add_connection(&point, pointer_point);
            }
            if (pos_y > 0) {
                add_connection(&point, &points_y[pos_y - 1][pos_x]);
            }
            memcpy(points_x + pos_x, &point, sizeof(Point));
            pointer_point = &points_x[pos_x];
        }
        points_y[pos_y] = points_x;
    }
    return points_y;
}

float sign(float x) {
    if (x > 0.00001) {
        return 1;
    } else if (x < 0.00001) {
        return -1;
    } else {
        return 0;
    }
}

void simulate_gravity(Point** points, int width, int height) {
    for (int pos_y = 0; pos_y < height; pos_y++) {
        for (int pos_x = 0; pos_x < width; pos_x++) {
            PointRelation* x_con = NULL;
            PointRelation* y_con = NULL;
            Point* point = &points[pos_y][pos_x];
            if (point->connectionsSize > 1) {
                x_con = &point->connections[0];
            }
            if (point->connectionsSize == 2) {
                y_con = &point->connections[1];
            }
            float stretch_amount = 1.3;
            float velocity_x = point->velocity[0];
            float velocity_y = (point->velocity[1] + (point->gravitationalForce[1] * point->mass));
            float angle = 0;
            if (y_con != NULL) {
                float initial_distance = y_con->initialDistance;
                float dx = point->position[0] - y_con->point->position[0];
                float dy = point->position[1] - y_con->point->position[1];
                float current_distance = sqrt(dx*dx + dy*dy);

                float distance_constrain = max(current_distance - initial_distance, 0); // divide by stretch amount

                float force_y = point->velocity[1] * point->mass * distance_constrain;
                if (force_y > point->forceCapacity[1] || force_y < -point->forceCapacity[1]) {
                    point->connectionsSize = 1; 
                    point->velocity[1] = 0;
                    break;
                }

                float cdx = point->position[0] - y_con->point->position[0];
                float cdy = point->position[1] + fabs(velocity_y) - y_con->point->position[1];
                float change_distance = sqrt(cdx*cdx + cdy*cdy);

                if (change_distance <= current_distance) {
                    // getting further, speed up in other direction
                    velocity_y += -sign(velocity_y) * (sqrt((distance_constrain * fabs(velocity_y))) * stretch_amount);
                } else {
                    // getting closer to point, slow down
                    velocity_y = velocity_y - (distance_constrain * point->mass);
                }
                angle = atan2(point->position[1] - y_con->point->position[1], point->position[0] - y_con->point->position[0]) + M_PI / 2;
                float accel_x = (current_distance*sin(angle))/60;
                accel_x -= -(point->gravitationalForce[1] / 2) * velocity_x;
                velocity_x += (accel_x * (point->gravitationalForce[1] * point->mass));
                velocity_x *= (1 + (point->gravitationalForce[1] / 2));
                point->velocity[1] = velocity_y;
            }
            if (x_con != NULL) {
                float initial_distancex = x_con->initialDistance - 2;
                float dxx = point->position[0] - x_con->point->position[0];
                float dyx = point->position[1] - x_con->point->position[1];
                float current_distancex = sqrt(dxx*dxx + dyx*dyx);

                float distance_constrainx = max(current_distancex - initial_distancex, FLT_MIN); // divide by stretch amount
                // printf("%f\n", distance_constrainx);
                // float force_x = point->velocity[0] * point->mass * distance_constrainx;
                // if (force_x > point->forceCapacity[0] || force_x < -point->forceCapacity[0]) {
                //     point->connectionsSize = 1; 
                //     point->velocity[1] = 0;
                //     break;
                // }

                float cdxx = point->position[0] + fabs(velocity_x) - x_con->point->position[0];
                float cdyx = point->position[1] - x_con->point->position[1];
                float change_distancex = sqrt(cdxx*cdxx + cdyx*cdyx);
                if (change_distancex <= current_distancex) {
                    // getting further, speed up in other direction
                    velocity_x += -sign(velocity_x) * sqrt(distance_constrainx * max(fabs(velocity_x), FLT_MIN));
                } else {
                    // getting closer to point, slow down
                    velocity_x -= (sqrt(distance_constrainx) / 150);
                }
            }
            point->velocity[0] = velocity_x;
            // point->velocity[0] = point->velocity[0] - (point->gravitationalForce[1] * point->mass) + point->gravitationalForce[0];

            move(&points[pos_y][pos_x]);
        }
    }
}

void move(Point* point) {
    float x = point->position[0];
    float y = point->position[1];

    point->position[0] = x + point->velocity[0];
    point->position[1] = y + point->velocity[1];
}
