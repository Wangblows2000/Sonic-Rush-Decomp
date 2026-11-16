#include <nitro/types.h>

typedef struct UnkStruct_022dc8cc {
    u8 pad[0x214];
    s32 unk214;
    u8 pad1[0xbe0 - 0x218];
    s32 unkBE0;
    s32 unkBE4;
    s32 unkBE8;
} UnkStruct_022dc8cc;

typedef struct UnkStruct_022dc8cc_7 {
    u8 pad[4];
    s32 unk4;
    u16 unk8;
} UnkStruct_022dc8cc_7;

extern UnkStruct_022dc8cc_7 gBossStatus;
extern s16 data_ov000_02363580[];
extern u8 data_ov000_02363548[];
extern s8 data_ov000_02363688[][4];

void func_ov000_022dc858(UnkStruct_022dc8cc *s);
BOOL func_ov000_022db704(u32 mask, s32 n);
void func_ov000_022de39c(UnkStruct_022dc8cc *s, s32 k);

void func_ov000_022dc8cc(UnkStruct_022dc8cc *s) {
    s32 k;
    if (gBossStatus.unk4 == 0x16)
        return;
    s->unkBE8--;
    if (s->unkBE8 > 0)
        return;
    if (s->unkBE0 == 1) {
        if (s->unkBE4 > 0) {
            func_ov000_022dc858(s);
            return;
        }
        s->unkBE0 = 0;
        s->unkBE8 = data_ov000_02363580[gBossStatus.unk8 - 1];
        return;
    }
    if (func_ov000_022db704(data_ov000_02363548[gBossStatus.unk8 - 1], 8)) {
        if (gBossStatus.unk4 == 0x15) {
            k = 0;
        } else {
            k = data_ov000_02363688[gBossStatus.unk8 - 1][s->unk214];
            s->unk214++;
            s->unk214 %= 4;
        }
        func_ov000_022de39c(s, k);
        return;
    }
    s->unkBE8 = data_ov000_02363580[gBossStatus.unk8 - 1];
}
