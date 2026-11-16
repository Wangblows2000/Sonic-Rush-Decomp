#include <nitro/types.h>

typedef struct UnkStruct_022cf3e0_s {
    u8 pad[0x20c];
    u32 unk20C;
    u8 pad1[0x398 - 0x210];
    s32 unk398;
    u8 pad2[0x4d0 - 0x39c];
    s32 unk4D0;
    u8 pad3[0x1348 - 0x4d4];
    s32 unk1348;
    s32 unk134C;
    s32 unk1350;
} UnkStruct_022cf3e0_s;

typedef struct UnkStruct_022cf3e0 UnkStruct_022cf3e0;
struct UnkStruct_022cf3e0 {
    UnkStruct_022cf3e0_s *unk0;
    void (*func)(UnkStruct_022cf3e0 *t);
    s32 unk8;
    u8 pad[4];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
};

typedef struct UnkStruct_022cf3e0_pl {
    u8 pad[0x2e];
    s16 unk2E;
    u8 pad1[0x34 - 0x30];
    s16 unk34;
    u8 pad2[0x58 - 0x36];
    u32 unk58;
} UnkStruct_022cf3e0_pl;

typedef struct UnkStruct_022cf3e0_o {
    u8 pad[0x10];
    s32 unk10;
    u8 pad1[4];
    s32 unk18;
} UnkStruct_022cf3e0_o;

typedef struct UnkStruct_022cf3e0_2 {
    u8 pad[0x10];
    UnkStruct_022cf3e0 *unk10;
} UnkStruct_022cf3e0_2;

typedef struct UnkStruct_022cf3e0_cam {
    u8 pad[0x70];
    s32 unk70;
} UnkStruct_022cf3e0_cam;

typedef struct UnkStruct_022cf3e0_7 {
    u8 pad[0x62];
    u16 unk62;
    u8 pad1[0xe0 - 0x64];
    s32 unkE0;
    s32 unkE4;
} UnkStruct_022cf3e0_7;

typedef struct UnkStruct_022cf3e0_v {
    s16 x;
    s16 y;
    s16 z;
} UnkStruct_022cf3e0_v;

