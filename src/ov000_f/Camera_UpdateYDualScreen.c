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

extern UnkStruct_023395a4 gCamera;
extern UnkStruct_023395a4_P *gPlayers[];

s32 Stage_GetSplitLineY(s32 x);
void Camera_CheckPlayerCrossScreen(UnkStruct_023395a4_W *w);
BOOL Camera_ClampLookPanYDual(UnkStruct_023395a4_W *w);
BOOL Camera_HasReturnedToPlayerYDual(UnkStruct_023395a4_W *w);
void Camera_Look_Reset(UnkStruct_023395a4_W *w);
void Camera_FollowPlayerY(UnkStruct_023395a4_W *w, s32 idx);
void Camera_SwapScreens(s32 a);
void Camera_ClampYToBounds(UnkStruct_023395a4_W *w, s32 idx);

static inline void Finish_023395a4(UnkStruct_023395a4_W *w) {
    gCamera.flags &= ~0x10000;
    gCamera.flags &= ~0x20000;
    Camera_Look_Reset(w);
}

#define CLAMP_023395a4(v, lo, hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))

void Camera_UpdateYDualScreen(UnkStruct_023395a4_W *w, s32 idx) {
    UnkStruct_023395a4 *g = &gCamera;
    UnkStruct_023395a4_L *l = &g->layer[idx];
    s32 ground = Stage_GetSplitLineY(l->x);
    UnkStruct_023395a4 *g2 = &gCamera;
    u32 flags = g2->flags;
    u32 dn;

    if (flags & 8) {
        ground = g2->unkDC;
    }
    if (flags & 0x10000) {
        if (!(flags & 0x20000)) {
            l->y += w->unk8;
            Camera_CheckPlayerCrossScreen(w);
            Camera_ClampLookPanYDual(w);
        } else {
            l->y += w->unk8;
            dn = gCamera.flags & 2;
            if ((dn && gPlayers[l->ply]->y < ground) || (!dn && gPlayers[l->ply]->y > ground)) {
                Camera_CheckPlayerCrossScreen(w);
                dn = gCamera.flags & 2;
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
                    } else if (Camera_HasReturnedToPlayerYDual(w)) {
                        l->y = gPlayers[l->ply]->y - l->unk48;
                        Finish_023395a4(w);
                    }
                } else if (w->unk8 < 0) {
                    if (l->y < ground - l->scaleY * 0xe8 && l->y < gPlayers[l->ply]->y - l->unk48) {
                        l->y = gPlayers[l->ply]->y - l->unk48;
                        l->y = CLAMP_023395a4(l->y, l->minY, ground - l->scaleY * 0xe8);
                        Finish_023395a4(w);
                    }
                } else if (Camera_ClampLookPanYDual(w)) {
                    Finish_023395a4(w);
                    return;
                }
            } else {
                if (w->unk8 > 0) {
                    if (l->y > ground + l->scaleY * 0x28 && l->y > gPlayers[l->ply]->y - l->unk48) {
                        l->y = gPlayers[l->ply]->y - l->unk48;
                        l->y = CLAMP_023395a4(l->y, ground + l->scaleY * 0x28, l->maxY - l->scaleY * 0xc0);
                        Finish_023395a4(w);
                    }
                } else if (w->unk8 < 0) {
                    if (l->y < ground + l->scaleY * 0x28) {
                        l->y = CLAMP_023395a4(l->y, ground + l->scaleY * 0x28, l->maxY - l->scaleY * 0xc0);
                        Finish_023395a4(w);
                    } else if (Camera_HasReturnedToPlayerYDual(w)) {
                        l->y = gPlayers[l->ply]->y - l->unk48;
                        Finish_023395a4(w);
                    }
                } else if (Camera_ClampLookPanYDual(w)) {
                    Finish_023395a4(w);
                    return;
                }
            }
        }
    } else if (!(flags & 4)) {
        if (!(flags & 2)) {
            if (gPlayers[l->ply]->y > ground) {
                Camera_FollowPlayerY(w, idx);
                Camera_SwapScreens(1);
                g->flags |= 0x200;
                g->unkD8 = -g->unkD8;
            } else if (!(l->flags & 0x4020)) {
                Camera_FollowPlayerY(w, idx);
                l->y = CLAMP_023395a4(l->y, l->minY, ground - l->scaleY * 0xe8);
            }
        } else {
            if (gPlayers[l->ply]->y < ground) {
                Camera_FollowPlayerY(w, idx);
                Camera_SwapScreens(0);
                g->flags |= 0x200;
                g->unkD8 = -g->unkD8;
            } else if (!(l->flags & 0x4020)) {
                Camera_FollowPlayerY(w, idx);
                l->y = CLAMP_023395a4(l->y, ground + l->scaleY * 0x28, l->maxY - l->scaleY * 0xc0);
            }
        }
    } else {
        Camera_FollowPlayerY(w, idx);
    }
    Camera_ClampYToBounds(w, idx);
}
