#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
    u8 pad[0x28];
} UnkStruct_02040d14;

typedef struct {
    s32 x;
    s32 y;
    s32 id;
    s32 type;
} UnkStruct_022e4494;

void KeyframeAnim_Init(void *p, s32 a, s32 b);
void KeyframeAnim_SetAnim(void *p, u32 value);
void KeyframeAnim_Update(void *p);

#define SET_ENTRY(e, anim, n)            \
    KeyframeAnim_SetAnim(&buf, anim);           \
    KeyframeAnim_Update(&buf);                 \
    out[e].x = buf.x;                    \
    out[e].y = buf.y;                    \
    out[e].id = n;                       \
    out[e].type = 3;

#pragma thumb on
void ZoneMap_InitSonicNodes(UnkStruct_022e4494 *out, s32 a) {
    UnkStruct_02040d14 buf;
    KeyframeAnim_Init(&buf, a, 0);
    SET_ENTRY(1, 0, 2)
    SET_ENTRY(2, 1, 3)
    SET_ENTRY(3, 2, 4)
    SET_ENTRY(6, 3, 5)
    SET_ENTRY(8, 4, 6)
    SET_ENTRY(12, 5, 7)
    SET_ENTRY(13, 6, 8)
    SET_ENTRY(14, 7, 9)
    SET_ENTRY(15, 8, 11)
    SET_ENTRY(16, 9, 12)
    SET_ENTRY(7, 11, 13)
    SET_ENTRY(9, 12, 14)
    out[0].x = 0;
    out[0].y = 0;
    out[0].id = 0x16;
    out[0].type = 4;
    out[4].x = 0;
    out[4].y = 0;
    out[4].id = 0x16;
    out[4].type = 4;
    out[5].x = 0;
    out[5].y = 0;
    out[5].id = 0x16;
    out[5].type = 4;
    out[11].x = 0;
    out[11].y = 0;
    out[11].id = 0x16;
    out[11].type = 4;
    out[10].x = 0;
    out[10].y = 0;
    out[10].id = 0x16;
    out[10].type = 4;
}
#pragma thumb reset
