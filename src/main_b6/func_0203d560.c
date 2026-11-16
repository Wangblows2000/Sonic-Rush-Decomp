#include <nitro/types.h>

typedef struct {
    u32 pad0;
    u8 unk4[0x54];
    u32 unk58;
    u32 unk5c;
    u8 pad60[0x20];
    s32 unk80;
    s32 unk84;
    u32 unk88;
    u8 pad8c[0x34];
    u8 bufc0[0x800];
} UnkNode_0203d560;

typedef struct {
    u32 w0;
    u32 w4;
    u32 w8;
} UnkEnt_0203d560;

typedef struct {
    UnkEnt_0203d560 ent[16];
    u8 padc0[0x834c];
    u32 unk840c;
} UnkData_0203d560;

typedef struct {
    u8 pad0[0xc];
    s32 unkc;
} UnkCtx_0203d560;

extern UnkCtx_0203d560 data_022b8be0;
extern u8 data_02073214[];

u32 OS_DisableInterrupts(void);
void OS_RestoreInterrupts(u32 e);
void func_02015354(void *p, u32 size, void *a, void *b, u32 c, s32 d);
void func_0203d358(UnkData_0203d560 *s, UnkNode_0203d560 *n);

void func_0203d560(UnkData_0203d560 *d, UnkNode_0203d560 *n) {
    u32 e = OS_DisableInterrupts();

    if (data_022b8be0.unkc != 0) {
        s32 m = n->unk84;
        u32 i;
        UnkEnt_0203d560 *ent;
        n->unk84 = 0;
        d->unk840c |= m;
        if (n->unk80 <= 0) {
            u32 len;
            void *buf = NULL;
            u32 id = n->unk88;
            len = n->unk5c - n->unk58;
            if (len <= 0x400)
                buf = n->bufc0;
            func_02015354(n->unk4, id, data_02073214, buf, len, 0);
            func_0203d358(d, n);
        }
        ent = d->ent;
        for (i = 0;; i++) {
            s32 bit = 1 << i;
            if (bit > m)
                break;
            if (bit & m) {
                n->unk80++;
                ent->w4 = n->unk88;
            }
            ent++;
        }
    }
    OS_RestoreInterrupts(e);
}
