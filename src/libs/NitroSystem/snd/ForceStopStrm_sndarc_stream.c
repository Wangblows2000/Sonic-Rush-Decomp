#include <nitro/types.h>

typedef struct {
    u8 unk0[0x5C];
    u8 file[0xB0];
    s32 unk10C_0 : 1;
    s32 unk10C_1 : 1;
} UnkStruct_0201fbc0;

extern u8 data_02084798[];
extern u8 *data_02083f48;

void OS_LockMutex(void *m);
void OS_UnlockMutex(void *m);
void NNS_SndStrmStop(void *s);
void FS_CancelFile(void *f);
void ShutdownPlayer_sndarc_stream(UnkStruct_0201fbc0 *s);

void ForceStopStrm_sndarc_stream(UnkStruct_0201fbc0 *s) {
    OS_LockMutex(data_02084798);
    if (data_02083f48 != NULL) OS_LockMutex(data_02083f48 + 0x498);
    if (s->unk10C_1) NNS_SndStrmStop(s);
    if (s->unk10C_0) FS_CancelFile(s->file);
    ShutdownPlayer_sndarc_stream(s);
    OS_UnlockMutex(data_02084798);
    if (data_02083f48 != NULL) OS_UnlockMutex(data_02083f48 + 0x498);
}
