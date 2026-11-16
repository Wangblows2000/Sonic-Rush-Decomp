#include <nitro/types.h>

extern u8 data_0224ffa0[];
extern u32 data_02087cb8;
extern void (*data_0224ff40)(void);
extern u16 data_022c1948;
extern u16 data_022c1950;
extern void *data_0224ff44[];

s32 func_0203e934(void);
u32 func_0203e944(void);
s32 func_0203e924(void);
BOOL func_0203ddd8(void *buf);
BOOL func_0203dba4(void);
void *func_0203de64(u16 aid);
void DC_StoreRange(const void *addr, u32 size);
void VBlankIntrWait(void);
void MI_CpuCopy8(const void *src, void *dst, u32 size);
void MI_CpuFill8(void *dst, u8 data, u32 size);

void func_02034ec0(void) {
    u32 vcount;
    u16 i;
    if (func_0203e934() != 5) {
        return;
    }
    vcount = func_0203e944();
    DC_StoreRange(data_0224ffa0, 0x200);
    if (data_02087cb8 & 0x200) {
        func_0203ddd8(data_0224ffa0);
    } else {
        while (!func_0203ddd8(data_0224ffa0)) {
            if (func_0203e924() != 5) {
                if (data_0224ff40 != NULL) {
                    data_0224ff40();
                }
                return;
            }
            VBlankIntrWait();
        }
        if (!func_0203dba4() && func_0203e944() < vcount && data_0224ff40 != NULL) {
            data_0224ff40();
        }
    }
    for (i = 0; i < data_022c1948 + 1; i++) {
        void *src = func_0203de64(i);
        if (src != NULL) {
            MI_CpuCopy8(src, data_0224ff44[i], data_022c1950);
        } else {
            MI_CpuFill8(data_0224ff44[i], 0, data_022c1950);
        }
    }
}
