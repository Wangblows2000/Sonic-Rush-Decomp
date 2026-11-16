#include <nitro/types.h>

typedef struct { u32 a; u32 b; } UnkReq_0201f5b8;
typedef BOOL (*UnkCb_0201f5b8)(u32 type, UnkReq_0201f5b8 *req, UnkReq_0201f5b8 *res, void *arg);

typedef struct {
    u8 unk0[0xA0];
    u32 unkA0;
    u8 unkA4[0x18];
    u8 format;
    u8 unkBD[3];
    u16 rate;
    u8 unkC2[0x4A];
    u32 unk10C_0 : 4;
    s32 unk10C_4 : 1;
    s32 unk10C_5 : 1;
    u8 unk110[0x28];
    UnkCb_0201f5b8 cb;
    void *cbArg;
    u32 unk140;
    u32 unk144;
    u8 unk148[0x10];
    u32 unk158;
} UnkStruct_0201f5b8;

u32 *NNS_SndArcGetStrmInfo(u32 idx);
u32 func_0201d1ec(u32 fileId);
s32 NNS_SndArcReadFile(u32 idx, void *buf, u32 size, u32 pos);

void func_0201f5b8(UnkStruct_0201f5b8 *s) {
    UnkReq_0201f5b8 req;
    UnkReq_0201f5b8 res;
    u32 *info;
    u8 format;
    u16 rate;
    req.a = s->unk144;
    req.b = s->unk140;
    res.a = s->unk140;
    res.b = 0;
    if (!s->cb(0, &req, &res, s->cbArg)) return;
    info = NNS_SndArcGetStrmInfo(res.a);
    if (info == NULL) return;
    format = s->format;
    rate = s->rate;
    s->unkA0 = func_0201d1ec(*info);
    NNS_SndArcReadFile(*info, s->unkA4, 0x40, 0);
    if (rate != s->rate) return;
    if (format == 0 && s->format != 0) return;
    if (format != 0 && s->format == 0) return;
    s->unk140 = res.a;
    s->unk158 = (s->rate * res.b) / 1000;
    if (s->unk158 != 0 && s->format == 2) {
        s->unk10C_4 = 1;
    } else {
        s->unk10C_4 = 0;
    }
    s->unk10C_5 = 0;
}
