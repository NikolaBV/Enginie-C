#pragma once
#include <stdbool.h>
#include "enginie/ecs/components.h"

typedef struct
{
    float left, right, top, bottom;
} Bounds;

typedef struct
{
    int a;
    int b;
    bool trigger;
} CollisionPair;

extern int current_collision_count;
extern CollisionPair collisions[MAX_COLLISIONS];

Bounds collider_bounds(int entity_id);

bool bounds_overlap(Bounds a, Bounds b);
bool was_overlapping(int a, int b);

void update_collision_system(Axis axis);
void collision_begin_frame(void);