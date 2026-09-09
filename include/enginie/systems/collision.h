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
} CollisionPair;

extern CollisionPair collisions[];
extern int collision_count;

Bounds collider_bounds(int entity_id);

bool bounds_overlap(Bounds a, Bounds b);
bool layers_should_test(int entity_a_id, int entity_b_id);

void update_collision_system(void);
void resolve_overlap(int a, int b);