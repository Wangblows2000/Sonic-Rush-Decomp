#include <nitro/types.h>

typedef struct {
    u8 pad0[0x4b4];
    u8 unk4b4;
    u8 pad4b5[7];
    void *unk4bc;
} UnkObj_02017ec0;

typedef struct {
    UnkObj_02017ec0 *unk0;
    UnkObj_02017ec0 *unk4;
    u8 pad8[4];
    u8 unkC;
    u8 padD;
    u8 unkE;
} UnkStruct_02080e74;

extern UnkStruct_02080e74 data_02080e74;

void MBi_ClearSendStatus(void);

static inline UnkObj_02017ec0 *GetNext(UnkObj_02017ec0 *first)
{
    UnkObj_02017ec0 *cur = data_02080e74.unk4;
    if (cur == NULL || cur->unk4bc == NULL) {
        return first;
    }
    return cur->unk4bc;
}

BOOL func_02017ec0(void)
{
    UnkObj_02017ec0 *obj = data_02080e74.unk0;
    UnkObj_02017ec0 *next;
    if (obj == NULL) {
        return FALSE;
    }
    data_02080e74.unk4 = GetNext(obj);
    MBi_ClearSendStatus();
    data_02080e74.unkE = data_02080e74.unk4->unk4b4;
    data_02080e74.unkC = 2;
    return TRUE;
}
