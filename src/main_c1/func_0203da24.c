#include <nitro/types.h>

typedef struct {
    u8 pad0[0x38];
    u8 file38[0x44];
    s32 unk7c;
    u8 pad80[8];
    u32 unk88;
    u32 unk8c;
    u32 unk90;
    s32 unk94;
    s32 unk98;
    s32 unk9c;
    s32 unka0[2];
    u8 pada8[0x18];
    u8 bufc0[2][0x400];
} UnkNode_0203da24;

typedef struct {
    u8 pad0[0x8408];
    UnkNode_0203da24 *head;
} UnkData_0203da24;

typedef struct {
    u8 pad0[0xc];
    s32 unkc;
} UnkCtx_0203da24;

extern u8 data_0207806c[];
extern UnkCtx_0203da24 data_022b8be0;
extern UnkData_0203da24 data_022b9020;

void *func_0200b828(void *name, s32 a);
u32 OS_DisableInterrupts(void);
void OS_RestoreInterrupts(u32 e);
void func_02005c48(s32 a);
BOOL func_0200bbec(void *f, void *arc, u32 top, u32 bottom, u32 id);
s32 func_0200b99c(void *f, void *buf, u32 size);
BOOL func_0200bad8(void *f);
void func_0203d560(UnkData_0203da24 *d, UnkNode_0203da24 *n, s32 a);
void func_0203d2d0(UnkData_0203da24 *d, UnkNode_0203da24 *n);

void func_0203da24(void)
{
    void *arc;
    u32 e;
    s32 i;
    UnkNode_0203da24 *n;
    arc = func_0200b828(data_0207806c, 3);

    for (;;) {
        e = OS_DisableInterrupts();
        while ((n = ((UnkData_0203da24 *)((u32)&data_022b9020 | 0))->head) == NULL) {
            if (data_022b8be0.unkc == 0) {
                OS_RestoreInterrupts(e);
                return;
            }
            func_02005c48(0);
        }
        OS_RestoreInterrupts(e);
        if (n->unk7c == 1) {
            func_0200bbec(n->file38, arc, n->unk8c, n->unk8c + n->unk90, n->unk88);
            for (i = 0; i < 2; i++) {
                n->unka0[i] = i * 0x400;
                func_0200b99c(n->file38, n->bufc0[i], 0x400);
            }
            n->unk94 = 0;
            n->unk9c = 1;
            n->unk98 = 2;
            e = OS_DisableInterrupts();
            func_0203d560(&data_022b9020, n, 1);
            OS_RestoreInterrupts(e);
        } else {
            func_0200bad8(n->file38);
            e = OS_DisableInterrupts();
            func_0203d2d0(&data_022b9020, n);
            OS_RestoreInterrupts(e);
        }
    }
}
