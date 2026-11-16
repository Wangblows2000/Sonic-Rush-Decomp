#include <nitro/types.h>

typedef struct {
    u8 pad0[0xE4];
    void *unkE4[5];
    u8 padF8[0x10C - 0xF8];
    u8 unk10C[5];
} UnkStruct_02327f24_M;

typedef struct UnkStruct_02327f24 {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
    u8 pad10[0x54 - 0x10];
    u32 unk54;
    u8 pad58[0x5C - 0x58];
    u16 unk5C;
    u8 pad5E[0x134 - 0x5E];
    UnkStruct_02327f24_M unk134;
    u8 pad[0x2A4 - 0x134 - sizeof(UnkStruct_02327f24_M)];
    void (*unk2A4)(struct UnkStruct_02327f24 *w);
    void *unk2A8;
    void *unk2AC;
} UnkStruct_02327f24;

s32 func_02025e8c(void *a);
void Model_SetAnimation(void *m, s32 a, void *b, s32 c, s32 d);

void func_ov000_02327f24(UnkStruct_02327f24 *w) {
    UnkStruct_02327f24_M *m = &w->unk134;
    u32 i;

    if (w->unk8 == 0) {
        if (w->unk2A4 != NULL)
            w->unk2A4(w);
        if (w->unkC == 0) {
            for (i = 0; i < 5; i++) {
                if (m->unkE4[i] != NULL && !(m->unk10C[i] & 2))
                    break;
            }
            if (i == 5)
                w->unk54 |= 4;
        } else if (w->unkC > 0) {
            w->unkC--;
            if (w->unkC == 0)
                w->unk54 |= 4;
        }
    } else {
        w->unk8--;
        if (w->unk8 != 0)
            return;
        if (w->unk2AC != NULL)
            Model_SetAnimation(m, 2, w->unk2AC, 0, func_02025e8c(w->unk2A8));
        w->unk5C &= ~0x30;
    }
}
