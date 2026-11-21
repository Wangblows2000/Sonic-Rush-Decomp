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
void ZoneMap_InitBlazeNodes(UnkStruct_022e4494 *out, s32 a) {
    UnkStruct_02040d14 buf;
    KeyframeAnim_Init(&buf, a, 0);
    SET_ENTRY(1, 3, 5)
    SET_ENTRY(2, 0, 2)
    SET_ENTRY(3, 2, 4)
    SET_ENTRY(6, 1, 3)
    SET_ENTRY(8, 5, 7)
    SET_ENTRY(12, 4, 6)
    SET_ENTRY(13, 6, 8)
    SET_ENTRY(14, 7, 9)
    SET_ENTRY(16, 9, 12)
    SET_ENTRY(0, 10, 10)
    SET_ENTRY(4, 11, 15)
    SET_ENTRY(5, 11, 16)
    SET_ENTRY(11, 12, 17)
    SET_ENTRY(10, 12, 18)
    out[15].x = 0;
    out[15].y = 0;
    out[15].id = 0x16;
    out[15].type = 4;
    out[7].x = 0;
    out[7].y = 0;
    out[7].id = 0x16;
    out[7].type = 4;
    out[9].x = 0;
    out[9].y = 0;
    out[9].id = 0x16;
    out[9].type = 3;
}
#pragma thumb reset
