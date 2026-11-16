#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    s32 z;
    s32 scale;
    u8 pad10[0x1C - 0x10];
    s32 ox;
    s32 oy;
    s32 oz;
} UnkStruct_023569d8;

void func_ov000_02356be8(UnkStruct_023569d8 *w);
void func_ov000_02356a48(UnkStruct_023569d8 *w);

#pragma thumb on

void func_ov000_023569d8(UnkStruct_023569d8 *w) {
    s32 x = w->x;
    s32 y = w->y;
    s32 z = w->z;
    s32 scale = w->scale;

    w->scale = (s32)(((s64)scale * 0x1000 + 0x800) >> 12);
    w->x = x + w->ox;
    w->y = y + w->oy;
    w->z = z + w->oz;
    func_ov000_02356be8(w);
    w->x = x - w->ox;
    w->y = y - w->oy;
    w->z = z - w->oz;
    func_ov000_02356a48(w);
    w->x = x;
    w->y = y;
    w->z = z;
    w->scale = scale;
}

#pragma thumb reset
