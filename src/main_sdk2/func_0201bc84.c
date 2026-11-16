#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u8 status;
    u8 pad2d[0xf];
    u8 playerNo;
    u8 pad3d[7];
} UnkSndPlayer_0201bc84;

typedef struct {
    u8 list[0xc];
    u8 list2[0xc];
    u32 unk18;
    u32 unk1c;
    u8 unk20;
    u8 pad21[3];
} UnkHeap_0201bc84;

extern u8 data_02083458[];
extern u8 data_0208344c[];
extern UnkSndPlayer_0201bc84 data_02083464[];
extern UnkHeap_0201bc84 data_020838a4[];

void NNS_FndInitList(void *list, u16 offset);
void func_0202076c(void *list, void *obj);

void func_0201bc84(void)
{
    s32 i;
    NNS_FndInitList(data_02083458, 0x14);
    NNS_FndInitList(data_0208344c, 0x14);
    for (i = 0; i < 16; i++) {
        UnkSndPlayer_0201bc84 *p = &data_02083464[i];
        p->status = 0;
        p->playerNo = i;
        func_0202076c(data_0208344c, p);
    }
    for (i = 0; i < 32; i++) {
        UnkHeap_0201bc84 *h = &data_020838a4[i];
        NNS_FndInitList(h->list, 0xc);
        NNS_FndInitList(h->list2, 0);
        h->unk20 = 0x7f;
        h->unk18 = 1;
        h->unk1c = 0;
    }
}
