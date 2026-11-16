#include <ov000_d/scene.h>

typedef struct {
    u8 pad0[3];
    u8 unk3;
} UnkStruct_02359af4_Sync;

UnkStruct_02359af4_Sync *func_020514e4(void);
void func_02002ab0(s32 a, s32 b, s32 c);
void func_ov000_023593bc(void);

#pragma thumb on

void func_ov000_02359af4(UnkStruct_SceneTask *t) {
    UnkStruct_Scene *w;

    if (t == NULL) {
        t = gTaskCurrent;
    }
    w = t->work;
    w->unk0 = 0;
    w->unk20 = 1;
    w->unk18 = 4;
    func_02040ec0(&w->pos[6], 9);
    func_02040ec0(&w->pos[7], 0xb);
    func_02040ec0(&w->pos[0], 1);
    func_02040ec0(&w->pos[1], 2);
    func_02040ec0(&w->pos[2], 3);
    func_02040ec0(&w->pos[3], 4);
    func_02040ec0(&w->pos[4], 5);
    func_02040ec0(&w->pos[5], 7);
    func_ov000_02357d20(w);
    func_ov000_02357c8c(w);
    func_ov000_02357b4c(w);
    func_ov000_02357b24(w);
    func_ov000_02357be4(w);
    func_ov000_02357ae8(w);
    func_ov000_02356810(w);
    if (w->unk14 != 2) {
        func_ov000_023582b0(w);
    } else {
        func_ov000_02358278(w);
    }
    func_02002ab0(1, 0, 1);
    *(vu16 *)0x400000e = (*(vu16 *)0x400000e & 0x43) | 0x69c;
    w->unk334 = 1;
    w->unk338 = 1;
    t->unk8 = func_ov000_023593bc;
    func_020514e4()->unk3 = 0;
}

#pragma thumb reset
