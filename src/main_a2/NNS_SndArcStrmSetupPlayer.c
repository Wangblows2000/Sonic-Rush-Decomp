#include <nitro/types.h>
#include <main_a2/strm.h>

typedef struct { u8 count; u8 ch[1]; } UnkChInfo_020202e0;

extern UnkStrm data_020847b8[];

UnkChInfo_020202e0 *NNS_SndArcGetStrmPlayerInfo(s32 idx);
void *NNS_SndHeapAlloc(void *heap, u32 size, void *cb, void *arg, u32 c);
void DisposeCallback_sndarc_stream(void);
void ForceStopStrm_sndarc_stream(UnkStrm *s);

BOOL NNS_SndArcStrmSetupPlayer(void *heap) {
    s32 i;
    u32 size;
    UnkStrm *s;
    void *buf;
    s32 j;
    UnkChInfo_020202e0 *info;
    s = data_020847b8;
    for (i = 0; i < 4; i++, s++) {
        info = NNS_SndArcGetStrmPlayerInfo(i);
        if (info == NULL) continue;
        s->unk120 = info->count;
        for (j = 0; j < info->count; j++) {
            s->unk122[j] = info->ch[j];
        }
        if (heap == NULL) continue;
        size = s->unk120 << 11;
        buf = NNS_SndHeapAlloc(heap, size, DisposeCallback_sndarc_stream, s, 0);
        if (buf == NULL) return FALSE;
        ForceStopStrm_sndarc_stream(s);
        s->unk128 = (u32)buf;
        s->unk12C = size;
    }
    return TRUE;
}
