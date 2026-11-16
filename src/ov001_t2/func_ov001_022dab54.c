#include <nitro/types.h>

typedef struct {
    s32 idx;
    u8 sub[0x1c];
    s32 unk90;
    s32 unk94;
    s32 unk98;
    u8 pad9c[8];
} UnkEnt_022dab54;

typedef struct {
    u8 pad0[0x70];
    UnkEnt_022dab54 ent[8];
} UnkStruct_022dab54;

void func_02050f70(void);
s32 func_02050f60(void);
s32 func_02050ee4(s32 i);
s32 func_02050ec0(s32 i);
s32 func_02050f04(s32 i, u8 *buf, s32 n);
void func_ov001_022d9a54(void *p, u8 *buf, u16 n);
void func_ov001_022d9814(UnkStruct_022dab54 *p, s32 i);

#pragma thumb on
BOOL func_ov001_022dab54(UnkStruct_022dab54 *p) {
    s32 n;
    s32 i;
    u8 buf[20];

    func_02050f70();
    n = func_02050f60();
    for (i = 0; i < 8; i++) {
        if (i < n) {
            p->ent[i].unk90 = func_02050ee4(i);
            p->ent[i].unk98 = func_02050ec0(i);
            p->ent[i].unk94 = 2;
            p->ent[i].idx = i;
            func_ov001_022d9a54(p->ent[i].sub, buf, func_02050f04(i, buf, 7));
        } else {
            p->ent[i].unk90 = 0;
            p->ent[i].unk98 = 0;
            p->ent[i].unk94 = 2;
            p->ent[i].idx = i;
            func_ov001_022d9814(p, i);
        }
    }
    return TRUE;
}
#pragma thumb reset
