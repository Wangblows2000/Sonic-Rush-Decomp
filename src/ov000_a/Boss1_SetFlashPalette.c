#include <nitro/types.h>

typedef struct UnkStruct_022cc35c {
    u8 pad[8];
    s32 unk8;
} UnkStruct_022cc35c;

extern UnkStruct_022cc35c data_ov000_02369ad0;
extern u16 data_ov000_02362b70[];
extern u16 data_ov000_02362b60[];
extern u16 data_ov000_02362b5c[];
extern u16 data_ov000_02362b6c[];
extern u16 data_ov000_02362b58[];
extern u32 data_ov000_02364f08[];

BOOL Boss1_Unk_ov000_022cc6d4(void);
void Boss_LoadModelPalettes(s32 a, u32 *p1, u16 *p2, u16 *p3, u16 n);

void Boss1_SetFlashPalette(s32 idx, s32 flag) {
    u16 *t;
    if (flag) {
        if (Boss1_Unk_ov000_022cc6d4())
            t = &data_ov000_02362b70[idx];
        else
            t = &data_ov000_02362b60[idx];
    } else {
        if (Boss1_Unk_ov000_022cc6d4())
            t = &data_ov000_02362b5c[idx];
        else
            t = &data_ov000_02362b6c[idx];
    }
    Boss_LoadModelPalettes(data_ov000_02369ad0.unk8, data_ov000_02364f08, t, data_ov000_02362b58, 1);
}
