#include <nitro/types.h>

typedef struct {
    u8 pad0[0xBE4];
    s32 unkBE4;
} UnkStruct_022de39c;

typedef struct {
    u8 pad[8];
    u16 unk8;
} UnkStruct_02369ac0;

extern UnkStruct_02369ac0 gBossStatus;
extern s16 data_ov000_023635a0[];

void func_ov000_022dc858(UnkStruct_022de39c *s);
void func_ov000_022dde20(UnkStruct_022de39c *s);
void func_ov000_022dd908(UnkStruct_022de39c *s);

void func_ov000_022de39c(UnkStruct_022de39c *s, s32 n) {
    switch (n) {
    case 0:
        s->unkBE4 = data_ov000_023635a0[gBossStatus.unk8 - 1];
        func_ov000_022dc858(s);
        break;
    case 1:
        func_ov000_022dde20(s);
        break;
    case 2:
        func_ov000_022dd908(s);
        break;
    }
}
