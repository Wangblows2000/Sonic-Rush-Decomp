#include <nitro/types.h>

typedef struct {
    u16 wave[2];
    u8 originalKey;
    u8 attack;
    u8 decay;
    u8 sustain;
    u8 release;
    u8 pan;
} UnkInstParam_02009be8;

typedef struct {
    u8 type;
    u8 padding_;
    UnkInstParam_02009be8 param;
} UnkInstData_02009be8;

typedef struct {
    u8 min;
    u8 max;
    UnkInstData_02009be8 instOffset[1];
} UnkDrumSet_02009be8;

typedef struct {
    u8 key[8];
    UnkInstData_02009be8 instOffset[1];
} UnkKeySplit_02009be8;

typedef struct {
    u8 pad0[0x38];
    u32 instCount;       /* 0x38 */
    u32 instOffset[1];   /* 0x3c */
} UnkBankData_02009be8;

typedef struct {
    u32 instIndex;
    u32 index;
} UnkInstIter_02009be8;

BOOL SND_GetNextInstData(const UnkBankData_02009be8 *bank, UnkInstData_02009be8 *inst, UnkInstIter_02009be8 *iter)
{
    for (; iter->instIndex < bank->instCount; iter->instIndex++, iter->index = 0) {
        u32 off = bank->instOffset[iter->instIndex];
        inst->type = (u8)off;
        switch (inst->type) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            inst->param = *(const UnkInstParam_02009be8 *)((u8 *)bank + (off >> 8));
            iter->instIndex++;
            return TRUE;
        case 16: {
            const UnkDrumSet_02009be8 *drumSet = (const UnkDrumSet_02009be8 *)((u8 *)bank + (off >> 8));
            while (iter->index < drumSet->max - drumSet->min + 1) {
                *inst = drumSet->instOffset[iter->index];
                iter->index++;
                return TRUE;
            }
            break;
        }
        case 17: {
            const UnkKeySplit_02009be8 *keySplit = (const UnkKeySplit_02009be8 *)((u8 *)bank + (off >> 8));
            while (iter->index < 8) {
                if (keySplit->key[iter->index] == 0) {
                    break;
                }
                *inst = keySplit->instOffset[iter->index];
                iter->index++;
                return TRUE;
            }
            break;
        }
        }
    }
    return FALSE;
}
