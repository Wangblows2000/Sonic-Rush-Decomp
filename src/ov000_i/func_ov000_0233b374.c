#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 pad4[4];
    s32 unk8;
    u8 padC[0x10 - 0xc];
    s32 unk10;
    u8 pad14[0x20 - 0x14];
    s16 unk20;
    u8 pad22[0x3a - 0x22];
    s8 unk3A;
    u8 pad3B[0x64 - 0x3b];
} UnkPlayer_0233b374;

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
} UnkObj_0233b374;

extern UnkPlayer_0233b374 gCamera[];
extern UnkObj_0233b374 *gPlayers[];
BOOL func_ov000_0233b470(s32 a);
void func_ov000_0233b684(void);

BOOL func_ov000_0233b374(void) {
    UnkPlayer_0233b374 *p = gCamera;
    u32 flags = p[2].flags;

    if (!(flags & 1))
        return FALSE;
    if (!(flags & 2)) {
        if (gPlayers[p->unk3A]->unk14 > p->unk8 + p->unk20 * 0xe8 && func_ov000_0233b470(1)) {
            p[2].flags |= 0x200;
            p[2].unk10 = -p[2].unk10;
            func_ov000_0233b684();
            return TRUE;
        }
    } else {
        if (gPlayers[p->unk3A]->unk14 < p->unk8 - p->unk20 * 0x28 && func_ov000_0233b470(0)) {
            p[2].flags |= 0x200;
            p[2].unk10 = -p[2].unk10;
            func_ov000_0233b684();
            return TRUE;
        }
    }
    return FALSE;
}
