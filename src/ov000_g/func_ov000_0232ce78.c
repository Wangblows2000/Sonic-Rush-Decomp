#include <nitro/types.h>

typedef struct {
    u8 pad0[0x212];
    s8 unk212;
    s8 unk213;
} UnkStruct_0232ce78_P;

typedef struct {
    u8 pad0[0x134];
    u8 unk134[0x48];
    u8 unk17C[0x48];
    u8 unk1C4[0x48];
    u8 unk20C[0x48];
    s16 unk254;
    s16 unk256;
    s16 unk258;
    s16 unk25A;
    s16 unk25C;
    s16 unk25E;
    u8 pad260[0x264 - 0x260];
    s16 unk264;
    s16 unk266;
    s32 unk268;
    s32 unk26C;
    u32 unk270;
    s16 unk274;
    s16 unk276;
} UnkStruct_0232ce78;

UnkStruct_0232ce78_P *func_ov000_02342358(s32 a);
void func_ov000_02336f28(u32 *p, s32 v, s32 n);
void AnimatorSprite_SetAnimation(void *s, u16 a);
void func_ov000_0232d30c(UnkStruct_0232ce78 *w, s32 a);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void func_ov000_0232ce4c(UnkStruct_0232ce78 *w);

void func_ov000_0232ce78(UnkStruct_0232ce78 *w) {
    UnkStruct_0232ce78_P *p;

    w->unk26C = w->unk268;
    p = func_ov000_02342358(1);
    if (p != NULL)
        w->unk268 = p->unk212 - p->unk213;
    else
        w->unk268 = 0;
    if (w->unk26C != w->unk268) {
        func_ov000_02336f28(&w->unk270, w->unk268, 100);
        AnimatorSprite_SetAnimation(w->unk134, ((w->unk270 & 0xf0) >> 4) + 1);
        AnimatorSprite_SetAnimation(w->unk17C, ((w->unk270 & 0xf0) >> 4) + 1);
        w->unk254 = w->unk258 = 0x180;
        w->unk264 = 4;
        w->unk274 = 8;
    }
    if (w->unk274 != 0) {
        w->unk274--;
        if (w->unk274 == 0) {
            AnimatorSprite_SetAnimation(w->unk1C4, (w->unk270 & 0xf) + 1);
            AnimatorSprite_SetAnimation(w->unk20C, (w->unk270 & 0xf) + 1);
            w->unk256 = w->unk25A = 0x180;
            w->unk266 = 4;
        }
    }
    if (w->unk264 != 0 || w->unk25C != 0)
        func_ov000_0232d30c(w, 0);
    if (w->unk266 != 0 || w->unk25E != 0)
        func_ov000_0232d30c(w, 1);
    if (w->unk268 == 0) {
        if (w->unk276 == 0)
            Sound_PlaySeqArcEx(NULL, 0, 0xe8);
        w->unk276++;
    }
    if (w->unk276 > 0x50)
        func_ov000_0232ce4c(w);
}
