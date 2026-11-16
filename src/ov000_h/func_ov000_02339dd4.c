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

extern UnkStruct_02339dd4_Task *data_022b4574;
extern UnkStruct_02339dd4 data_ov000_0236aad8;

void func_ov000_0233877c(void);
void func_ov000_02339c2c(void *w);
void func_ov000_023425d0(void);
void func_ov000_023423b8(void);
void func_ov000_02341ebc(void);
void func_ov000_0233e668(void);
void func_ov000_0233b684(void);

void func_ov000_02339dd4(void) {
    void *work = data_022b4574->work;
    u32 i;
    UnkPlayer_02339dd4 *p;
    s32 v;

    data_ov000_0236aad8.prevFlags = data_ov000_0236aad8.flags;
    data_ov000_0236aad8.p[0].prevX = data_ov000_0236aad8.p[0].x;
    data_ov000_0236aad8.p[0].prevY = data_ov000_0236aad8.p[0].y;
    data_ov000_0236aad8.p[1].prevX = data_ov000_0236aad8.p[1].x;
    data_ov000_0236aad8.p[1].prevY = data_ov000_0236aad8.p[1].y;
    if (data_ov000_0236aad8.flags & 1) {
        v = !(data_ov000_0236aad8.flags & 2) ? data_ov000_0236aad8.p[0].unk20 * 0x110 : data_ov000_0236aad8.p[0].unk20 * -0x110;
        data_ov000_0236aad8.unkD8 = v;
        data_ov000_0236aad8.unkD0 = data_ov000_0236aad8.p[0].unk20 * 0x50;
    }
    func_ov000_0233877c();
    func_ov000_02339c2c(work);
    func_ov000_023425d0();
    func_ov000_023423b8();
    func_ov000_02341ebc();
    func_ov000_0233e668();
    p = data_ov000_0236aad8.p;
    for (i = 0; i < 2; i++, p++) {
        p->x += p->vx;
        p->y += p->vy;
    }
    func_ov000_0233b684();
    p = data_ov000_0236aad8.p;
    for (i = 0; i < 2; i++, p++) {
        p->vx = p->vy = 0;
        p->unk18 = p->unk1A = 0;
    }
}
