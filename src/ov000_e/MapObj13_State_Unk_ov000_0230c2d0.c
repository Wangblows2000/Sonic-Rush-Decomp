#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[4];
    s32 unk4;
    u8 pad8[0x34 - 8];
    u32 unk34;
    u8 pad38[0x64 - 0x38];
} UnkStruct_0236aad8_E;

typedef struct {
    UnkStruct_0236aad8_E e[2];
} UnkStruct_0236aad8;

extern UnkStruct_0236aad8 data_ov000_0236aad8;
extern s16 data_022c4ef8;

void func_ov000_0230c290(UnkStruct_StageObj *w);

void MapObj13_State_Unk_ov000_0230c2d0(UnkStruct_StageObj *w) {
    s32 x = 0;

    if (w->unk88 != NULL)
        ((UnkStruct_StageObj *)w->unk88)->unkC++;
    if (data_ov000_0236aad8.e[1].unk34 & 1)
        x = data_ov000_0236aad8.e[1].unk4;
    if (data_ov000_0236aad8.e[0].unk34 & 1)
        x = data_ov000_0236aad8.e[0].unk4;
    if (w->unk10 - 0x2000 < x) {
        w->unk8 = 0x8000 - (w->unk10 - x);
        w->unk2E = 0;
        func_ov000_0230c290(w);
        return;
    }
    if (data_022c4ef8 == 0)
        w->unk54 |= 4;
}
