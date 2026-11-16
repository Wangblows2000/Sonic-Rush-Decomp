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

extern UnkDisp_0203005c gDisplayMain;
extern UnkDisp_0203005c gDisplaySub;
extern UnkCopy_0203005c data_02087cdc[];
extern u32 gSystemFlags;
extern u16 data_0208f78e[];

void DC_StoreRange(void *p, u32 size);
void DC_WaitWriteBufferEmpty(void);
void func_02007da4(u32 dma, const void *src, volatile void *dst, u32 size);
void func_02007d2c(u32 dma, const void *src, volatile void *dst, u32 size);
void GXx_SetMasterBrightness_(volatile u16 *reg, s32 brightness);
void Oam_Upload(u32 engine);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void PaletteTransfer_Flush(void);
void ScreenTransfer_Flush(void);
void VRAMTransfer_Flush(void);
void Task_RunVBlank(void);
void Task_FreeDestroyed(void);
void func_0201b3a0(void);
void func_02007b3c(u32 dma);

void Display_VBlankUpdate(void) {
    u32 i;
    UnkCopy_0203005c *c;
    vu16 z;
    DC_StoreRange(&gDisplayMain, sizeof(UnkDisp_0203005c));
    DC_WaitWriteBufferEmpty();
    func_02007da4(3, gDisplayMain.bgcnt, (vu32 *)0x04000010, 0x10);
    func_02007da4(3, gDisplayMain.bgofs, (vu32 *)0x04000040, 0xC);
    *(vu32 *)0x04000000 = (*(vu32 *)0x04000000 & ~0xE000) | (gDisplayMain.dispMode << 13);
    func_02007d2c(3, gDisplayMain.win, (vu16 *)0x04000050, 6);
    GXx_SetMasterBrightness_((vu16 *)0x0400006C, gDisplayMain.brightness);
    *(vu16 *)0x0400004C = gDisplayMain.mosaic;
    func_02007da4(3, gDisplayMain.bgAffine, (vu32 *)0x04000020, 0x10);
    func_02007da4(3, gDisplayMain.unk38, (vu32 *)0x04000030, 0x10);
    Oam_Upload(0);
    DC_StoreRange(&gDisplaySub, sizeof(UnkDisp_0203005c));
    DC_WaitWriteBufferEmpty();
    func_02007da4(3, gDisplaySub.bgcnt, (vu32 *)0x04001010, 0x10);
    func_02007da4(3, gDisplaySub.bgofs, (vu32 *)0x04001040, 0xC);
    *(vu32 *)0x04001000 = (*(vu32 *)0x04001000 & ~0xE000) | (gDisplaySub.dispMode << 13);
    func_02007d2c(3, gDisplaySub.win, (vu16 *)0x04001050, 6);
    GXx_SetMasterBrightness_((vu16 *)0x0400106C, gDisplaySub.brightness);
    *(vu16 *)0x0400104C = gDisplaySub.mosaic;
    func_02007da4(3, gDisplaySub.bgAffine, (vu32 *)0x04001020, 0x10);
    func_02007da4(3, gDisplaySub.unk38, (vu32 *)0x04001030, 0x10);
    Oam_Upload(1);
    for (i = 0, c = data_02087cdc; i < 2; i++, c++) {
        if (gSystemFlags & (0x400 << i)) {
            MIi_CpuCopy16(c->buf[c->cur ^ 1], c->dst, c->size);
        }
    }
    PaletteTransfer_Flush();
    ScreenTransfer_Flush();
    VRAMTransfer_Flush();
    Task_RunVBlank();
    Task_FreeDestroyed();
    z = 0;
    MIi_CpuClear16(z, data_0208f78e, 0x40);
    func_0201b3a0();
    func_02007b3c(3);
}
