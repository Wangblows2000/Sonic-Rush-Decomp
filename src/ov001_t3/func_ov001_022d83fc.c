#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
    u8 pad4[0x28];
} UnkAnim_022d83fc;

typedef struct UnkStruct_022d83fc {
    u8 pad0[0x258];
    s32 unk258;
} UnkStruct_022d83fc;

typedef struct {
    u16 ids[6];
    s32 vals[6];
    UnkAnim_022d83fc anims[6];
    UnkStruct_022d83fc *parent;
} UnkWork_022d83fc;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d83fc *work;
} UnkTask_022d83fc;

typedef struct {
    s16 x;
    s16 y;
} UnkPos_022d83fc;

typedef struct {
    u16 ids[3];
    UnkPos_022d83fc pos[3];
    s32 vals[3];
} UnkList_022d83fc;

extern UnkTask_022d83fc *gTaskCurrent;

void MIi_CpuClear16(u16 data, void *dest, u32 size);
void KeyframeAnim_Update(UnkAnim_022d83fc *a);
BOOL KeyframeAnim_IsFinished(UnkAnim_022d83fc *a);
void func_ov001_022d77a8(UnkList_022d83fc *l, UnkStruct_022d83fc *p);
void Task_Destroy(UnkTask_022d83fc *t);

#pragma thumb on
void func_ov001_022d83fc(void) {
    UnkWork_022d83fc *w;
    s32 i;
    BOOL done;
    s32 n;
    BOOL r;
    s16 x;
    vu16 zero;
    UnkList_022d83fc l;

    done = TRUE;
    w = gTaskCurrent->work;
    zero = 0;
    MIi_CpuClear16(zero, &l, sizeof(l));
    n = 0;
    for (i = 0; i < 6; i++) {
        KeyframeAnim_Update(&w->anims[i]);
        r = KeyframeAnim_IsFinished(&w->anims[i]);
        x = w->anims[i].x;
        if (x > -0x100 && x < 0x100 && n < 3) {
            l.pos[n].x = x;
            l.pos[n].y = 0;
            l.ids[n] = w->ids[i];
            l.vals[n] = w->vals[i];
            n++;
        }
        if (!r) done = FALSE;
    }
    for (; n < 3; n++) {
        l.pos[n].x = -0x100;
        l.ids[n] = 0;
        l.vals[n] = n;
    }
    func_ov001_022d77a8(&l, w->parent);
    if (done) {
        w->parent->unk258 = 1;
        Task_Destroy(gTaskCurrent);
    }
}
#pragma thumb reset
