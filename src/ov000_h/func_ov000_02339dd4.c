#include <nitro/types.h>

typedef struct {
    u32 unk0;
    s32 x;
    s32 y;
    s32 prevX;
    s32 prevY;
    s16 vx;
    s16 vy;
    s16 unk18;
    s16 unk1A;
    u8 pad1C[0x20 - 0x1C];
    s16 unk20;
    u8 pad22[0x64 - 0x22];
} UnkPlayer_02339dd4;

typedef struct {
    UnkPlayer_02339dd4 p[2];
    u32 flags;
    u32 prevFlags;
    s32 unkD0;
    s32 unkD4;
    s32 unkD8;
} UnkStruct_02339dd4;

typedef struct {
    u8 pad0[0x10];
    void *work;
} UnkStruct_02339dd4_Task;

extern UnkStruct_02339dd4_Task *gTaskCurrent;
extern UnkStruct_02339dd4 gCamera;

void func_ov000_0233877c(void);
void func_ov000_02339c2c(void *w);
void func_ov000_023425d0(void);
void func_ov000_023423b8(void);
void func_ov000_02341ebc(void);
void func_ov000_0233e668(void);
void func_ov000_0233b684(void);

void func_ov000_02339dd4(void) {
    void *work = gTaskCurrent->work;
    u32 i;
    UnkPlayer_02339dd4 *p;
    s32 v;

    gCamera.prevFlags = gCamera.flags;
    gCamera.p[0].prevX = gCamera.p[0].x;
    gCamera.p[0].prevY = gCamera.p[0].y;
    gCamera.p[1].prevX = gCamera.p[1].x;
    gCamera.p[1].prevY = gCamera.p[1].y;
    if (gCamera.flags & 1) {
        v = !(gCamera.flags & 2) ? gCamera.p[0].unk20 * 0x110 : gCamera.p[0].unk20 * -0x110;
        gCamera.unkD8 = v;
        gCamera.unkD0 = gCamera.p[0].unk20 * 0x50;
    }
    func_ov000_0233877c();
    func_ov000_02339c2c(work);
    func_ov000_023425d0();
    func_ov000_023423b8();
    func_ov000_02341ebc();
    func_ov000_0233e668();
    p = gCamera.p;
    for (i = 0; i < 2; i++, p++) {
        p->x += p->vx;
        p->y += p->vy;
    }
    func_ov000_0233b684();
    p = gCamera.p;
    for (i = 0; i < 2; i++, p++) {
        p->vx = p->vy = 0;
        p->unk18 = p->unk1A = 0;
    }
}
