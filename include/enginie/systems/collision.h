#include <stdbool.h>

void update_collision_system(int entity_id);
bool aabb(float first_entity_x, float first_entity_y, float second_entity_x, float second_entity_y,
          int first_entity_width, int first_entity_height, int second_entity_width, int second_entity_height);