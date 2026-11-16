#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1c];
    u8 stage;
    u8 act;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[0x34];
    s32 vram;
    u8 pad38[0x10];
} UnkSpr_0205eb68;

typedef struct {
    u8 pad0[0xd0];
    u16 palD0;
    u8 padd2[0x458 - 0xd2];
    s32 vram458;
    u8 pad45c[0xc];
    u16 pal468;
    u8 pad46a[0x4dc - 0x46a];
    UnkSpr_0205eb68 spr[14];
} UnkWork_0205eb68;

typedef struct {
    u8 pad0[0x10];
    UnkWork_0205eb68 *work;
} UnkTask_0205eb68;

typedef struct {
    u8 pad0[0x1c];
    u32 unk1c;
} UnkDisp_0205eb68;

extern UnkStruct_022c4560 data_022c4560;
extern UnkDisp_0205eb68 data_0208922c;
extern UnkDisp_0205eb68 data_02087d04;

void func_0205e218(UnkWork_0205eb68 *w);
void func_02039bb0(s32 a, s32 b);
void func_0206d3fc(u8 idx);

void func_0205eb68(UnkTask_0205eb68 *t) {
    UnkWork_0205eb68 *w = t->work;
    u32 i;
    BOOL special;

    func_0205e218(w);
    for (i = 0; i < 14; i++)
        func_02039bb0(0, w->spr[i].vram);
    func_02039bb0(0, w->vram458);
    if (data_022c4560.stage != 8 || data_022c4560.act != 2) {
        func_0206d3fc(w->palD0);
        func_0206d3fc(w->pal468);
    }
    special = FALSE;
    if (data_022c4560.stage == 8 && data_022c4560.act == 2)
        special = TRUE;
    if (special || data_022c4560.stage >= 9 || data_022c4560.act >= 2) {
        data_0208922c.unk1c = 0;
        data_02087d04.unk1c = 0;
    }
}
