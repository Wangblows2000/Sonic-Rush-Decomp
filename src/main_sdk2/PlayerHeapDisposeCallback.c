#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    void *unk8;
} UnkSub_0201b63c;

typedef struct {
    u8 pad0[8];
    void *unk8;
    UnkSub_0201b63c *unkC;
    s32 unk10;
} UnkObj_0201b63c;

typedef struct {
    u8 pad0[0xc];
    u8 list[0x18];
} UnkHeap_0201b63c;

extern UnkHeap_0201b63c data_020838a4[];

void func_0201db14(void *p);
void NNS_FndRemoveListObject(void *list, void *obj);

void PlayerHeapDisposeCallback(UnkObj_0201b63c *x)
{
    if (x->unk8 == NULL) {
        return;
    }
    func_0201db14(x->unk8);
    if (x->unkC != NULL) {
        x->unkC->unk8 = NULL;
        return;
    }
    NNS_FndRemoveListObject(data_020838a4[x->unk10].list, x);
}
