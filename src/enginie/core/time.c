#include "enginie/core/time.h"

Uint64 last_frame_time = 0;
const float FIXED_DT = 1.0f / 60.0f;
float accumulator = 0.0f;
