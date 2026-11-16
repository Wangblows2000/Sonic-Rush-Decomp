#include <nitro/types.h>

typedef struct { u8 unk0[0x118]; s32 unk118; } UnkStrm_0201f704;
typedef struct { u8 link[8]; UnkStrm_0201f704 *strm; s32 type; s32 count; void *bufs[6]; u32 size; } UnkNode_0201f704;

extern u8 data_020847ac[];
extern u8 data_02084300[];
extern u8 *data_02083f48;

void *NNS_FndGetNextListObject(void *list, void *obj);
void MI_CpuFill8(void *dst, u8 val, u32 size);
void NNS_FndRemoveListObject(void *list, void *obj);
void FreeCommandBuffer(UnkNode_0201f704 *n);
UnkNode_0201f704 *func_0201f93c(void);
void NNS_FndAppendListObject(void *list, void *obj);
void OS_WakeupThread(void *q);

void func_0201f704(s32 type, s32 count, void **bufs, u32 size, u32 unused, UnkStrm_0201f704 *s) {
    UnkNode_0201f704 *n;
    s32 i;
    u8 *w;
    if (s->unk118 >= 2) {
        for (n = NNS_FndGetNextListObject(data_020847ac, NULL); n != NULL; n = NNS_FndGetNextListObject(data_020847ac, n)) {
            if (n->strm == s) break;
        }
        for (i = 0; i < n->count; i++) {
            MI_CpuFill8(n->bufs[i], 0, n->size);
        }
        NNS_FndRemoveListObject(data_020847ac, n);
        s->unk118--;
        FreeCommandBuffer(n);
    }
    n = func_0201f93c();
    n->strm = s;
    n->type = type;
    n->count = count;
    for (i = 0; i < count; i++) {
        n->bufs[i] = bufs[i];
    }
    n->size = size;
    w = data_02084300;
    if (type == 0 && data_02083f48 != NULL) w = data_02083f48;
    s->unk118++;
    NNS_FndAppendListObject(w + 0x4AC, n);
    OS_WakeupThread(w + 0x494);
}
