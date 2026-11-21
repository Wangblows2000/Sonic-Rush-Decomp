#include <nitro/types.h>

void NNS_SndFreeAlarm(void *p);
void NNS_FndRemoveListObject(void *list, void *obj);
extern u8 data_02083d28[];

typedef struct { u8 unk0[0x24]; u32 unk24; u8 unk28[0x18]; void *unk40; } UnkStruct_0201c2ec;

void ShutdownStrm(UnkStruct_0201c2ec *s) {
    NNS_SndFreeAlarm(s->unk40);
    NNS_FndRemoveListObject(data_02083d28, s);
    s->unk24 &= ~1;
}
