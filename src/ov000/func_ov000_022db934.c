#include <nitro/types.h>

typedef struct UnkStruct_022db934 {
    u8 pad[8];
    u16 unk8;
} UnkStruct_022db934;

extern UnkStruct_022db934 gBossStatus;
extern s32 data_ov000_02363668[];

s32 func_ov000_022db934(void) {
    return data_ov000_02363668[gBossStatus.unk8 - 1];
}
