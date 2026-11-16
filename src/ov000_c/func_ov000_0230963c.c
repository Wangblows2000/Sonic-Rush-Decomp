#include <ov000_c/stageobj.h>

typedef struct {
    u32 unk0;
    u8 pad4[0x34 - 4];
    u32 unk34;
    u8 pad38[0x64 - 0x38];
} UnkStruct_Plr;

typedef struct {
    u8 pad0[0xE];
    u16 unkE;
    u16 unk10;
} UnkStruct_Map;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    s8 unk210[2];
    u8 pad212[2];
    u8 unk214;
    u8 pad215;
    u16 unk216;
    u16 unk218;
    u16 unk21A;
    u16 unk21C;
} UnkStruct_0230963c;

extern UnkStruct_Plr data_ov000_0236aad8[];
extern UnkStruct_Map data_ov000_0236abd8;
extern UnkStruct_StageObj *data_ov000_0236ac60[];

void func_ov000_02342348(s32 a);
void func_ov000_023355e0(u16 x0, u16 y0, u16 x1, u16 y1);
u8 func_ov000_02307e84(UnkStruct_0230963c *w);
void func_ov000_023423a4(s32 a, UnkStruct_0230963c *w, s32 b);
void func_ov000_0232d620(void);
void func_ov000_023094f0(void);

#define CLAMP(x, lo, hi) ((x) < (lo) ? (lo) : ((x) > (hi) ? (hi) : (x)))

void func_ov000_0230963c(UnkStruct_0230963c *w) {
    u8 i;
    UnkStruct_StageObj *p;
    s32 px, py;

    func_ov000_02342348(w->unk214);
    w->unk20C |= 8;
    if (!(data_ov000_0236aad8[2].unk0 & 1)) {
        u16 x = w->base.unk10 >> 8;
        u16 y = w->base.unk14 >> 8;
        s32 x0, y0, x1, y1;
        u16 mw = data_ov000_0236abd8.unkE;
        u16 mh = data_ov000_0236abd8.unk10;
        x0 = CLAMP(x - 0x300, 0, mw - 1);
        y0 = CLAMP(y - 0x300, 0, mh - 1);
        x1 = CLAMP(x + 0x300, 0, mw - 1);
        y1 = CLAMP(y + 0x300, 0, mh - 1);
        func_ov000_023355e0(x0, y0, x1, y1);
        w->unk214 = func_ov000_02307e84(w);
        func_ov000_023423a4(1, w, w->unk214);
    }
    w->unk20C |= 0x1000;
    w->unk20C |= 0x10;
    for (i = 0; i < 2; i = (u8)(i + 1)) {
        if (w->unk210[i] >= 0) {
            p = data_ov000_0236ac60[w->unk210[i]];
            px = p->unk10 >> 8;
            if (w->unk216 <= px && px <= w->unk21A) {
                py = p->unk14 >> 8;
                if (w->unk218 <= py && py <= w->unk21C)
                    w->unk20C |= 2 << i;
            }
        }
    }
    if (w->unk20C & 2)
        func_ov000_0232d620();
    if (data_ov000_0236aad8[2].unk0 & 1) {
        data_ov000_0236aad8[0].unk34 |= 0x10000;
        data_ov000_0236aad8[1].unk34 |= 0x10000;
    } else {
        if (w->unk20C & 2)
            data_ov000_0236aad8[0].unk34 |= 0x10000;
        if (w->unk20C & 4)
            data_ov000_0236aad8[1].unk34 |= 0x10000;
    }
    w->base.unk68 = func_ov000_023094f0;
}