typedef struct UnkStruct_022cf3e0_4 {
    u8 pad[0x1c];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022cf3e0_4;

extern UnkStruct_022cf3e0_2 *gTaskCurrent;
extern UnkStruct_022cf3e0_pl *data_ov000_0236ac60;
extern UnkStruct_022cf3e0_cam data_ov000_0236a444;
extern UnkStruct_022cf3e0_7 data_ov000_0236aad8;
extern UnkStruct_022cf3e0_4 data_022c4560;
extern u16 data_ov000_02362df4[];
extern UnkStruct_022cf3e0_v data_ov000_02362ef8[];
extern u16 data_02074960[][3][3];
extern UnkStruct_022cf3e0_v data_020748bc[][3];

void func_02022270(s32 i, u16 a);
void func_02022284(s32 i, s32 x, s32 y, s32 z);
UnkStruct_022cf3e0_o *func_ov000_023293f4(UnkStruct_022cf3e0_s *s, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
UnkStruct_022cf3e0_o *func_ov000_023295a4(UnkStruct_022cf3e0_s *s, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void func_ov000_02329128(UnkStruct_022cf3e0_pl *p, s32 a, s32 b, s32 c, s32 d, s32 e);
void func_ov000_023294c8(UnkStruct_022cf3e0_pl *p);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);

void func_ov000_022cf3e0(void) {
    UnkStruct_022cf3e0 *t;
    UnkStruct_022cf3e0_s *s;
    UnkStruct_022cf3e0_pl *pl;
    s32 i;
    u16 h;
    pl = data_ov000_0236ac60;
    t = gTaskCurrent->unk10;
    s = t->unk0;
    if (t->func != NULL)
        t->func(t);
    if ((data_ov000_0236aad8.unk62 << 12) > -data_ov000_0236a444.unk70) {
        for (i = 0; i <= 2; i++) {
            func_02022270(i, data_ov000_02362df4[i]);
            func_02022284(i, data_ov000_02362ef8[i].x, data_ov000_02362ef8[i].y, data_ov000_02362ef8[i].z);
        }
    } else {
        for (i = 0; i <= 2; i++) {
            func_02022270(i, data_02074960[data_022c4560.unk1C][data_022c4560.unk1D][i]);
            func_02022284(i, data_020748bc[data_022c4560.unk1C][i].x, data_020748bc[data_022c4560.unk1C][i].y, data_020748bc[data_022c4560.unk1C][i].z);
        }
    }
    data_ov000_0236aad8.unk62 = (t->unk8 >> 12) + 0x200;
    {
        s32 v = -(data_ov000_0236aad8.unk62 << 12);
        s->unk4D0 = v;
        s->unk398 = v;
    }
    h = data_ov000_0236aad8.unk62;
    if (h <= 0x200) {
        s32 k = (-s->unk134C >> 12) + 0x34;
        if (h < k && h > k / 128.7) {
            if (t->unk16 == 0x18) {
                UnkStruct_022cf3e0_o *o = func_ov000_023293f4(s, 0, 1, 2, h, data_ov000_0236aad8.unkE0, data_ov000_0236aad8.unkE4);
                if (s->unk20C & 2) {
                    o->unk10 = s->unk1348 >> 4;
                    o->unk18 = s->unk1350 >> 4;
                }
                t->unk16 = 0;
            }
            t->unk16++;
        }
    }
    if (s->unk20C & 8) {
        if (data_ov000_0236aad8.unk62 > (-s->unk134C >> 12) + 0x34) {
            s->unk20C &= ~8;
            func_ov000_023295a4(s, 0, 0, data_ov000_0236aad8.unk62, 1, data_ov000_0236aad8.unkE0, data_ov000_0236aad8.unkE4);
            Sound_PlaySeqArc(0, 0, 0xa1);
        }
    } else {
        if (data_ov000_0236aad8.unk62 <= (-s->unk134C >> 12) + 0x34) {
            s->unk20C |= 8;
            func_ov000_023295a4(s, 0, 0, data_ov000_0236aad8.unk62, 1, data_ov000_0236aad8.unkE0, data_ov000_0236aad8.unkE4);
            Sound_PlaySeqArc(0, 0, 0xa1);
        }
    }
    if (s->unk20C & 0x10) {
        s32 vel;
        if (pl->unk58 & 0x8000)
            vel = pl->unk2E;
        else
            vel = pl->unk34;
        if (pl->unk58 & 0x10) {
            t->unk10 = 0;
        } else {
            if (t->unk10 == 0 && vel != 0) {
                func_ov000_02329128(pl, 0, 0, data_ov000_0236aad8.unk62, data_ov000_0236aad8.unkE0, data_ov000_0236aad8.unkE4);
                t->unk10 = 0xc;
                Sound_PlaySeqArc(0, 0, 0x51);
            }
            t->unk10--;
        }
        if (data_ov000_0236aad8.unk62 > 0x200) {
            t->unk12--;
            if (t->unk12 <= 0)
                s->unk20C &= ~0x10;
        } else if (data_ov000_0236aad8.unk62 < 502.1) {
            s->unk20C &= ~0x10;
        }
    } else {
        h = data_ov000_0236aad8.unk62;
        if (h >= 502.1 && h <= 0x200) {
            s->unk20C |= 0x10;
            t->unk10 = 0;
            t->unk12 = 0x3c;
        }
    }
    h = data_ov000_0236aad8.unk62;
    if (!(h >= 459.2))
        return;
    if (h > 0x200)
        return;
    if (pl->unk58 & 0x10)
        return;
    if ((t->unk14 & 7) == 0)
        func_ov000_023294c8(pl);
    t->unk14++;
}
