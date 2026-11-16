#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
    u8 pad1C[0x20 - 0x1c];
    u32 unk20;
} UnkCfg_02348974;

extern UnkCfg_02348974 data_022c4560;
void func_ov000_02331a24(u8 *w);
void func_ov000_023475fc(u8 *w);
void func_ov000_0234739c(u8 *w);
void func_ov000_0234c644(u8 *w);
void func_ov000_0234ab34(u8 *w);
void func_ov000_0234a064(u8 *w, s32 a);

void func_ov000_02348974(u8 *w) {
    if (!(*(u32 *)(w + 0x330) & 0x1000) && !(data_022c4560.unk20 & 7) && (data_022c4560.unk18 & 1))
        func_ov000_02331a24(w);
    if (*(s16 *)(w + 0x3e0) != 0)
        (*(s16 *)(w + 0x3e0))--;
    func_ov000_023475fc(w);
    func_ov000_0234739c(w);
    if (!(*(u16 *)(w + 0x420) & 0x100) || *(s16 *)(w + 0x3e0) == 0) {
        func_ov000_0234c644(w);
        *(s16 *)(w + 0x34) = *(s16 *)(w + 0x2e);
        *(u16 *)(w + 0x5c) |= 0x400;
        func_ov000_0234ab34(w);
        return;
    }
    if (!(*(u32 *)(w + 0x58) & 1))
        return;
    func_ov000_0234c644(w);
    func_ov000_0234a064(w, 0);
    (*(void (**)(u8 *))(w + 0x334))(w);
}
