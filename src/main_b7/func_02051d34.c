#include <nitro/types.h>

typedef struct {
    s32 kind;
    s32 unk4;
} UnkState_02051d34;

typedef struct {
    s32 unk0;
} UnkStruct_022c4560;

extern UnkState_02051d34 *gSysSound;
extern UnkStruct_022c4560 data_022c4560;
extern void *gSoundHeap;
void func_02052000(void);
void func_0201e7cc(s32 id, void *p);

#pragma thumb on
void func_02051d34(s32 x) {
    s32 kind;
    s32 id;
    if (x >= 6)
        return;
    if (x <= 1)
        kind = 0;
    else if (x <= 3)
        kind = 1;
    else if (x <= 5)
        kind = 2;
    else
        return;
    if (kind == gSysSound->kind)
        return;
    func_02052000();
    gSysSound->unk4 = 7;
    switch (kind) {
    case 0:
        id = 1;
        break;
    case 1:
        id = 0x15;
        break;
    case 2:
        if (data_022c4560.unk0 == 0)
            id = 2;
        else
            id = 3;
        break;
    }
    func_0201e7cc(id, gSoundHeap);
    gSysSound->kind = kind;
}
#pragma thumb reset
