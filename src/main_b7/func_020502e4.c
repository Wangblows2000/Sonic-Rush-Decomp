#include <nitro/types.h>

typedef struct {
    u8 pad0[0x34];
    u16 id;
    u8 name[0x16];
} UnkObj_020502e4;

s32 func_02042958(void);
u32 func_020429b8(void);
u8 *func_0204324c(u16 id);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);

#pragma thumb on
BOOL func_020502e4(UnkObj_020502e4 *o) {
    u32 mask;
    u8 *p;
    s32 i;
    if (func_02042958() != 2)
        return FALSE;
    mask = func_020429b8();
    p = NULL;
    for (i = 1; i < 16; i++) {
        if (mask & (1 << i)) {
            p = func_0204324c(i);
            break;
        }
    }
    if (p == NULL)
        return FALSE;
    o->id = i;
    MIi_CpuCopy16(p + 6, o->name, 0x16);
    return TRUE;
}
#pragma thumb reset
