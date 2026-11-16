#include <nitro/types.h>

typedef struct {
    u8 pad0[0x34];
    u16 type;
    u8 pad36[2];
    u16 unk38;
    u16 unk3a;
} UnkSndPlayer_0201bea8;

extern u8 data_02083458[];

void *NNS_FndGetNextListObject(void *list, void *obj);

s32 NNS_SndPlayerCountPlayingSeqByPlayerNo(u16 a, u16 b)
{
    s32 n = 0;
    UnkSndPlayer_0201bea8 *p = NNS_FndGetNextListObject(data_02083458, NULL);
    while (p != NULL) {
        if (p->type == 2 && p->unk38 == a && p->unk3a == b) {
            n++;
        }
        p = NNS_FndGetNextListObject(data_02083458, p);
    }
    return n;
}
