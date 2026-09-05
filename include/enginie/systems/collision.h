#pragma once
#include <stdbool.h>

typedef struct
{
    float left, right, top, bottom;
} Bounds;

typedef struct
{
    int a;
    int b;
} CollisionPair;

extern CollisionPair collisions[];
extern int collision_count;

Bounds collider_bounds(int entity_id);
bool bounds_overlap(Bounds a, Bounds b);
void update_collision_system(void);