#include <nitro/types.h>

typedef struct {
    s32 unk0;
    u8 pad4[0xa];
    u16 unke;
    u8 pad10[4];
    BOOL unk14;
    u8 pad18[0x10];
    u16 unk28;
} UnkStruct_022c4560;

typedef struct {
    u16 unk0;
    u8 unk2;
    u8 unk3;
    u8 pad4[0x54];
    u16 unk58;
    u8 unk5a;
} UnkStruct_022c468c;

typedef struct {
    u8 pad0[0x14];
    u8 unk14;
} UnkStruct_022c4590;

typedef struct {
    u8 pad0[8];
    u32 unk8;
} UnkStruct_022c4680;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_022c468c data_022c468c;
extern UnkStruct_022c4590 data_022c4590;
extern UnkStruct_022c4680 gSaveData;
extern u8 data_02074bb4[];

/* enum-typed ternary keeps CW from canonicalising to !x (movne 0 before moveq 1) */
typedef enum { E_ZERO, E_ONE } UnkEnum_0204ec40;

u16 StoryProgress_Unk_0204e614(u16 v);

void StoryProgress_RestoreFromSave(void)
{
    UnkStruct_022c468c *p = &data_022c468c;
    u8 v;
    u8 i;

    if (gGameState.unk0 != 1) {
        gGameState.unke = StoryProgress_Unk_0204e614(p->unk0);
        v = p->unk2;
        if (v > 99)
            v = 99;
        data_022c4590.unk14 = v;
        gGameState.unk28 = p->unk3;
    } else {
        gGameState.unke = StoryProgress_Unk_0204e614(p->unk58);
        v = p->unk5a;
        if (v > 99)
            v = 99;
        data_022c4590.unk14 = v;
        gGameState.unk28 = 0;
        for (i = 0; i < 7; i++) {
            if (p->unk58 >= data_02074bb4[i])
                gGameState.unk28 |= 1 << i;
        }
    }
    gGameState.unk14 = (gSaveData.unk8 & 1) ? E_ZERO : E_ONE;
}
