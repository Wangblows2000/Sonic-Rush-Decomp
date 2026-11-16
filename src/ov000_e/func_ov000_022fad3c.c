#include <ov000_c/stageobj.h>

typedef struct {
    u8 unk0;
    u8 unk1;
    s16 unk2;
    s16 unk4;
    s8 unk6;
    s8 unk7;
    u8 unk8;
    u8 unk9;
    u16 unkA;
} UnkStruct_ObjSpawn;

typedef UnkStruct_StageObj *(*ObjCtor)(UnkStruct_ObjSpawn *e, s32 x, s32 y, u8 i);

extern UnkStruct_ObjSpawn data_ov000_02369bac[];
extern ObjCtor data_ov000_0236498c[];

u8 func_ov000_022faeac(void);
void func_ov000_022fae34(UnkStruct_ObjSpawn *e);

UnkStruct_StageObj *func_ov000_022fad3c(s32 type, s32 x, s32 y, s16 d, s8 e, s8 f, u8 g, u8 h, u8 i) {
    UnkStruct_StageObj *o = NULL;
    s16 idx;
    UnkStruct_ObjSpawn *ent;

    idx = func_ov000_022faeac();
    if (idx < 0x28) {
        data_ov000_02369bac[idx].unk0 = 0xff;
        data_ov000_02369bac[idx].unk1 = 0xff;
        data_ov000_02369bac[idx].unk2 = type;
        data_ov000_02369bac[idx].unk4 = d;
        data_ov000_02369bac[idx].unk6 = e;
        data_ov000_02369bac[idx].unk7 = f;
        data_ov000_02369bac[idx].unk8 = g;
        data_ov000_02369bac[idx].unk9 = h;
        data_ov000_02369bac[idx].unkA = 0;
        ent = &data_ov000_02369bac[idx];
        o = data_ov000_0236498c[type](ent, x, y, i);
        if (o == NULL)
            func_ov000_022fae34(ent);
    }
    return o;
}
