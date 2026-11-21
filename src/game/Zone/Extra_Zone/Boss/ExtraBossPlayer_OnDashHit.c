#include <nitro/types.h>

typedef struct { u8 pad[0x18]; s32 x, y, z; u8 pad2[0x2c]; s32 v50, v54; s32 pad3; s32 v5c, v60; u8 pad4[0x10]; void (*fn)(void); } UnkStruct_023019f8;
void ExtraBossPlayer_State_Dash(void);
void ExtraBossPlayer_SpawnClashFx(s32 a, s32 x, s32 y, s32 z);

void ExtraBossPlayer_OnDashHit(UnkStruct_023019f8 **pa, u8 **pb) {
    UnkStruct_023019f8 *a = *pa;
    u8 *b = *pb;
    if (a->fn != ExtraBossPlayer_State_Dash) {
        return;
    }
    a->v60 = 0;
    a->v5c = a->v60;
    a->v54 = a->v5c;
    a->v50 = a->v54;
    switch (*(s32 *)(b + 0x1c8)) {
    case 8:
    case 9:
    case 0xf:
        ExtraBossPlayer_SpawnClashFx(0, a->x, a->y, a->z);
        break;
    }
}
