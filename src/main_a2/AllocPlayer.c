#include <nitro/types.h>

typedef struct {
    u8 unk0[0x10C];
    s32 unk10C_0 : 1;
    u8 unk110[0x18];
    u32 unk128;
    u8 unk12C[0x1C];
    void *handle;
    s32 prio;
    u8 unk150[0xC];
} UnkStrm_0201ffd8;
typedef struct { UnkStrm_0201ffd8 *strm; } UnkHandle_0201ffd8;

extern UnkStrm_0201ffd8 data_020847b8[];

void NNS_SndStrmHandleRelease(UnkHandle_0201ffd8 *h);
void func_0201fbc0(UnkStrm_0201ffd8 *s);

UnkStrm_0201ffd8 *AllocPlayer(UnkHandle_0201ffd8 *h, s32 no, s32 prio) {
    UnkStrm_0201ffd8 *s;
    if (h->strm != NULL) NNS_SndStrmHandleRelease(h);
    s = &data_020847b8[no];
    if (s->unk128 == 0) return NULL;
    if (s->unk10C_0) {
        if (prio < s->prio) return NULL;
        func_0201fbc0(s);
    }
    s->prio = prio;
    s->unk10C_0 = 1;
    s->handle = h;
    h->strm = s;
    return s;
}
