#include <nitro/types.h>

typedef struct {
    u8 pad0[0xcc0];
    s32 unkcc0;
    u8 padcc4[0xcd4 - 0xcc4];
    s32 unkcd4;
    s32 unkcd8;
    u8 padcdc[0xcec - 0xcdc];
    s32 unkcec;
    s32 unkcf0;
} UnkStruct_022d0658;

void func_ov001_022d0220(UnkStruct_022d0658 *p);
void func_ov001_022d1404(UnkStruct_022d0658 *p, void (*fn)(UnkStruct_022d0658 *));
void func_ov001_022d05f8(UnkStruct_022d0658 *p);

#pragma thumb on
void func_ov001_022d0658(UnkStruct_022d0658 *p) {
    func_ov001_022d0220(p);
    if (p->unkcc0 > 0x50000) {
        p->unkcc0 = 0x50000;
        p->unkcd8 = 0;
        p->unkcf0 = 0;
    }
    if (p->unkcd4 + p->unkcec > 0) {
        p->unkcd4 = 0;
        p->unkcec = p->unkcec / 4;
        func_ov001_022d1404(p, func_ov001_022d05f8);
    }
}
#pragma thumb reset
