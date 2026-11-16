#include <nitro/types.h>

typedef struct {
    void *player;
} UnkSndHandle_0201b698;

typedef struct {
    u8 pad0[0xc];
    void *unkC;
} UnkSeq_0201b698;

typedef struct {
    UnkSndHandle_0201b698 *handle;
    u8 *heap;
    UnkSeq_0201b698 *seq;
    u8 padC[0x2c - 0xc];
    u8 status;
} UnkSndPlayer_0201b698;

extern u8 data_02083458[];
extern u8 data_0208344c[];

void NNS_FndRemoveListObject(void *list, void *obj);
void NNS_FndAppendListObject(void *list, void *obj);

void ShutdownPlayer(UnkSndPlayer_0201b698 *p)
{
    u8 *heap;
    if (p->handle != NULL) {
        p->handle->player = NULL;
        p->handle = NULL;
    }
    heap = p->heap;
    NNS_FndRemoveListObject(heap, p);
    p->heap = NULL;
    if (p->seq != NULL) {
        NNS_FndAppendListObject(heap + 0xc, p->seq);
        p->seq->unkC = NULL;
        p->seq = NULL;
    }
    NNS_FndRemoveListObject(data_02083458, p);
    NNS_FndAppendListObject(data_0208344c, p);
    p->status = 0;
}
