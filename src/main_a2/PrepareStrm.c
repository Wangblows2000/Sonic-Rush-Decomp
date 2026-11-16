#include <nitro/types.h>

typedef struct { s32 origin; s32 target; s32 counter; s32 frame; } NNSSndFader;
typedef struct { void *arc; u32 id; } UnkFileID_0201fcc4;
typedef struct { u32 fileId; u8 volume; u8 unk5[2]; u8 flags; } UnkStrmInfo_0201fcc4;

typedef struct {
    u8 unk0[0x5C];
    u8 file[0x44];
    u32 unkA0;
    u8 unkA4[0x18];
    u8 format;
    u8 unkBD;
    u8 channels;
    u8 unkBF;
    u16 rate;
    u16 timer;
    u8 unkC4[0x20];
    NNSSndFader fader;
    u8 unkF4[0x18];
    u32 unk10C_0 : 1;
    u32 unk10C_1 : 1;
    u32 unk10C_2 : 1;
    u32 unk10C_3 : 1;
    u32 unk10C_4 : 1;
    u32 unk10C_5 : 1;
    u32 unk10C_6 : 1;
    s32 unk110;
    s32 unk114;
    s32 unk118;
    s32 unk11C;
    u8 unk120;
    u8 unk121;
    u8 unk122[6];
    u32 unk128;
    u32 unk12C;
    u32 unk130;
    u32 unk134;
    void *unk138;
    void *unk13C;
    u32 unk140;
    u8 unk144[0xC];
    u32 unk150;
    u32 unk154;
    u32 unk158;
} UnkStrm_0201fcc4;

UnkStrm_0201fcc4 *AllocPlayer(void *handle, u32 a, u32 b);
s32 NNS_SndArcReadFile(u32 idx, void *buf, u32 size, u32 pos);
void FreePlayer(UnkStrm_0201fcc4 *s);
UnkFileID_0201fcc4 NNS_SndArcGetFileID(void);
BOOL FS_OpenFileFast(void *file, UnkFileID_0201fcc4 id);
u32 NNS_SndArcGetFileOffset(u32 fileId);
void NNSi_SndFaderInit(NNSSndFader *f);
void NNSi_SndFaderSet(NNSSndFader *f, s32 target, s32 frame);
BOOL AllocChannel(UnkStrm_0201fcc4 *s, s32 ch, void *p);
void FS_CloseFile(void *file);
u32 _u32_div_f(u32 a, u32 b);
BOOL NNS_SndStrmSetup(UnkStrm_0201fcc4 *s, s32 format, u32 data, u32 rate, s32 timer, s32 arg5, void *arg6, void *arg7);
void func_0201f704(void);
void FreeChannel(UnkStrm_0201fcc4 *s);
void NNS_SndStrmSetChannelPan(UnkStrm_0201fcc4 *s, s32 ch, s32 pan);

BOOL PrepareStrm(void *handle, UnkStrmInfo_0201fcc4 *info, u32 a, u32 b, u32 strmNo, u32 offsetMs,
                   u32 p6, u32 p7, void *cb, void *cbArg) {
    UnkStrm_0201fcc4 *s;
    s32 fmt;
    s32 ch;
    UnkFileID_0201fcc4 id;

    s = AllocPlayer(handle, a, b);
    if (s == NULL) return FALSE;
    if (NNS_SndArcReadFile(info->fileId, s->unkA4, 0x40, 0) != 0x40) {
        FreePlayer(s);
        return FALSE;
    }
    id = NNS_SndArcGetFileID();
    if (!FS_OpenFileFast(s->file, id)) {
        FreePlayer(s);
        return FALSE;
    }
    s->unkA0 = NNS_SndArcGetFileOffset(info->fileId);
    s->unk158 = (s->rate * offsetMs) / 1000;
    if (s->unk158 != 0 && s->format == 2) {
        s->unk10C_4 = 1;
    } else {
        s->unk10C_4 = 0;
    }
    s->unk110 = 4;
    s->unk10C_5 = 0;
    s->unk10C_1 = 0;
    s->unk114 = 0;
    s->unk10C_2 = 0;
    s->unk10C_3 = 0;
    s->unk118 = 0;
    s->unk130 = p6;
    s->unk134 = p7;
    s->unk138 = cb;
    s->unk13C = cbArg;
    s->unk140 = strmNo;
    s->unk154 = 0;
    s->unk150 = info->volume;
    NNSi_SndFaderInit(&s->fader);
    NNSi_SndFaderSet(&s->fader, 0x7F00, 1);
    switch (s->format) {
    case 0:
        fmt = 0;
        break;
    case 1:
    case 2:
        fmt = 1;
        break;
    }
    ch = s->channels;
    if (info->flags & 1) ch = 2;
    if (ch > s->unk120) ch = s->unk120;
    s->unk10C_6 = (ch == 1);
    if (!AllocChannel(s, ch, s->unk122)) {
        FS_CloseFile(s->file);
        FreePlayer(s);
        return FALSE;
    }
    if (!NNS_SndStrmSetup(s, fmt, s->unk128, _u32_div_f(s->unk12C * ch, s->unk120), s->timer, 4, func_0201f704, s)) {
        FreeChannel(s);
        FS_CloseFile(s->file);
        FreePlayer(s);
        return FALSE;
    }
    if (ch == 2) {
        NNS_SndStrmSetChannelPan(s, 0, 0);
        NNS_SndStrmSetChannelPan(s, 1, 0x7F);
    }
    return TRUE;
}
