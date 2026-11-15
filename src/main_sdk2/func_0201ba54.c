#include <nitro/types.h>
#include <main_sdk2/sndp.h>

extern UnkSndPlayerHeap data_020838a4[];

void func_0201bf1c(UnkSndHandle *handle);
void func_0201b7a4(UnkSndPlayer *p);
UnkSndPlayer *func_0201b728(s32 prio);
void func_0201b840(UnkSndPlayerHeap *heap, UnkSndPlayer *p);

UnkSndPlayer *func_0201ba54(UnkSndHandle *handle, s32 playerNo, s32 prio)
{
    UnkSndPlayerHeap *heap = &data_020838a4[playerNo];
    UnkSndPlayer *p;

    if (handle->player != NULL) {
        func_0201bf1c(handle);
    }
    if (heap->count >= heap->max) {
        p = NNS_FndGetNextListObject(heap, NULL);
        if (p == NULL) {
            return NULL;
        }
        if (prio < p->prio) {
            return NULL;
        }
        func_0201b7a4(p);
    }
    p = func_0201b728(prio);
    if (p == NULL) {
        return NULL;
    }
    func_0201b840(heap, p);
    p->handle = handle;
    handle->player = p;
    return p;
}
