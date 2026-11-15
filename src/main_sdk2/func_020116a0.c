#include <nitro/types.h>
#include <main_sdk2/wm.h>

void DC_StoreRange(void *p, u32 size);
void MI_CpuCopy8(const void *src, void *dest, u32 size);
void MI_CpuFill8(void *dest, u8 data, u32 size);

typedef struct {
    u16 apiid;
    u16 pad;
    u16 *list;
    u8 data[0x18];
    u32 unk20;
    u16 pad24;
    u16 unk26;
} UnkWmCmd_020116a0;

s32 func_020116a0(void *cb, u16 *list, const void *data, u32 a, u16 b)
{
    UnkWmCmd_020116a0 *cmd;
    s32 r = func_02010a50(1, 2);
    if (r != 0) {
        return r;
    }
    if (list == NULL) {
        return 6;
    }
    DC_StoreRange(list, list[0] * 2);
    func_02010cd4(0xc, cb);
    cmd = (UnkWmCmd_020116a0 *)((UnkWmWork *)func_02010b70())->unkC;
    cmd->apiid = 0xc;
    cmd->list = list;
    if (data != NULL) {
        MI_CpuCopy8(data, cmd->data, 0x18);
    } else {
        MI_CpuFill8(cmd->data, 0, 0x18);
    }
    cmd->unk20 = a;
    cmd->unk26 = b;
    r = func_02010b80(0xc, 0);
    if (r == 0) {
        r = 2;
    }
    return r;
}
