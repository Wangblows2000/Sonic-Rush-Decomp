#include <nitro/types.h>

typedef struct UnkWork_02053090 UnkWork_02053090;

typedef struct {
    u8 pad0[0x28];
    u16 unk28;
    u16 unk2a;
    u16 unk2c;
    u8 pad2e[0xa];
    u8 *unk38;
    u8 pad3c[2];
    u16 unk3e;
} UnkBg_02053090;

struct UnkWork_02053090 {
    u8 pad0[0xc];
    u32 unkc;
    u8 pad10[4];
    UnkBg_02053090 bg[2];
    UnkBg_02053090 bg94;
    u32 flags;
    void (*fnd8)(UnkWork_02053090 *w);
    void (*fndc)(UnkWork_02053090 *w);
    void (*fne0)(UnkWork_02053090 *w);
    u8 pade4[0x24];
    s32 unk108;
    s32 unk10c;
    u32 unk110;
    void *unk114;
};

typedef struct {
    u8 pad0[0x10];
    UnkWork_02053090 *work;
} UnkTask_02053090;

extern UnkTask_02053090 *gNaviMsgTask;

void func_02052bb0(UnkWork_02053090 *w);
void Background_Draw(UnkBg_02053090 *bg);
void func_02052638(void *src, u32 a, u16 *dst, s32 b, u16 c, u16 d, s32 e);
void func_02052568(u32 a, UnkBg_02053090 *bg, s32 b);
void func_02052d18(UnkWork_02053090 *w);
void NNS_SndPlayerStopSeq(void *p, s32 a);
void Sound_PlayVoice(void *a, u32 b, s32 c);

void NaviMsg_Main(void) {
    UnkWork_02053090 *w = gNaviMsgTask->work;

    if (w->flags & 2) {
        if (w->fnd8 != NULL)
            w->fnd8(w);
        if (w->fndc != NULL)
            w->fndc(w);
        if (w->fne0 != NULL)
            w->fne0(w);
        func_02052bb0(w);
        if (w->flags & 0x40000) {
            if (w->flags & 0x10000)
                Background_Draw(&w->bg[0]);
            else
                func_02052638(w->bg[1].unk38 + 4, w->bg[1].unk2c * 2, (u16 *)0x6205000 + (w->bg[1].unk28 + (w->bg[1].unk2a << 8) / 8), 0x40, w->bg[1].unk2c, w->bg[1].unk3e, 0x100);
            w->flags &= ~0x40000;
            w->flags |= 0x80000;
        } else if (w->flags & 0x80000) {
            func_02052568(w->unkc, &w->bg[(w->flags & 0x10000) ? 0 : 1], 0);
            w->flags &= ~0x80000;
            w->flags |= 0x100000;
        } else if (w->flags & 0x100000) {
            func_02052568(w->unkc, &w->bg[(w->flags & 0x10000) ? 0 : 1], 1);
            w->flags &= ~0x100000;
            w->flags |= 0x20000;
        } else if (w->flags & 0x200000) {
            Background_Draw(&w->bg94);
            w->flags &= ~0x200000;
            w->flags |= 0x400000;
            w->fne0 = func_02052d18;
        }
    }
    if (w->unk108 == -1)
        return;
    w->unk10c--;
    if (w->unk10c > 0)
        return;
    if (w->unk114 != NULL)
        NNS_SndPlayerStopSeq(w->unk114, 0);
    Sound_PlayVoice(w->unk114, w->unk110, w->unk108);
    w->unk108 = -1;
}
