#include <ov000_d/mdl.h>

typedef struct {
    s32 t;
    s32 b;
    s32 l;
    s32 r;
    s32 n;
    s32 f;
} UnkFrustum_02356d88;

typedef struct {
    UnkVec_Mdl pos;
    UnkVec_Mdl up;
    UnkVec_Mdl at;
} UnkCam_02356d88;

void func_ov000_02356994(UnkFrustum_02356d88 *p);
void func_ov000_0235692c(UnkCam_02356d88 *cam);
void func_ov000_02356be8(UnkStruct_Mdl *w);
void func_ov000_02356a48(UnkStruct_Mdl *w);
void func_ov000_023569d8(UnkStruct_Mdl *w);

#pragma thumb on

void func_ov000_02356d88(UnkStruct_Mdl *w) {
    UnkCam_02356d88 cam;
    UnkFrustum_02356d88 fr;

    fr.t = 0xbe00;
    fr.b = -0xc000;
    fr.l = -0x10000;
    fr.r = 0xfe00;
    fr.n = 0x1000;
    fr.f = 0x64000;
    func_ov000_02356994(&fr);
    cam.pos.x = 0;
    cam.pos.y = 0x6000;
    cam.pos.z = 0;
    cam.up.x = 0;
    cam.up.y = 0x1000;
    cam.up.z = 0;
    cam.at.x = 0;
    cam.at.y = 0x6000;
    cam.at.z = -0x16000;
    func_ov000_0235692c(&cam);
    switch (w->mode) {
    case 0:
        func_ov000_02356be8(w);
        break;
    case 1:
        func_ov000_02356a48(w);
        break;
    case 2:
        func_ov000_023569d8(w);
        break;
    }
}

#pragma thumb reset
