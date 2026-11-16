#include <nitro/types.h>

typedef struct UnkStruct_022dbb18 {
    s32 unk0;
    s32 unk4;
} UnkStruct_022dbb18;

extern s32 gGameState[];
extern UnkStruct_022dbb18 data_ov000_02363600[];
extern u8 data_ov000_02363540;
extern UnkStruct_022dbb18 data_ov000_023635d0[];
extern void *data_ov000_02369b30[];
extern u8 data_ov000_02365700;

void func_ov000_022c5d4c(void *a, void *b, void *c, void *d, s32 e);

void func_ov000_022dbb18(BOOL flag) {
    s32 i = gGameState[0];
    UnkStruct_022dbb18 *p;
    if (flag)
        p = &data_ov000_02363600[i];
    else
        p = &data_ov000_023635d0[i];
    func_ov000_022c5d4c(data_ov000_02369b30[0x8 / 4], &data_ov000_02365700, p, &data_ov000_02363540, 4);
}
