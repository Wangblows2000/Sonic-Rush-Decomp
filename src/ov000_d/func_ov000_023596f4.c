#include <ov000_d/scene.h>

void WirelessSession_Stop(void);
void func_ov000_02358548(void);

#pragma thumb on

void func_ov000_023596f4(UnkStruct_SceneTask *t) {
    UnkStruct_Scene *w;

    if (t == NULL) {
        t = gTaskCurrent;
    }
    w = t->work;
    w->unk0 = 0;
    func_ov000_02357d20(w);
    func_ov000_02357c8c(w);
    Task_DestroyRange(0x4000, 0x4000);
    WirelessSession_Stop();
    w->unk334 = 0;
    t->unk8 = func_ov000_02358548;
}

#pragma thumb reset
