#include <nitro/types.h>

typedef struct {
    u8 pad0[0x8];
    s32 unk8;
    s32 unkC;
    u8 pad10[0x2e - 0x10];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x34 - 0x32];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5e];
    void *unk68;
    u8 pad6C[0x2cc - 0x6c];
    u8 unk2CC[0x32a - 0x2cc];
    s16 unk32A;
    u32 unk32C;
} UnkStruct_0234aa80;

void func_ov000_0234a288(void);
s32 func_ov000_0234cb70();
void func_ov000_0234c4a4(UnkStruct_0234aa80 *w, s32 a);
void ObjRect_SetAtkDefFlags(void *p, s32 a, s32 b);

void func_ov000_0234aa80(UnkStruct_0234aa80 *w) {
    s16 vx, v;

    if (w->unk58 & 0x10) {
        vx = w->unk2E;
        v = w->unk34;
        func_ov000_0234cb70(w);
        func_ov000_0234c4a4(w, 2);
        w->unk5C |= 4;
        w->unk58 |= 0x10;
        w->unk58 |= 0x8000;
        w->unk68 = func_ov000_0234a288;
        ObjRect_SetAtkDefFlags(w->unk2CC, 0x13, 0);
        w->unk30 = -0x300;
        w->unk2E = vx;
        w->unk34 = v;
        w->unk32C &= ~0xf;
        w->unk32C |= 1;
    }
    w->unkC = 0;
    w->unk8 = 0;
    w->unk32A = 0;
}
