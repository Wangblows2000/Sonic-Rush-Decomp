#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    s32 unkc;
    u8 pad10[0x18];
} UnkItem_022ea2f0;

typedef struct {
    UnkItem_022ea2f0 items[5];
} UnkStruct_022ea2f0;

extern u32 gSystemFlags;

u16 *TimeAttackMenu_Unk_ov001_022ea27c(UnkStruct_022ea2f0 *p);

#pragma thumb on
void TimeAttackMenu_Unk_ov001_022ea2f0(UnkStruct_022ea2f0 *p) {
    u16 *buf = TimeAttackMenu_Unk_ov001_022ea27c(p);
    s32 i;
    s32 j;
    s32 end;

    if (buf != NULL) {
        for (i = 0; i < 8; i++) {
            buf[i] = 0;
        }
        for (j = 0; j < 5; j++) {
            u16 v = -p->items[j].unkc;
            for (end = i + 0x18; i < end; i++) {
                buf[i] = v;
            }
        }
        for (; i < 0xc0; i++) {
            buf[i] = 0;
        }
        gSystemFlags |= 0x1000;
    }
}
#pragma thumb reset
