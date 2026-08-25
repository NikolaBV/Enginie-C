#include <assert.h>
#include <math.h>

#include "enginie/ecs/entity.h"
#include "enginie/systems/animation.h"
#include "game/content.h"

void update_animation_system(int entity_id, float delta_time)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);

    if (does_entity_have_component(entity_id, Animation_Component_Signature))
    {
        AnimationComponent *animation = &components->animation_components[entity_id];
        const AnimationClip *animation_clip = &animation->clips[animation->active_clip];

        if (animation->finished)
            return;

        animation->time_in_frame += delta_time;
        assert(animation_clip->frame_duration > 0.0f);
        while (animation->time_in_frame >= animation_clip->frame_duration)
        {
            animation->time_in_frame -= animation_clip->frame_duration;
            animation->frame_index++;

            if (animation->frame_index >= animation_clip->frame_count)
            {
                if (animation_clip->looping)
                {
                    animation->frame_index = 0;
                }
                else
                {
                    animation->frame_index = animation_clip->frame_count - 1;
                    animation->finished = true;
                }
            }
        }
    }
}

void animation_play(AnimationComponent *animation, int clip)
{
    assert(clip >= 0 && clip < animation->clip_count);

    if (animation->active_clip == clip)
        return;

    animation->active_clip = clip;
    animation->frame_index = 0;
    animation->time_in_frame = 0.0f;
    animation->finished = false;
}
void update_animation_selection_system(int entity_id)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);

    if (does_entity_have_component(entity_id, Velocity_Component_Signature) && does_entity_have_component(entity_id, Animation_Component_Signature))
    {
        VelocityComponent *velocity = &components->velocity_components[entity_id];
        AnimationComponent *animation = &components->animation_components[entity_id];

        bool is_moving = (fabsf(velocity->x) > 0.001f || fabsf(velocity->y) > 0.001f);
        animation_play(animation, is_moving ? CLIP_WALK : CLIP_IDLE);
    }
}
