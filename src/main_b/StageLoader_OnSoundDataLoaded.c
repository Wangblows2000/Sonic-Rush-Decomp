#include <nitro/types.h>

typedef struct { u8 unk0[0x5c]; u32 unk5C; } UnkStruct_0204aa98;
extern u32 gZoneSoundData;
void StageLoader_OnSoundDataLoaded(UnkStruct_0204aa98 *s) { gZoneSoundData = s->unk5C; }
