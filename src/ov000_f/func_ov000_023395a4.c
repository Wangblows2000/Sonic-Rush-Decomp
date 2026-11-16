#include <nitro/types.h>

typedef struct {
    u32 flags;
    s32 x;
    s32 y;
    u8 padC[0x20 - 0xC];
    s16 scaleY;
    u8 pad22[0x3A - 0x22];
    s8 ply;
    u8 pad3B[0x48 - 0x3B];
    s32 unk48;
    u8 pad4C[0x50 - 0x4C];
    s32 minY;
    u8 pad54[0x58 - 0x54];
    s32 maxY;
    u8 pad5C[0x64 - 0x5C];
} UnkStruct_023395a4_L;

typedef struct {
    UnkStruct_023395a4_L layer[2];
    u32 flags;
    u8 padCC[0xD8 - 0xCC];
    s32 unkD8;
    s32 unkDC;
} UnkStruct_023395a4;

typedef struct {
    u8 pad0[0x14];
    s32 y;
} UnkStruct_023395a4_P;

typedef struct {
    u8 pad0[8];
    s32 unk8;
} UnkStruct_023395a4_W;

extern UnkStruct_023395a4 data_ov000_0236aad8;
extern UnkStruct_023395a4_P *data_ov000_0236ac60[];

s32 func_ov000_0233b078(s32 x);
void func_ov000_02338f88(UnkStruct_023395a4_W *w);
BOOL func_ov000_02338d6c(UnkStruct_023395a4_W *w);
BOOL func_ov000_02338f18(UnkStruct_023395a4_W *w);
void func_ov000_02338800(UnkStruct_023395a4_W *w);
void func_ov000_023390cc(UnkStruct_023395a4_W *w, s32 idx);
void func_ov000_0233b470(s32 a);
void func_ov000_023393a8(UnkStruct_023395a4_W *w, s32 idx);

static inline void Finish_023395a4(UnkStruct_023395a4_W *w) {
    data_ov000_0236aad8.flags &= ~0x10000;
    data_ov000_0236aad8.flags &= ~0x20000;
    func_ov000_02338800(w);
}

#define CLAMP_023395a4(v, lo, hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))

void func_ov000_023395a4(UnkStruct_023395a4_W *w, s32 idx) {
    UnkStruct_023395a4 *g = &data_ov000_0236aad8;
    UnkStruct_023395a4_L *l = &g->layer[idx];
    s32 ground = func_ov000_0233b078(l->x);
    UnkStruct_023395a4 *g2 = &data_ov000_0236aad8;
    u32 flags = g2->flags;
    u32 dn;

    if (flags & 8) {
        ground = g2->unkDC;
    }
    if (flags & 0x10000) {
        if (!(flags & 0x20000)) {
            l->y += w->unk8;
            func_ov000_02338f88(w);
            func_ov000_02338d6c(w);
        } else {
            l->y += w->unk8;
            dn = data_ov000_0236aad8.flags & 2;
            if ((dn && data_ov000_0236ac60[l->ply]->y < ground) || (!dn && data_ov000_0236ac60[l->ply]->y > ground)) {
                func_ov000_02338f88(w);
                dn = data_ov000_0236aad8.flags & 2;
                if (dn && w->unk8 < 0) {
                    if (l->y < ground + l->scaleY * 0x28) {
                        l->y = CLAMP_023395a4(l->y, ground + l->scaleY * 0x28, l->maxY - l->scaleY * 0xc0);
                        Finish_023395a4(w);
                    }
                } else if (!dn && w->unk8 > 0) {
                    if (l->y > ground - l->scaleY * 0xe8) {
                        l->y = CLAMP_023395a4(l->y, l->minY, ground - l->scaleY * 0xe8);
                        Finish_023395a4(w);
                    }
                }
            } else if (!dn) {
                if (w->unk8 > 0) {
                    if (l->y > ground - l->scaleY * 0xe8) {
                        l->y = CLAMP_023395a4(l->y, l->minY, ground - l->scaleY * 0xe8);
                        Finish_023395a4(w);
                    } else if (func_ov000_02338f18(w)) {
                        l->y = data_ov000_0236ac60[l->ply]->y - l->unk48;
                        Finish_023395a4(w);
                    }
                } else if (w->unk8 < 0) {
                    if (l->y < ground - l->scaleY * 0xe8 && l->y < data_ov000_0236ac60[l->ply]->y - l->unk48) {
                        l->y = data_ov000_0236ac60[l->ply]->y - l->unk48;
                        l->y = CLAMP_023395a4(l->y, l->minY, ground - l->scaleY * 0xe8);
                        Finish_023395a4(w);
                    }
                } else if (func_ov000_02338d6c(w)) {
                    Finish_023395a4(w);
                    return;
                }
            } else {
                if (w->unk8 > 0) {
                    if (l->y > ground + l->scaleY * 0x28 && l->y > data_ov000_0236ac60[l->ply]->y - l->unk48) {
                        l->y = data_ov000_0236ac60[l->ply]->y - l->unk48;
                        l->y = CLAMP_023395a4(l->y, ground + l->scaleY * 0x28, l->maxY - l->scaleY * 0xc0);
                        Finish_023395a4(w);
                    }
                } else if (w->unk8 < 0) {
                    if (l->y < ground + l->scaleY * 0x28) {
                        l->y = CLAMP_023395a4(l->y, ground + l->scaleY * 0x28, l->maxY - l->scaleY * 0xc0);
                        Finish_023395a4(w);
                    } else if (func_ov000_02338f18(w)) {
                        l->y = data_ov000_0236ac60[l->ply]->y - l->unk48;
                        Finish_023395a4(w);
                    }
                } else if (func_ov000_02338d6c(w)) {
                    Finish_023395a4(w);
                    return;
                }
            }
        }
    } else if (!(flags & 4)) {
        if (!(flags & 2)) {
            if (data_ov000_0236ac60[l->ply]->y > ground) {
                func_ov000_023390cc(w, idx);
                func_ov000_0233b470(1);
                g->flags |= 0x200;
                g->unkD8 = -g->unkD8;
            } else if (!(l->flags & 0x4020)) {
                func_ov000_023390cc(w, idx);
                l->y = CLAMP_023395a4(l->y, l->minY, ground - l->scaleY * 0xe8);
            }
        } else {
            if (data_ov000_0236ac60[l->ply]->y < ground) {
                func_ov000_023390cc(w, idx);
                func_ov000_0233b470(0);
                g->flags |= 0x200;
                g->unkD8 = -g->unkD8;
            } else if (!(l->flags & 0x4020)) {
                func_ov000_023390cc(w, idx);
                l->y = CLAMP_023395a4(l->y, ground + l->scaleY * 0x28, l->maxY - l->scaleY * 0xc0);
            }
        }
    } else {
        func_ov000_023390cc(w, idx);
    }
    func_ov000_023393a8(w, idx);
}
