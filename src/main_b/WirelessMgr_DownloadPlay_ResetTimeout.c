#include <nitro/types.h>

typedef struct { u8 unk0[0x1a]; u16 unk1A; } UnkStruct_02040f34;
void WirelessMgr_DownloadPlay_ResetTimeout(UnkStruct_02040f34 *s) { s->unk1A = 0; }
