#include <nitro/types.h>

void ExtraBoss_SpawnMeteorOfKind(s32 a, s32 b, s32 c, s32 d, s32 e);

void ExtraBoss_SpawnMeteor(s32 a, s32 b, s32 c, s32 d) {
    ExtraBoss_SpawnMeteorOfKind(a, b, c, d, 2);
}
