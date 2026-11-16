#include <nitro/types.h>

typedef struct {
    u8 pad0[0x32C];
    u32 unk32C;
    u8 pad330[0x3B4 - 0x330];
    s16 unk3B4;
    u8 pad3B6[0x3CA - 0x3B6];
    s16 unk3CA;
} UnkStruct_023428dc_Pl;

typedef struct {
    void *spr;
    u16 state;
    u8 pad6[2];
    UnkStruct_023428dc_Pl *pl;
} UnkStruct_023428dc;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_023428dc *work;
} UnkStruct_023428dc_T;

extern UnkStruct_023428dc_T *data_022b4574;
extern u8 data_0225044c;
extern u32 data_02087cb8;

void func_0201c070(void *spr, s32 a);
void func_0201bf1c(void *spr);
void func_0201bfac(void *spr, s32 a);
void func_0204c598(u8 a, s32 b, u32 c);
void func_020371c8(void *task);

void func_ov000_023428dc(void) {
    UnkStruct_023428dc *w = data_022b4574->work;

    switch (w->state) {
    case 0x16:
        if (w->pl->unk3B4 == 0x5A) {
            func_0201c070(w->spr, 0x5A);
            func_0201bf1c(w->spr);
            func_0204c598(data_0225044c, 0x5A, w->pl->unk32C & 0x4000000);
            func_020371c8(data_022b4574);
        } else if (w->pl->unk3B4 == 0) {
            func_0201c070(w->spr, 0);
            func_0204c598(data_0225044c, 0, w->pl->unk32C & 0x4000000);
            func_020371c8(data_022b4574);
        }
        break;
    case 0x17:
        if (data_02087cb8 & 8) {
            func_0201bfac(w->spr, 1);
        } else {
            func_0201bfac(w->spr, 0);
        }
        if (w->pl->unk3CA == 0) {
            func_0201c070(w->spr, 0);
            func_0204c598(data_0225044c, 0x3C, w->pl->unk32C & 0x4000000);
            func_020371c8(data_022b4574);
        }
        break;
    }
}
