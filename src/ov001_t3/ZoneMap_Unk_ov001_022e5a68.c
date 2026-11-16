#include <nitro/types.h>

typedef struct {
    u8 pad0[0xf];
    u8 unkf;
} UnkStruct_022c4680;

typedef struct {
    u8 pad0[7];
    u8 unk7;
} UnkStruct_022c46e0;

typedef struct {
    s32 mode;
    u8 pad4[4];
    s32 unk8;
    u8 padc[0x74 - 0xc];
    s32 unk74;
    u8 pad78[0x9c - 0x78];
    s32 unk9c;
    s32 unka0;
} UnkWork_022e5a68;

extern UnkStruct_022c4680 gSaveData;
extern UnkStruct_022c46e0 data_022c46e0;

s32 NaviM_Unk_02064828(void);
void NaviM_Unk_02064740(u16 a);
void SaveData_CommitOrShowError(s32 a);

#pragma thumb on
BOOL ZoneMap_Unk_ov001_022e5a68(UnkWork_022e5a68 *w) {
    s32 r;
    BOOL save;
    s32 idx;
    u8 cur;
    BOOL found;
    s32 i;
    u32 a;
    u32 b;

    found = FALSE;
    save = FALSE;
    r = NaviM_Unk_02064828();
    if (w->mode == 0) {
        cur = gSaveData.unkf;
        b = data_022c46e0.unk7 & 0x7f;
        a = cur & 0x7f;
        if (a > b) {
            for (i = 6; i >= 0; i--) {
                if ((a & (1 << i)) && !(b & (1 << i))) {
                    idx = i;
                    found = TRUE;
                    break;
                }
            }
        }
        if (found == TRUE) {
            data_022c46e0.unk7 = cur;
            save = TRUE;
        }
    } else if (w->unk8 == 4) {
        found = TRUE;
        idx = 0;
    } else if (w->unk8 == 0xc) {
        found = TRUE;
        idx = 1;
    } else if (w->unk8 == 0xe) {
        found = TRUE;
        idx = 2;
    } else if (w->unk8 == 0x12) {
        found = TRUE;
        idx = 3;
    } else if (w->unk8 == 0x16) {
        found = TRUE;
        idx = 4;
    } else if (w->unk8 == 0x1a) {
        found = TRUE;
        idx = 5;
    } else if (w->unk8 == 0x1e) {
        found = TRUE;
        idx = 6;
    }
    if (r == 0 && !found) return FALSE;
    w->unk74 = 0;
    w->unk9c = found;
    w->unka0 = r;
    if (found) NaviM_Unk_02064740(idx);
    if (save == TRUE) SaveData_CommitOrShowError(0);
    return TRUE;
}
#pragma thumb reset
