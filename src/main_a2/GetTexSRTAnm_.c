#include <nitro/types.h>

typedef struct { u8 revision; u8 numEntry; u16 sizeDictBlk; u16 dummy; u16 ofsEntry; } UnkDict_02027ec8;
typedef struct { u16 sizeUnit; u16 ofsName; u8 data[4]; } UnkDictEntry_02027ec8;
typedef struct { u8 unk0[8]; UnkDict_02027ec8 dict; } UnkResAnm_02027ec8;
typedef struct {
    u32 flag;
    u8 unk4[0x14];
    fx32 scaleS;
    fx32 scaleT;
    u16 sinR;
    u16 cosR;
    fx32 transS;
    fx32 transT;
} UnkTexSRT_02027ec8;

s32 func_020280d4(UnkResAnm_02027ec8 *anm, u32 info, u32 data, s32 frame);
u32 func_02027fc8(UnkResAnm_02027ec8 *anm, u32 info, u32 data, s32 frame);

static inline void *GetData_02027ec8(UnkDict_02027ec8 *dict, u32 idx) {
    UnkDictEntry_02027ec8 *e = (UnkDictEntry_02027ec8 *)((u8 *)dict + dict->ofsEntry);
    return e->data + e->sizeUnit * idx;
}

void GetTexSRTAnm_(UnkResAnm_02027ec8 *anm, u16 idx, s32 frame, UnkTexSRT_02027ec8 *res) {
    u32 *data = GetData_02027ec8(&anm->dict, idx);
    u32 flag = res->flag;
    s32 a;
    s32 b;
    u32 rot;
    a = func_020280d4(anm, data[6], data[7], frame);
    b = func_020280d4(anm, data[8], data[9], frame);
    if (a == 0 && b == 0) {
        flag |= 4;
    } else {
        res->transS = a;
        res->transT = b;
        flag &= ~4;
    }
    rot = func_02027fc8(anm, data[4], data[5], frame);
    if (rot == 0x10000000) {
        flag |= 2;
    } else {
        res->sinR = rot;
        res->cosR = rot >> 16;
        flag &= ~2;
    }
    a = func_020280d4(anm, data[0], data[1], frame);
    b = func_020280d4(anm, data[2], data[3], frame);
    if (a == 0x1000 && b == 0x1000) {
        flag |= 1;
    } else {
        res->scaleS = a;
        res->scaleT = b;
        flag &= ~1;
    }
    res->flag = flag;
}
