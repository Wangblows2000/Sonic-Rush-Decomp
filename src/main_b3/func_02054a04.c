#include <nitro/types.h>

typedef struct { u8 pad0[8]; s16 unk8; } UnkBlend_02054a04;
typedef struct { u16 a; u16 b; } UnkPair_02054a04;
typedef struct {
    UnkPair_02054a04 p[4]; /* 0x0 */
    u8 pad10[0xc];
    s32 unk1C;
    u16 unk20;
    u8 pad22[0x28];
    u16 unk4A;
} UnkBg_02054a04;
extern UnkBlend_02054a04 data_0208926c;
extern UnkBlend_02054a04 data_02087d44;
extern u32 gSystemFlags;
extern UnkBg_02054a04 *gDisplayTable[];

void GXx_SetMasterBrightness_(vu16 *reg, s32 v);
void Task_DestroyAll(void);
void PaletteTransfer_Clear(void);
void ScreenTransfer_Clear(void);
void VRAMTransfer_Clear(void);
void ObjVram_Reset(u16 a);
void Vram_ResetTexAndPltt(void);
void VramPltt_Reset(void);
void Heap_InitMain(void);
void Heap_InitDTCM(void);
void Heap_InitITCM(void);

#pragma thumb on
void func_02054a04(void) {
    UnkBg_02054a04 *s;
    u16 i;
    u16 j;

    data_0208926c.unk8 = -0x10;
    data_02087d44.unk8 = -0x10;
    GXx_SetMasterBrightness_((vu16 *)0x0400006c, data_0208926c.unk8);
    GXx_SetMasterBrightness_((vu16 *)0x0400106c, data_02087d44.unk8);
    Task_DestroyAll();
    PaletteTransfer_Clear();
    ScreenTransfer_Clear();
    VRAMTransfer_Clear();
    gSystemFlags &= ~0xc00;
    for (i = 0; i < 2; i++) {
        s = gDisplayTable[i];
        for (j = 0; j < 4; j++)
            s->p[j].a = s->p[j].b = 0;
        s->unk1C = 0;
        s->unk20 &= ~0xc0;
        s->unk4A = 0;
        ObjVram_Reset(i);
        ObjVram_Reset(i);
    }
    Vram_ResetTexAndPltt();
    VramPltt_Reset();
    Heap_InitMain();
    Heap_InitDTCM();
    Heap_InitITCM();
}
#pragma thumb reset
