#include <nitro/types.h>

void SNDi_LockMutex(void);
void SNDi_UnlockMutex(void);
typedef struct { u8 pad[0x3c]; u32 offs[1]; } UnkStruct_02009b60;
void *SND_GetWaveDataAddress(UnkStruct_02009b60 *p, s32 i) {
    u8 *r;
    u32 off;
    SNDi_LockMutex();
    off = p->offs[i];
    if (off != 0) { if (off < 0x2000000) r = (u8 *)p + off; else r = (u8 *)off; } else r = 0;
    SNDi_UnlockMutex();
    return r;
}
