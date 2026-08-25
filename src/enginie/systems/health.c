#include <enginie/ecs/components.h>
#include <enginie/ecs/entity.h>
#include <stdio.h>

void update_health_system(int entity_id)
{
    // TODO actually implement health functionality, im not even sure this function is needed
    HealthComponent *health = &components->health_components[entity_id];
    printf("Player %d health: %d\n", entity_id, health->health);
}
