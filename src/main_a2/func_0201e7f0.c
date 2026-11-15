#include <nitro/types.h>

typedef struct { u32 offset; u16 bankNo; u8 volume; u8 channelPrio; } UnkSeqInfo_0201e7f0;
typedef struct { u8 unk0[0x18]; u32 dataOffset; } UnkSeqData_0201e7f0;

void *func_0201ba54(void *handle, u32 playerNo, s32 prio);
void *func_0201b8e8(u32 playerNo, void *seq);
s32 func_0201e3a0(s32 idx, u32 flags, void *heap, BOOL cache, void **out);
void func_0201ba48(void *seq);
void func_0201b9ec(void *seq, void *data, u32 offset, void *bank);
void func_0201be88(void *handle, u8 v);
void func_0201be10(void *handle, u8 v);
void func_0201bd3c(void *handle, u32 a, u32 b);

BOOL func_0201e7f0(void *handle, u32 playerNo, s32 bankNo, s32 prio, UnkSeqInfo_0201e7f0 *info,
                   UnkSeqData_0201e7f0 *data, u32 a, u32 b) {
    void *seq;
    void *heap;
    void *bank;
    seq = func_0201ba54(handle, playerNo, prio);
    if (seq == NULL) return FALSE;
    heap = func_0201b8e8(playerNo, seq);
    if (func_0201e3a0(bankNo, 6, heap, FALSE, &bank) != 0) {
        func_0201ba48(seq);
        return FALSE;
    }
    func_0201b9ec(seq, (u8 *)data + data->dataOffset, info->offset, bank);
    func_0201be88(handle, info->volume);
    func_0201be10(handle, info->channelPrio);
    func_0201bd3c(handle, a, b);
    return TRUE;
}
