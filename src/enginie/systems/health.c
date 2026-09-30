#include <enginie/ecs/components.h>
#include <enginie/ecs/entity.h>
#include <assert.h>
void apply_damage(int entity_id, int amount)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);

    if (!does_entity_have_component(entity_id, Health_Component_Signature))
    {
        return;
    }
    HealthComponent *health = &components->health_components[entity_id];

    if (health->health - amount <= 0)
    {
        health->health = 0;
    }
    else
    {
        health->health -= amount;
    }
}
void heal(int entity_id, int amount)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);
    if (!does_entity_have_component(entity_id, Health_Component_Signature))
    {
        return;
    }
    HealthComponent *health = &components->health_components[entity_id];

    if (health->health + amount > health->max_health)
    {
        health->health = health->max_health;
    }
    else
    {
        health->health += amount;
    }
}
bool is_dead(int entity_id)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);

    return does_entity_have_component(entity_id, Health_Component_Signature) &&
           components->health_components[entity_id].health <= 0;
}
