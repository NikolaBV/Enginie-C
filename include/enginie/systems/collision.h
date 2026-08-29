#include <stdbool.h>
#include "enginie/core/config.h"

typedef struct
{
    int a, b;
    bool trigger;
} CollisionPair;
extern CollisionPair collisions[MAX_COLLISIONS];
extern int collision_count;

void update_collision_system(void);
bool aabb(float first_entity_x, float first_entity_y, float second_entity_x, float second_entity_y,
          int first_entity_width, int first_entity_height, int second_entity_width, int second_entity_height);