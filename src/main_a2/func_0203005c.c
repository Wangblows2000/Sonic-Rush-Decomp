#include <nitro/types.h>

typedef struct {
    u32 bgcnt[4];
    u32 bgofs[3];
    u32 dispMode;
    u16 win[4];
    u32 bgAffine[4];
    u8 unk38[0x10];
    s16 brightness;
    u16 mosaic;
    u8 unk4C[0x14DC];
} UnkDisp_0203005c;

typedef struct { void *dst; void *buf[2]; u32 size; u32 cur; } UnkCopy_0203005c;

extern UnkDisp_0203005c data_0208922c;
extern UnkDisp_0203005c data_02087d04;
extern UnkCopy_0203005c data_02087cdc[];
extern u32 data_02087cb8;
extern u16 data_0208f78e[];

void DC_StoreRange(void *p, u32 size);
void DC_WaitWriteBufferEmpty(void);
void func_02007da4(u32 dma, const void *src, volatile void *dst, u32 size);
void func_02007d2c(u32 dma, const void *src, volatile void *dst, u32 size);
void GXx_SetMasterBrightness_(volatile u16 *reg, s32 brightness);
void func_020369ac(u32 engine);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_02035754(void);
void func_020363f0(void);
void func_0203383c(void);
void func_02036f94(void);
void func_02036ee8(void);
void func_0201b3a0(void);
void func_02007b3c(u32 dma);

void func_0203005c(void) {
    u32 i;
    UnkCopy_0203005c *c;
    vu16 z;
    DC_StoreRange(&data_0208922c, sizeof(UnkDisp_0203005c));
    DC_WaitWriteBufferEmpty();
    func_02007da4(3, data_0208922c.bgcnt, (vu32 *)0x04000010, 0x10);
    func_02007da4(3, data_0208922c.bgofs, (vu32 *)0x04000040, 0xC);
    *(vu32 *)0x04000000 = (*(vu32 *)0x04000000 & ~0xE000) | (data_0208922c.dispMode << 13);
    func_02007d2c(3, data_0208922c.win, (vu16 *)0x04000050, 6);
    GXx_SetMasterBrightness_((vu16 *)0x0400006C, data_0208922c.brightness);
    *(vu16 *)0x0400004C = data_0208922c.mosaic;
    func_02007da4(3, data_0208922c.bgAffine, (vu32 *)0x04000020, 0x10);
    func_02007da4(3, data_0208922c.unk38, (vu32 *)0x04000030, 0x10);
    func_020369ac(0);
    DC_StoreRange(&data_02087d04, sizeof(UnkDisp_0203005c));
    DC_WaitWriteBufferEmpty();
    func_02007da4(3, data_02087d04.bgcnt, (vu32 *)0x04001010, 0x10);
    func_02007da4(3, data_02087d04.bgofs, (vu32 *)0x04001040, 0xC);
    *(vu32 *)0x04001000 = (*(vu32 *)0x04001000 & ~0xE000) | (data_02087d04.dispMode << 13);
    func_02007d2c(3, data_02087d04.win, (vu16 *)0x04001050, 6);
    GXx_SetMasterBrightness_((vu16 *)0x0400106C, data_02087d04.brightness);
    *(vu16 *)0x0400104C = data_02087d04.mosaic;
    func_02007da4(3, data_02087d04.bgAffine, (vu32 *)0x04001020, 0x10);
    func_02007da4(3, data_02087d04.unk38, (vu32 *)0x04001030, 0x10);
    func_020369ac(1);
    for (i = 0, c = data_02087cdc; i < 2; i++, c++) {
        if (data_02087cb8 & (0x400 << i)) {
            MIi_CpuCopy16(c->buf[c->cur ^ 1], c->dst, c->size);
        }
    }
    func_02035754();
    func_020363f0();
    func_0203383c();
    func_02036f94();
    func_02036ee8();
    z = 0;
    MIi_CpuClear16(z, data_0208f78e, 0x40);
    func_0201b3a0();
    func_02007b3c(3);
}
